module bits2cat_32
     #(parameter REFRESH_CLOCKS = 200_000)
     (
        input clk,
        input i_rst,
        input [31:0] i_bits,
        output CA, CB, CC, CD, CE, CF, CG,
        output reg [7:0] AN
    );
    
    reg [2:0] cur_digit;
    reg [$clog2(REFRESH_CLOCKS)-1:0] cnt_ff;
    wire [$clog2(REFRESH_CLOCKS)-1:0] cnt_next;
    
    reg [3:0] btc_bits;
    
    assign cnt_next = (cnt_ff == REFRESH_CLOCKS) ? {$clog2(REFRESH_CLOCKS){1'b0}} : cnt_ff + 1'b1;
    
    always @(posedge clk) begin
        if (i_rst) begin
            cnt_ff <= {$clog2(REFRESH_CLOCKS){1'b0}};
        end else begin
            cnt_ff <= cnt_next;
        end
    end
    
    bits2cat_4 btc(
        btc_bits,
        CA, CB, CC, CD, CE, CF, CG
    );
    
    always @(posedge clk) begin
        if (i_rst) begin
            cur_digit <= 0;
            AN <= 8'b11111110;
        end else if (cnt_ff == REFRESH_CLOCKS) begin
            cur_digit <= cur_digit + 1;
            AN <= {AN[6:0], AN[7]};
        end
    end
    
    always @(cur_digit) begin
        case (cur_digit)
        3'b000: btc_bits = i_bits[3:0];
        3'b001: btc_bits = i_bits[7:4];
        3'b010: btc_bits = i_bits[11:8];
        3'b011: btc_bits = i_bits[15:12];
        3'b100: btc_bits = i_bits[19:16];
        3'b101: btc_bits = i_bits[23:20];
        3'b110: btc_bits = i_bits[27:24];
        3'b111: btc_bits = i_bits[31:28];
        endcase
    end
    
endmodule

module bits2cat_4(
    input [3:0] i_bits,
    output reg CA, CB, CC, CD, CE, CF, CG
    );
    always @(*) begin
        case (i_bits)
            4'b0000: begin
                CA <= 0;
                CB <= 0;
                CC <= 0;
                CD <= 0;
                CE <= 0;
                CF <= 0;
                CG <= 1;
            end
            4'b0001: begin
                CA <= 1;
                CB <= 0;
                CC <= 0;
                CD <= 1;
                CE <= 1;
                CF <= 1;
                CG <= 1;
            end
            4'b0010: begin
                CA <= 0;
                CB <= 0;
                CC <= 1;
                CD <= 0;
                CE <= 0;
                CF <= 1;
                CG <= 0;
            end
            4'b0011: begin
                CA <= 0;
                CB <= 0;
                CC <= 0;
                CD <= 0;
                CE <= 1;
                CF <= 1;
                CG <= 0;
            end
            4'b0100: begin
                CA <= 1;
                CB <= 0;
                CC <= 0;
                CD <= 1;
                CE <= 1;
                CF <= 0;
                CG <= 0;
            end
            4'b0101: begin
                CA <= 0;
                CB <= 1;
                CC <= 0;
                CD <= 0;
                CE <= 1;
                CF <= 0;
                CG <= 0;
            end
            4'b0110: begin
                CA <= 0;
                CB <= 1;
                CC <= 0;
                CD <= 0;
                CE <= 0;
                CF <= 0;
                CG <= 0;
            end
            4'b0111: begin
                CA <= 0;
                CB <= 0;
                CC <= 0;
                CD <= 1;
                CE <= 1;
                CF <= 1;
                CG <= 1;
            end
            4'b1000: begin
                CA <= 0;
                CB <= 0;
                CC <= 0;
                CD <= 0;
                CE <= 0;
                CF <= 0;
                CG <= 0;
            end
            4'b1001: begin
                CA <= 0;
                CB <= 0;
                CC <= 0;
                CD <= 0;
                CE <= 1;
                CF <= 0;
                CG <= 0;
            end
            4'b1010: begin
                CA <= 0;
                CB <= 0;
                CC <= 0;
                CD <= 1;
                CE <= 0;
                CF <= 0;
                CG <= 0;
            end
            4'b1011: begin
                CA <= 1;
                CB <= 1;
                CC <= 0;
                CD <= 0;
                CE <= 0;
                CF <= 0;
                CG <= 0;
            end
            4'b1100: begin
                CA <= 0;
                CB <= 1;
                CC <= 1;
                CD <= 0;
                CE <= 0;
                CF <= 0;
                CG <= 1;
            end
            4'b1101: begin
                CA <= 1;
                CB <= 0;
                CC <= 0;
                CD <= 0;
                CE <= 0;
                CF <= 1;
                CG <= 0;
            end
            4'b1110: begin
                CA <= 0;
                CB <= 1;
                CC <= 1;
                CD <= 0;
                CE <= 0;
                CF <= 0;
                CG <= 0;
            end
            4'b1111: begin
                CA <= 0;
                CB <= 1;
                CC <= 1;
                CD <= 1;
                CE <= 0;
                CF <= 0;
                CG <= 0;
            end        
        endcase
    end
endmodule
