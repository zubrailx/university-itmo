`timescale 1ns / 1ps

module displayer (
    input clk_i,
    input rst_i,
    input [2:0] state_i,
    input [1:0] clap_state_i,
    input [15:0] sw_i,
    input [31:0] data_i,
    input [8:0] amplitude_i,
    output [6:0] CAT,
    output [7:0] AN,
    output [15:0] LED,
    output [2:0] BGR1, BGR2
);

    displayer_data dd(
        .clk_i(clk_i),
        .rst_i(rst_i),
        .state_i(state_i),
        .data_i(data_i),
        .sw_i(sw_i),
        .CAT(CAT),
        .AN(AN)
    );
    
    displayer_amplitude da(
        .clk_i(clk_i),
        .rst_i(rst_i),
        .amplitude_i(amplitude_i),
        .LED(LED)
    );
    
    assign BGR1 = state_i;
    assign BGR2 = clap_state_i;
    
endmodule


module displayer_data
(
    input clk_i,
    input rst_i,
    input [2:0] state_i,
    input [31:0] data_i,
    input [15:0] sw_i,
    output [6:0] CAT,
    output [7:0] AN
    
);
    localparam [2:0]
        COND_CNT_EN = 3'b100,
        COND_LRU_WR = 3'b010,
        COND_LRU_RD = 3'b001;
        
    reg [31:0] btc_data;
    
    bits2cat_32 btc32(
        .clk(clk_i),
        .rst_i(rst_i),
        .data_i(btc_data),
        .CAT(CAT),
        .AN(AN)
    );
    
    always @(*) begin
        case (state_i)
            COND_CNT_EN: begin
                btc_data = data_i;
            end
            COND_LRU_RD: begin
                btc_data = data_i;
            end
            COND_LRU_WR: begin
                btc_data = {data_i[31:16], sw_i};
            end
            default: begin
                btc_data = data_i;
            end
        endcase
    end

endmodule


module displayer_amplitude
    #(parameter DISPLAY_PERIOD = 10_000_000)
(
    input clk_i,
    input rst_i,
    input [8:0] amplitude_i,
    output [15:0] LED
);
    
    localparam CNT_WIDTH = $clog2(DISPLAY_PERIOD + 1);
    
    reg [CNT_WIDTH-1:0] cnt_ff;
    wire [CNT_WIDTH-1:0] cnt_next;
    wire cnt_zero;
    
    assign cnt_next = (cnt_ff == DISPLAY_PERIOD) ? 0 : cnt_ff + 1'b1;
    assign cnt_zero = (cnt_ff == 0);
    
    always @(posedge clk_i) begin
        if (rst_i) begin
            cnt_ff <= 0;
        end else begin
            cnt_ff <= cnt_next;
        end
    end
    
    // current amplitude
    wire [7:0] amp_off_abs_o;

    amp2off_7bit ato7bit(
        .amplitude_i(amplitude_i),
        .amp_off_abs_o(amp_off_abs_o)
    );
    
    // max amplitude in this period
    reg [7:0] max_amp;
    
    always @(posedge clk_i) begin
        if (rst_i || cnt_zero) begin
            max_amp <= 0;
        end else begin
            if (amp_off_abs_o > max_amp) begin
                max_amp <= amp_off_abs_o;
            end
        end
    end
    
    reg [15:0] disp_norm_ampl_ff;
    reg [15:0] disp_norm_ampl_next;
    
    wire [5:0] max_amp_sh2 = max_amp >> 2;
    
    integer i;
    always @(*) begin
        for (i = 0; i < 16; i = i + 1) begin
            disp_norm_ampl_next[i] = (i < max_amp_sh2);
        end
    end
    
    always @(posedge clk_i) begin
        if (rst_i) begin
            disp_norm_ampl_ff <= 0;
        end else if (cnt_zero) begin
            disp_norm_ampl_ff <= disp_norm_ampl_next;
        end
    end
    
    assign LED = disp_norm_ampl_ff;

endmodule