`timescale 1ns / 1ps

module clap_controller (
    input clk_i,
    input rst_i,
    input clap_i,
    output reg [1:0] clap_state_o,
    output clap_set_o
);
    localparam [1:0]
        CLAP_ZERO = 2'b00,
        CLAP_ONE = 2'b01,
        CLAP_TWO = 2'b10;

    assign clap_set_o = (clap_state_o == CLAP_TWO);
    
    always @(posedge clk_i) begin
        if (rst_i) begin
            clap_state_o <= CLAP_ZERO;
        end else begin
        end
    end
    
endmodule