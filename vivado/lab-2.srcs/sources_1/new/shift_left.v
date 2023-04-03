`timescale 1ns / 1ps
 
module shift_left
    #(parameter width = 32)
   (input                  clk,
    input                  en,
    input                  rst,
    input                  l,
    input      [width-1:0] in,
    output reg             out); // carry flag
    
    reg [width-1:0] data;
 
    always @(posedge clk, posedge rst) begin
        if (rst) {out, data} <= 0;
        else if (l) begin
            data <= in;
        end else if (en)
            {out, data[width-1:0]} <= {data[width-1:0], 1'b0};
        else out <= 0;
    end
    
endmodule
