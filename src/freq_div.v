`timescale 1ns / 1ps

module freq_div
    #(parameter FREQ_DIV = 32)
(
    input clk_i,
    input rst_i,
    output reg fdclk_o
);

    localparam CNT_WIDTH = $clog2(FREQ_DIV);
    
    reg [CNT_WIDTH-1:0] cnt_ff;
    wire [CNT_WIDTH-1:0] cnt_next;
    
    assign cnt_next = (cnt_ff == FREQ_DIV - 1) ? {CNT_WIDTH{1'b0}} : cnt_ff + 1'b1;
    
    always @(posedge clk_i) begin
        if (rst_i) begin
            cnt_ff <= {CNT_WIDTH{1'b0}};
        end else begin
            cnt_ff <= cnt_next;
        end
    end
    
    always @(posedge clk_i) begin
        if (rst_i) begin
            fdclk_o <= 0;
        end else if (cnt_ff == FREQ_DIV - 1) begin
            fdclk_o <= ~fdclk_o;
        end
    end

endmodule
