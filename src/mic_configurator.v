`timescale 1ns / 1ps

module mic_configurator
    // 12Hz (max_freq) * 128 (precision) * 2 (>= 2 - Nyquist) = 3072000 Hz
    // 100MHz / 3072000Hz <= 32 - less than 3.3MHz
    #(parameter FREQ_DIV = 32)
(
    input clk_i,
    input rst_i,
    output M_CLK,
    output M_LRSEL
);

    freq_div #(.FREQ_DIV(FREQ_DIV)) fd(
        .clk_i(clk_i),
        .rst_i(rst_i),
        .fdclk_o(M_CLK)
    );
    
    assign M_LRSEL = 1'b0;

endmodule