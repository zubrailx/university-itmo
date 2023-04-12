`timescale 1ns / 1ps

module displayer (
    input clk_i,
    input rst_i,
    input [31:0] data_i,
    input [3:0] layout_i,
    input [15:0] sw_i,
    output CA, CB, CC, CD, CE, CF, CG,
    output [7:0] AN,
    output [15:0] LED
);

    bits2cat_32 btc32(
        .clk(clk_i),
        .rst_i(rst_i),
        .data_i(data_i),
        .CA(CA), .CB(CB), .CC(CC), .CD(CD), .CE(CE), .CF(CF), .CG(CG),
        .AN(AN)
    );
    
    assign LED = sw_i;

endmodule