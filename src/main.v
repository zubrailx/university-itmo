`timescale 1ns / 1ps


module main(
    input CLK100MHZ,
    input [15:0] SW,
    input BTNU, BTND, BTNL, BTNR, BTNC,
    // 7-segment
    output CA, CB, CC, CD, CE, CF, CG,
    output [7:0] AN,
    // led
    output [15:0] LED,
    output LED16_B, LED16_G, LED16_R, LED17_B, LED17_G, LED17_R
);

    wire btnu, btnd, btnl, btnr, btnc;
    wire [15:0] sw;

    receiver receiver (
        .clk_i(CLK100MHZ),
        .btnu_i(BTNU), .btnd_i(BTND), .btnl_i(BTNL), .btnr_i(BTNR), .btnc_i(BTNC),
        .sw_i(SW),
        .btnu_o(btnu), .btnd_o(btnd), .btnl_o(btnl), .btnr_o(btnr), .btnc_o(btnc),
        .sw_o(sw)
    );
    
    wire [127:0] data;
    wire [7:0] buf_pres_array;
    wire [2:0] state;
    
    logic logic(
        .clk_i(CLK100MHZ),
        .btnu_i(btnu),
        .btnl_i(btnl),
        .btnr_i(btnr),
        .btnd_i(btnd),
        .btnc_i(btnc),
        .sw_i(sw),
        .data_o(data),
        .state_o(state)
    );
    
    displayer displayer (
        .clk_i(CLK100MHZ),
        .rst_i(btnd),
        .data_i(data),
        .sw_i(sw),
        .state_i(state),
        .CA(CA), .CB(CB), .CC(CC), .CD(CD), .CE(CE), .CF(CF), .CG(CG),
        .AN(AN),
        .LED(LED),
        .BGR1({LED16_B, LED16_G, LED16_R}),
        .BGR2({LED17_B, LED17_G, LED17_R})
    );
    
    
endmodule