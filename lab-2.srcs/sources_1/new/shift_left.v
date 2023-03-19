`timescale 1ns / 1ps

module shift_left (
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
            out[31:1] = out[30:0];
            out[0] = 0;
        end
    end
    
endmodule
