`timescale 1ns / 1ps
 
module shift_right
    #(parameter width = 32)
   (input                  clk,
    input                  en,
    input                  rst,
    input      [width-1:0] in,
    output reg             carry);
    
    reg [width-1:0] data;
 
    always @(posedge clk, posedge rst) begin
        if (rst) data <= 0;
        else if (en) begin
            data <= {1'b0, data[width-1:1]};
            carry <= data[0];
        end // if not enabled than read input
        else data <= in;
    end
    
endmodule