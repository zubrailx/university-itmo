`timescale 1ns / 1ps

module displayer (
    input clk_i,
    input rst_i,
    input [2:0] state_i,
    input [1:0] clap_state_i,
    input [15:0] sw_i,
    input [31:0] data_i,
    output [6:0] CAT,
    output [7:0] AN,
    output reg [15:0] LED,
    output [2:0] BGR1, BGR2
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
        LED = 0;
        
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
    
    // display current state
    assign BGR1 = state_i;
    // claps
    assign BGR2 = clap_state_i;

endmodule