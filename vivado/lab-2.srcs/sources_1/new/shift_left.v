`timescale 1ns / 1ps
 
module shift_left
    #(parameter width = 32)
   (input                  clk,
    input                  en,
    input                  rst,
    input                  l,
    input      [width-1:0] in,
    output                 out);
    
    reg [width-1:0] data;
 
    always @(posedge clk, posedge rst) begin
        if (rst) data <= 0;
        else if (l)
            data <= in;
        else if (en)
            {data[width-1:1]} <= {data[width-2:0], 1'b0};
    end
    
    
    assign out = en ? data[width-1] : 1'b0;
    
endmodule
