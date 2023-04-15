`timescale 1ns / 1ps

module bits2cat_32
     #(parameter DIGIT_PERIOD = 200_000)
     (
        input clk,
        input rst_i,
        input [31:0] data_i,
        output [6:0] CAT,
        output reg [7:0] AN
    );
    
    localparam CNT_WIDTH = $clog2(DIGIT_PERIOD + 1);
    
    reg [2:0] cur_digit;
    
    reg [CNT_WIDTH-1:0] cnt_ff;
    wire [CNT_WIDTH-1:0] cnt_next;
    
    reg [3:0] hex;
    
    bits2cat_4 btc4(
        .hex_i(hex),
        .CAT(CAT)
    );
    
    reg rst_q;
    wire rst_chg;
    
    always @(posedge clk) rst_q <= rst_i;
    assign rst_chg = (rst_q != rst_i);
    
    // set counter
    assign cnt_next = (cnt_ff == DIGIT_PERIOD) ? {CNT_WIDTH{1'b0}} : cnt_ff + 1'b1;
    
    always @(posedge clk) begin
        if (rst_chg && rst_i) begin
            cnt_ff <= {CNT_WIDTH{1'b0}};
        end else begin
            cnt_ff <= cnt_next;
        end
    end
    
    always @(posedge clk) begin
        if (rst_chg && rst_i) begin
            cur_digit <= 0;
            AN <= 8'b11111110;
        end else if (cnt_ff == DIGIT_PERIOD) begin
            cur_digit <= cur_digit + 1;
            AN <= {AN[6:0], AN[7]};
        end
    end
    
    always @(*) begin
        case (cur_digit)
            3'b000: hex = data_i[3:0];
            3'b001: hex = data_i[7:4];
            3'b010: hex = data_i[11:8];
            3'b011: hex = data_i[15:12];
            3'b100: hex = data_i[19:16];
            3'b101: hex = data_i[23:20];
            3'b110: hex = data_i[27:24];
            3'b111: hex = data_i[31:28];
        endcase
    end
    
endmodule


module bits2cat_4(
    input [3:0] hex_i,
    output reg [6:0] CAT
);

    always @(*) begin
        case (hex_i)
            4'h0: CAT = 7'b0000001;
            4'h1: CAT = 7'b1001111;
            4'h2: CAT = 7'b0010010;
            4'h3: CAT = 7'b0000110;
            4'h4: CAT = 7'b1001100;
            4'h5: CAT = 7'b0100100;
            4'h6: CAT = 7'b0100000;
            4'h7: CAT = 7'b0001111;
            4'h8: CAT = 7'b0000000;
            4'h9: CAT = 7'b0000100;
            4'ha: CAT = 7'b0001000;
            4'hb: CAT = 7'b1100000;
            4'hc: CAT = 7'b0110001;
            4'hd: CAT = 7'b1000010;
            4'he: CAT = 7'b0110000;
            4'hf: CAT = 7'b0111000;   
        endcase
    end
endmodule
