`timescale 1ns / 1ps
 
module shift_left
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
            data <= {data[width-1:1],1'b0};
            carry <= data[width-1];
        end // if not enabled than read input
        else data <= in;
    end
    
endmodule
