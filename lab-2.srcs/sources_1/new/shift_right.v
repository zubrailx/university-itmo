`timescale 1ns / 1ps
 
module shift_right (
    input clk,        
    input en,         
    input rst,         
    input wire[31:0] in,  
    output reg[31:0] out,
    output reg carry
    );
 
    always @(in) begin
        out = in;
    end 
 
    always @(posedge clk or posedge rst) begin
        if (rst) begin
            out = 0;
        end else if (en) begin
            carry = out[31];
            out = out[31:1]; 
        end
    end
 
endmodule