module bits2cat_32
     #(parameter REFRESH_CLOCKS = 200_000)
     (
        input clk,
        input rst_i,
        input [31:0] data_i,
        output [6:0] CAT,
        output reg [7:0] AN
    );
    
    localparam CNT_WIDTH = $clog2(REFRESH_CLOCKS);
    
    reg [2:0] cur_digit;
    
    reg [CNT_WIDTH-1:0] cnt_ff;
    wire [CNT_WIDTH-1:0] cnt_next;
    
    reg [3:0] hex;
    
    bits2cat_4 btc4(
        .hex_i(hex),
        .CAT(CAT)
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
            4'b0000: begin
                CAT = { 1'b0, 1'b0, 1'b0, 1'b0, 1'b0, 1'b0, 1'b1 };
            end
            4'b0001: begin
                CAT = { 1'b1, 1'b0, 1'b0, 1'b1, 1'b1, 1'b1, 1'b1 };
            end
            4'b0010: begin
                CAT = { 1'b0, 1'b0, 1'b1, 1'b0, 1'b0, 1'b1, 1'b0 };
            end
            4'b0011: begin
                CAT = { 1'b0, 1'b0, 1'b0, 1'b0, 1'b1, 1'b1, 1'b0 };
            end
            4'b0100: begin
                CAT = { 1'b1, 1'b0, 1'b0, 1'b1, 1'b1, 1'b0, 1'b0 };
            end
            4'b0101: begin
                CAT = { 1'b0, 1'b1, 1'b0, 1'b0, 1'b1, 1'b0, 1'b0 };
            end
            4'b0110: begin
                CAT = { 1'b0, 1'b1, 1'b0, 1'b0, 1'b0, 1'b0, 1'b0 };
            end
            4'b0111: begin
                CAT = { 1'b0, 1'b0, 1'b0, 1'b1, 1'b1, 1'b1, 1'b1 };
            end
            4'b1000: begin
                CAT = { 1'b0, 1'b0, 1'b0, 1'b0, 1'b0, 1'b0, 1'b0 };
            end
            4'b1001: begin
                CAT = { 1'b0, 1'b0, 1'b0, 1'b0, 1'b1, 1'b0, 1'b0 };
            end
            4'b1010: begin
                CAT = { 1'b0, 1'b0, 1'b0, 1'b1, 1'b0, 1'b0, 1'b0 };
            end
            4'b1011: begin
                CAT = { 1'b1, 1'b1, 1'b0, 1'b0, 1'b0, 1'b0, 1'b0 };
            end
            4'b1100: begin
                CAT = { 1'b0, 1'b1, 1'b1, 1'b0, 1'b0, 1'b0, 1'b1 };
            end
            4'b1101: begin
                CAT = { 1'b1, 1'b0, 1'b0, 1'b0, 1'b0, 1'b1, 1'b0 };
            end
            4'b1110: begin
                CAT = { 1'b0, 1'b1, 1'b1, 1'b0, 1'b0, 1'b0, 1'b0 };
            end
            4'b1111: begin
                CAT = { 1'b0, 1'b1, 1'b1, 1'b1, 1'b0, 1'b0, 1'b0 };
            end        
        endcase
    end
endmodule
