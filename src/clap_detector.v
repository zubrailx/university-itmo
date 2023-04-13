`timescale 1ns / 1ps

// algorithm to detect whether clap occured
module clap_detector (
    input clk_i,
    // ports to micro
    input M_DATA,
    output M_LRSEL,
    output M_CLK,
    // if over clap threshold
    output clap_o
);

    assign clap_o = 1'b0;


endmodule