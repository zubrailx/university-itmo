`timescale 1ns / 1ps


module main(
    input CLK100MHZ,
    input [15:0] SW,
    input BTNU, BTND, BTNL, BTNR, BTNC,
    // 7-segment
    output CA, CB, CC, CD, CE, CF, CG,
    output [7:0] AN,
    // led
    output [15:0] LED
);

    wire btnu, btnd, btnl, btnr, btnc;
    wire [15:0] sw;

    receiver receiver (
        .clk_i(CLK100MHZ),
        .btnu_i(BTNU), .btnd_i(BTND), .btnl_i(BTNL), .btnr_i(BTNR), .btnc_i(BTNC),
        .sw_i(SW),
        .btnu_o(btnu), .btnd_o(btnd), .btnl_o(btnl), .btnr_o(btnr), .btnrc_o(btnc),
        .sw_o(sw)
    );
    
    wire [31:0] data;
    wire [3:0] layout;
    
    logic logic(
        .clk_i(CLK100MHZ),
        .rst_i(btnd),
        .cnt_i(btnu),
        .wr_i(btnl),
        .rd_i(btnr),
        .ev_i(btnc),
        .data_o(data),
        .layout_o(layout)
    );
    
    displayer displayer (
        .clk_i(CLK100MHZ),
        .rst_i(btnd),
        .data_i(data),
        .layout_i(layout),
        .sw_i(sw),
        .CA(CA), .CB(CB), .CC(CC), .CD(CD), .CE(CE), .CF(CF), .CG(CG),
        .AN(AN),
        .LED(LED)
    );
    
    
endmodule