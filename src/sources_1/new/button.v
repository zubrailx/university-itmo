`timescale 1ns / 1ps

module button
    #(parameter WAIT_CLOCKS = 1_000_000)
    (
        input clk,
        input i_btn,
        output reg o_out
    );
    
    reg [$clog2(WAIT_CLOCKS)-1:0] one_cnt;
    
    always @(posedge clk) begin
        if (i_btn) begin
            if (one_cnt == WAIT_CLOCKS) begin
                o_out <= 1;
            end else begin
                one_cnt <= one_cnt + 1;
            end
        end else begin
            one_cnt <= 0;
            o_out <= 0;
        end
    end
endmodule
