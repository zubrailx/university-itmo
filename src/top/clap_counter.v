`timescale 1ns / 1ps

module clap_counter (
    input CLK100MHZ,
    // micro
    output M_LRSEL,
    inout M_CLK,
    input M_DATA,
    input BTND,
    output [15:0] LED,
    output LED16_B, LED16_G, LED16_R, LED17_B, LED17_G, LED17_R,
    // 7-segment
    output CA, CB, CC, CD, CE, CF, CG,
    output [7:0] AN
);

    wire btnd;

    button_debouncer dbd(
        .clk_i(CLK100MHZ),
        .btn_i(BTND),
        .btn_o(btnd)
    );

    wire clap_pulse;

    clap_detector cd (
        .clk_i(CLK100MHZ),
        .rst_i(btnd),
        .M_DATA(M_DATA),
        .M_LRSEL(M_LRSEL),
        .M_CLK(M_CLK),
        .clap_pulse_o(clap_pulse)
    );
    
    reg [31:0] clap_cnt;
    
    always @(posedge M_CLK) begin
        if (btnd) begin
            clap_cnt <= 0;
        end else begin
            clap_cnt <= clap_cnt + clap_pulse;
        end
    end
    
    // display max amplitude on LEDS
    wire [6:0] CAT;
    assign {CA, CB, CC, CD, CE, CF, CG} = CAT;
    
    bits2cat_32 btc(
        .clk(CLK100MHZ),
        .rst_i(btnd),
        .data_i(clap_cnt),
        .CAT(CAT),
        .AN(AN)
    );
    
endmodule
