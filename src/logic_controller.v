`timescale 1ns / 1ps

module logic_controller (
    input clk_i,
    input rst_i,
    input btnu_i, btnl_i, btnd_i, btnr_i, btnc_i,
    // whether clap condition is met
    input clap_set_i,
    output rst_o,
    output set_o,
    output reg [2:0] state_o
);

    localparam [2:0]
        COND_CNT_EN = 3'b100,
        COND_LRU_WR = 3'b010,
        COND_LRU_RD = 3'b001;
        
    wire [2:0] cond;
    assign cond = {btnu_i, btnl_i, btnr_i};
    
    assign rst_o = btnd_i;
    
    assign set_o = btnc_i;
        
    // latch state
    always @(posedge clk_i) begin
        if (rst_i) begin
           state_o <= COND_CNT_EN;
        end else begin
            if (clap_set_i) begin
                case (cond)
                    // cycle
                    COND_CNT_EN: state_o <= COND_LRU_WR;
                    COND_LRU_WR: state_o <= COND_LRU_RD;
                    COND_LRU_RD: state_o <= COND_CNT_EN;
                    // start cycle state
                    default: state_o <= COND_CNT_EN;
                endcase
            end else begin
                case (cond)
                    COND_CNT_EN,
                    COND_LRU_WR,
                    COND_LRU_RD: state_o <= cond;
                endcase
            end
        end
    end
endmodule
