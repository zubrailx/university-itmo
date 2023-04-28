`timescale 1ns / 1ps

module button_debouncer
    #(parameter WAIT_CLOCKS = 1_000_000)
    (
        input clk_i,
        input btn_i,
        output reg btn_o
    );
    
    reg [$clog2(WAIT_CLOCKS)-1:0] cnt;
    
    always @(posedge clk_i) begin
        if (btn_i) begin
            if (cnt == WAIT_CLOCKS) begin
                btn_o <= 1;
            end else begin
                cnt <= cnt + 1;
            end
        end else begin
            cnt <= 0;
            btn_o <= 0;
        end
    end
endmodule
