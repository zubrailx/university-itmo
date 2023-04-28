`timescale 1ns / 1ps

module amplitude_displayer (
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

    wire clap_pulse;

    clap_detector cd (
        .clk_i(CLK100MHZ),
        .M_DATA(M_DATA),
        .M_LRSEL(M_LRSEL),
        .M_CLK(M_CLK),
        .clap_pulse_o(clap_pulse)
    );
    
    wire btnd;

    button_debouncer dbd(
        .clk_i(CLK100MHZ),
        .btn_i(BTND),
        .btn_o(btnd)
    );
    
    reg [6:0] cnt_ff;
    wire [6:0] cnt_next;
    
    // counter logic
    assign cnt_next = (cnt_ff == 127) ? {7'b0} : cnt_ff + 1'b1;
    
    // calculate amplitude and set window
    reg [127:0] window;
    reg [8:0] amplitude;
    reg prev;
    
    always @(posedge M_CLK) begin
        if (btnd) begin
            cnt_ff <= 0;
            window <= 0;
            amplitude <= 0;
            prev <= 0;
        end else begin
            cnt_ff <= cnt_next;
            prev <= window[cnt_next];
            // update amplitude and window
            amplitude <= (amplitude - prev + M_DATA);
            window[cnt_ff] <= M_DATA;
        end
    end

    reg [7:0] amp_off_abs;
    reg [7:0] max_ampl;
    
    reg [15:0] led_lights;
    
    integer i;
    
    always @(*) begin
        // calculate amplitude from center offset [0-64]
        if (amplitude >= 64) begin
            amp_off_abs = amplitude - 64;
        end else begin
            amp_off_abs = 64 - amplitude;
        end
        
        if (amp_off_abs > max_ampl) begin
            max_ampl = amp_off_abs;
        end
        
        amp_off_abs = amp_off_abs >> 2;
        
        
        for (i = 0; i < 16; i = i + 1) begin
            led_lights[i] = (i < amp_off_abs) ? 1'b1 : 1'b0;
        end
    end

    assign LED[15:0] = led_lights;
    
    // display max amplitude on LEDS
    wire [6:0] CAT;
    assign {CA, CB, CC, CD, CE, CF, CG} = CAT;
    
    bits2cat_32 btc(
        .clk(CLK100MHZ),
        .rst_i(btnd),
        .data_i(max_ampl),
        .CAT(CAT),
        .AN(AN)
    );
    
endmodule
