`timescale 1ns / 1ps
 
module shift_right
    #(parameter WIDTH = 32)
   (input                  clk,
    input                  en,
    input                  rst,
    input                  l,
    input      [WIDTH-1:0] in,
    output reg             out); // carry flag
    
    reg [WIDTH-1:0] data;
 
    always @(posedge clk, posedge rst) begin
        if (rst) {out, data} <= 0;
        else if (l) begin
            data <= in;
        end else if (en)
            {data[WIDTH-1:0], out} <= {1'b0, data[WIDTH-1:0]};
        else out <= 0;
    end
    
endmodule