`timescale 1ns / 1ps

module clap_controller
    #(parameter MAX_CLKS_BTWN_CLAPS = 100_000_000)
(
    input clk_i,
    input rst_i,
    input clap_pulse_i,
    output reg [1:0] clap_state_o,
    output clap_set_o
);
    localparam [1:0]
        CLAP_ZERO = 2'b00,
        CLAP_ONE = 2'b01,
        CLAP_TWO = 2'b10,
        CLAP_MANY = 2'b11;
        
    localparam CNT_WIDTH = $clog2(MAX_CLKS_BTWN_CLAPS + 1);
    
    reg [CNT_WIDTH-1:0] cnt_ff;
    wire [CNT_WIDTH-1:0] cnt_next;
    
    reg clap_q;
    wire clap_posedge;
    
    always @(posedge clk_i) clap_q <= clap_pulse_i;
    assign clap_posedge  = (clap_q != clap_pulse_i) && clap_pulse_i;
    
    reg [1:0] clap_state_next;
    
    assign cnt_next = (clap_state_next == CLAP_ZERO || clap_pulse_i) ? 0 : cnt_ff + 1;
    
    always @(posedge clk_i) begin
        if (rst_i) begin
            cnt_ff <= 0;
        end else begin
            cnt_ff <= cnt_next;
        end
    end
    
    always @(*) begin
        if (clap_posedge && cnt_ff < MAX_CLKS_BTWN_CLAPS) begin
            case(clap_state_o)
                CLAP_ZERO: clap_state_next = CLAP_ONE;
                CLAP_ONE : clap_state_next = CLAP_TWO;
                CLAP_TWO : clap_state_next = CLAP_MANY;
                CLAP_MANY: clap_state_next = CLAP_MANY;
            endcase
        end else if (cnt_ff == MAX_CLKS_BTWN_CLAPS) begin
            clap_state_next = CLAP_ZERO;
        end else begin
            clap_state_next = clap_state_o;
        end
    end
    
    always @(posedge clk_i) begin
        if (rst_i) begin
            clap_state_o <= CLAP_ZERO;
        end else begin
            clap_state_o <= clap_state_next;
        end
    end
    
    assign clap_set_o = (clap_state_o == CLAP_TWO);
    
endmodule