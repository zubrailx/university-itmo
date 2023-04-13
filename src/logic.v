`timescale 1ns / 1ps

module logic (
   input clk_i,
   // buttons
   input btnu_i,
   input btnl_i,
   input btnr_i,
   input btnd_i,
   input btnc_i,
   // switch
   input [15:0] sw_i,
   // result and what to display 
   output [31:0] data_o,
   // only to display or not display
   output reg [2:0] state_o
);

    localparam [2:0]
        COND_CNT_EN = 3'b100,
        COND_LRU_WR = 3'b010,
        COND_LRU_RD = 3'b001;
    
    wire [2:0] cond;
    wire rst;
    wire set;
    
    assign cond = {btnu_i, btnl_i, btnr_i};
    assign rst = btnd_i;
    assign set = btnc_i;
    

    wire [31:0] evc_out;
    wire evc_rst;
    wire evc_set;
   
    ev_counter evc (
        .clk_i(clk_i),
        .rst_i(evc_rst),
        .set_i(evc_set),
        .cnt_o(evc_out)
    );
    
    wire lru_rst;
    wire lru_set;
    wire [127:0] lru_buf_array;
    wire [7:0] lru_buf_pres_array;
    
    buffer_lru lru(
        .clk_i(clk_i),
        .rst_i(lru_rst),
        .set_i(lru_set),
        .val_i(sw_i),
        .buf_array_o(lru_buf_array),
        .buf_pres_array_o(lru_buf_pres_array)
    );
            
    // count elements inside buffer
    reg [15:0] buf_ones;
    integer i;
    
    always @(*) begin
        buf_ones = 0;
        for (i = 0; i < 8; i = i + 1) begin
            buf_ones = buf_ones + lru_buf_pres_array[i];
        end
    end
    
    always @(posedge clk_i) begin
        case (cond)
            COND_CNT_EN : state_o <= COND_CNT_EN;
            COND_LRU_WR : state_o <= COND_LRU_WR;
            COND_LRU_RD : state_o <= COND_LRU_RD;
        endcase
    end
    
    // logic
    assign {lru_rst, lru_set} = (state_o == COND_LRU_RD || state_o == COND_LRU_WR)
        ? {rst, set} : 1'b0;

    assign {evc_rst, evc_set} = (state_o == COND_CNT_EN)
        ? {rst, set} : 1'b0;
    
    // output result
    reg [31:0] data;
    assign data_o = data;
    
    wire [2:0] buf_idx;
    assign buf_idx = sw_i[2:0];
    
    always @(*) begin
        case (state_o)
            COND_CNT_EN: begin
                data = evc_out;
            end
            COND_LRU_RD: begin
                data[15:0] = lru_buf_array[16*buf_idx +: 16];
                data[31:16] = buf_ones;
            end
            COND_LRU_WR: begin
                data = sw_i;
                data[31:16] = buf_ones;
            end
            default: begin
                data = 32'hffffffff;
            end
        endcase
    end
    
    
endmodule


module ev_counter(
    input clk_i,
    input rst_i,
    // whether event is on
    input set_i,
    // output digit
    output reg [31:0] cnt_o
);

    reg set_q;
    wire set_chg;
    
    always @(posedge clk_i) set_q <= set_i;
    
    assign set_chg = (set_q != set_i);
    
    always @(posedge clk_i) begin
        if (rst_i) begin
            cnt_o <= 0;
        end else if (set_chg && set_i) begin
            cnt_o <= cnt_o + 1; 
        end
    end

endmodule
