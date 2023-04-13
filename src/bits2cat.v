module bits2cat_32
     #(parameter REFRESH_CLOCKS = 200_000)
     (
        input clk,
        input rst_i,
        input [31:0] data_i,
        output CA, CB, CC, CD, CE, CF, CG,
        output reg [7:0] AN
    );
    
    localparam CNT_WIDTH = $clog2(REFRESH_CLOCKS);
    
    reg [2:0] cur_digit;
    
    reg [CNT_WIDTH-1:0] cnt_ff;
    wire [CNT_WIDTH-1:0] cnt_next;
    
    reg [3:0] hex;
    
    bits2cat_4 btc4(
        .hex_i(hex),
        .CA(CA), .CB(CB), .CC(CC), .CD(CD), .CE(CE), .CF(CF), .CG(CG)
    );
    
    // reset only on front
    reg rst_q;
    wire rst_chg;
    
    always @(posedge clk) rst_q <= rst_i;
    
    assign rst_chg = (rst_q != rst_i);
    
    // counter logic
    assign cnt_next = (cnt_ff == REFRESH_CLOCKS) ? {CNT_WIDTH{1'b0}} : cnt_ff + 1'b1;
    
    always @(posedge clk) begin
        if (rst_chg && rst_i) begin
            cnt_ff <= {$clog2(REFRESH_CLOCKS){1'b0}};
        end else begin
            cnt_ff <= cnt_next;
        end
    end
    
    // digic logic
    always @(posedge clk) begin
        if (rst_chg && rst_i) begin
            cur_digit <= 0;
            AN <= 8'b11111110;
        end else if (cnt_ff == REFRESH_CLOCKS) begin
            cur_digit <= cur_digit + 1;
            AN <= {AN[6:0], AN[7]};
        end
    end
    
    always @(cur_digit) begin
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
    output reg CA, CB, CC, CD, CE, CF, CG
    );
    always @(*) begin
        case (hex_i)
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
