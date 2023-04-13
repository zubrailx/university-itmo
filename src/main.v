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
    output LED16_B, LED16_G, LED16_R, LED17_B, LED17_G, LED17_R,
    // micro
    output M_LRSEL,
    output M_CLK,
    input M_DATA
);

    wire btnu, btnd, btnl, btnr, btnc;
    wire [15:0] sw;

    button_debouncer bd(
        .clk_i(CLK100MHZ),
        // input buttons
        .btnu_i(BTNU), 
        .btnd_i(BTND), 
        .btnl_i(BTNL), 
        .btnr_i(BTNR), 
        .btnc_i(BTNC),
        // output buttons
        .btnu_o(btnu), 
        .btnd_o(btnd), 
        .btnl_o(btnl), 
        .btnr_o(btnr), 
        .btnc_o(btnc)
    );
    
    wire clap;
    
    clap_detector cd(
        .clk_i(CLK100MHZ),
        .M_DATA(M_DATA),
        .M_LRSEL(M_LRSEL),
        .M_CLK(M_CLK),
        .clap_o(clap)
    );
    
    wire [1:0] clap_state;
    wire clap_set;
    
    clap_state cs(
        .clk_i(CLK100MHZ),
        .clap_i(clap),
        .clap_state_o(clap_state),
        .clap_set_o(clap_set)
    );
    
    wire rst, set;
    wire [2:0] state;
    
    state_logic sl(
        .clk_i(CLK100MHZ),
        .btnu_i(btnu), .btnl_i(btnl), .btnd_i(btnd), .btnr_i(btnr), .btnc_i(btnc),
        .clap_set_i(clap_set),
        .rst_o(rst),
        .set_o(set),
        .state_o(state)
    );
    
    wire [31:0] data;
    
    logic logic(
        .clk_i(CLK100MHZ),
        .rst_i(rst),
        .set_i(set),
        .state_i(state),
        .sw_i(sw),
        .data_o(data)
    );
    
    displayer displayer (
        .clk_i(CLK100MHZ),
        .rst_i(btnd),
        .state_i(state),
        .clap_state_i(clap_state),  
        .sw_i(sw),
        .data_i(data),
        .CAT({CA, CB, CC, CD, CE, CG, CG}),
        .AN(AN),
        .LED(LED),
        .BGR1({LED16_B, LED16_G, LED16_R}),
        .BGR2({LED17_B, LED17_G, LED17_R})
    );
    
endmodule