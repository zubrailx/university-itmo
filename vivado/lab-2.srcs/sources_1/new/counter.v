`timescale 1ns / 10ps
 
module counter
    #(parameter width = 32)
   (input                 clk,
    input                 en,
    input                 rst,
    output reg[width-1:0] out);
 
    always @(posedge clk, posedge rst) begin
        if (rst) begin
            out <= 0;
        end else if (en) begin
            out <= out + 1;
        end
    end
 
endmodule