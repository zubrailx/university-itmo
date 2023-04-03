`timescale 1ns / 1ps

module adder
    #(parameter WIDTH = 32)
    (input  [WIDTH-1:0] in_l,
     input  [WIDTH-1:0] in_r,
     output [WIDTH-1:0] out);
     
     assign out = in_l + in_r;
endmodule
