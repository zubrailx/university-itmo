`timescale 1ns / 10ps
 
module counter(
    input clk,
    input en,
    input rst,
    input wire[31:0] in,
    output reg[31:0] out
    );
 
    always @(in) begin
        out = in;
    end 
 
    always @(posedge clk or posedge rst) begin
        if (rst) begin
            out <= 0;
        end else if (en) begin
            out <= out + 1;
        end
    end
 
endmodule