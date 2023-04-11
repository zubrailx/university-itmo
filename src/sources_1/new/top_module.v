`timescale 1ns / 1ps


module top_module(
    input CLK100MHZ,
    input BTNC,
    input BTND,
//    output reg [0:0] LED,
    output CA, CB, CC, CD, CE, CF, CG,
    output [7:0] AN
    );
    
    wire btnc_out;
        
    reg digit_was_inc;
    reg [31:0] digit;
    
    button btnc(
        .clk(CLK100MHZ),
        .i_btn(BTNC),
        .o_out(btnc_out)
    );
    
    bits2cat_32 btc (
        CLK100MHZ,
        BTND,
        digit,
        CA, CB, CC, CD, CE, CF, CG,
        AN
    );
    
    always @(posedge CLK100MHZ) begin
        if (BTND) begin
            digit <= 0;
            digit_was_inc <= 0;
        end
        
        if (btnc_out) begin
            digit_was_inc <= 1;
            if (!digit_was_inc) begin
                digit <= digit + 1;
            end
        end else begin
            digit_was_inc <= 0;
        end
    end
endmodule