`timescale 1ns / 1ps

module freq_div
    #(parameter WIDTH = 32,
                FREQ_DIV = 5)
    (   input clk,
        input en,
        input rst,
        output reg out);
        
    // don't want to use my counter because 
    // can't perform reset in one tick (only if not on posedge of clk)
    reg [$clog2(FREQ_DIV)-1:0] cnt;
        
    always @(posedge clk, posedge rst) begin
        if (rst) begin
            out <= 0;
            cnt <= 0;
        end else if (en) begin
            if (cnt == FREQ_DIV - 1) begin
                cnt <= 0;
                out <= ~out;
            end else begin
                cnt <= cnt + 1;
            end
        end
    end
endmodule
