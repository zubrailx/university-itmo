`timescale 1ns / 1ps


module main(
    input CLK100MHZ,
    input [15:0] SW,
    input BTNU, BTND, BTNL, BTNR, BTNC,
    // reset button
    input CPU_RESETN,
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

    wire btnu, btnd, btnl, btnr, btnc, cpu_resetn;
    
    wire rst;
    assign rst = ~cpu_resetn;
    
    genvar i; 
    generate
        wire [5:0] btns_in, btns_out;
        
        assign btns_in = {BTNU, BTND, BTNL, BTNR, BTNC, CPU_RESETN};
        assign {btnu, btnd, btnl, btnr, btnc, cpu_resetn} = btns_out;
    
        for (i = 0; i <= 5; i = i + 1) begin
            button_debouncer bd(
                .clk_i(CLK100MHZ),
                .btn_i(btns_in[i]),
                .btn_o(btns_out[i])
            );
        end
    endgenerate
        
    mic_configurator mc(
        .clk_i(CLK100MHZ),
        .rst_i(rst),
        .M_CLK(M_CLK),
        .M_LRSEL(M_LRSEL)
    );
    
    wire [8:0] amplitude;
    
    amplituder_7bit am7bit(
        .M_CLK(M_CLK),
        .rst_i(rst),
        .M_DATA(M_DATA),
        .amplitude_o(amplitude)
    );
    
    wire clap_pulse;
    
    clap_detector cd(
        .M_CLK(M_CLK),
        .rst_i(rst),
        .amplitude_i(amplitude),
        .clap_pulse_o(clap_pulse)
    );
    
    wire [1:0] clap_state;
    wire clap_set;
    
    clap_controller cc(
        .clk_i(CLK100MHZ),
        .rst_i(rst),
        .clap_pulse_i(clap_pulse),
        .clap_state_o(clap_state),
        .clap_set_o(clap_set)
    );
    
    wire lc_rst, lc_set;
    wire [2:0] state;
    
    logic_controller lc(
        .clk_i(CLK100MHZ),
        .rst_i(rst),
        .btnu_i(btnu), .btnl_i(btnl), .btnd_i(btnd), .btnr_i(btnr), .btnc_i(btnc),
        .clap_set_i(clap_set),
        .rst_o(lc_rst),
        .set_o(lc_set),
        .state_o(state)
    );
    
    wire [31:0] data;
    
    logic logic(
        .clk_i(CLK100MHZ),
        .rst_i(rst),
        .mod_rst_i(lc_rst),
        .set_i(lc_set),
        .state_i(state),
        .sw_i(SW),
        .data_o(data)
    );
    
    wire [6:0] CAT;
    assign {CA, CB, CC, CD, CE, CF, CG} = CAT;
    
    wire [2:0] BGR1, BGR2;
    assign {LED16_B, LED16_G, LED16_R} = BGR1;
    assign {LED17_B, LED17_G, LED17_R} = BGR2;
    
    displayer displayer (
        .clk_i(CLK100MHZ),
        .rst_i(rst),
        .state_i(state),
        .clap_state_i(clap_state),  
        .sw_i(SW),
        .data_i(data),
        .amplitude_i(amplitude),
        .CAT(CAT),
        .AN(AN),
        .LED(LED),
        .BGR1(BGR1),
        .BGR2(BGR2)
    );
    
endmodule