`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 15.05.2023 20:34:24
// Design Name: 
// Module Name: uart
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module uart(
    input CLK100MHZ, BTNC,
    input UART_TXD_IN,
    output UART_RXD_OUT,
        // 7-segment
//    output CA, CB, CC, CD, CE, CF, CG,
//    output [7:0] AN,
    output [15:0] LED
//    input [15:0] SW
    );
    
    reg [31:0] uart_cnt;
    
    localparam BIT_DONE = 10416;
    
    wire bit_done;
    assign bit_done = (uart_cnt >= BIT_DONE);
    
    reg [1:0] state;
    reg [1:0] next_state;
    
        
    localparam [1:0] 
        STATE_IDLE = 2'b00,
        STATE_LOAD = 2'b01,
        STATE_STOP = 2'b10;
    
    always @(posedge CLK100MHZ) begin
        if (BTNC) begin
           uart_cnt <= 0; 
        end 
        else begin
            if (bit_done | (state == STATE_STOP)) begin
                uart_cnt <= 0;
            end else begin
                uart_cnt <= uart_cnt + 1;
            end
        end
    end
    
       
    reg [3:0] bit_cnt;
    wire [3:0] bit_cnt_next;
    assign bit_cnt_next = (state != STATE_LOAD) ? 4'b0 : bit_cnt + 1;
    
    wire bit_en;
    assign bit_en = state == STATE_LOAD & bit_done;
    
    always @(posedge CLK100MHZ) begin
        if (BTNC) begin
           bit_cnt <= 0; 
        end 
        else if (bit_en) begin
            bit_cnt <= bit_cnt_next;
        end
    end
       
    always @(*) begin
        case (state)
            STATE_IDLE: begin
                if (bit_done & UART_TXD_IN == 1'b0) next_state = STATE_LOAD;
                else next_state = STATE_IDLE;
            end
            STATE_LOAD: begin
                if (bit_cnt == 8) next_state = STATE_STOP;
                else next_state = STATE_LOAD;
            end
            STATE_STOP: begin
                next_state = STATE_IDLE;
            end
            default: begin
                next_state = STATE_IDLE;
            end
        endcase 
    end 
    
    always @(posedge CLK100MHZ) begin
        if (BTNC) begin
           state <= STATE_IDLE; 
        end else begin
           state <= next_state;
        end
    end
    
    reg [4:0] state_cnt;
    
    always @(posedge CLK100MHZ) begin
        if (BTNC) begin
            state_cnt <= 0;
        end else if (state == STATE_STOP) begin
            state_cnt <= state_cnt + 1;
        end
    end
    
    reg [7:0] uart_buf;
    wire [7:0] uart_buf_next;
    assign uart_buf_next = (state != STATE_LOAD) ? 8'h00 : {uart_buf[6:0],UART_TXD_IN};
    
    wire uart_buf_en;
    assign uart_buf_en = (state == STATE_LOAD & bit_done);
    
    always @(posedge CLK100MHZ) begin
        if (BTNC) begin
           uart_buf <= 0; 
        end else if (bit_done) begin
           uart_buf <= uart_buf_next;
        end
    end
    
    reg [7:0] led_buf;
    
    always @(posedge CLK100MHZ) begin
        if (BTNC) begin
            led_buf <= 0;
        end else if (state == STATE_STOP) begin
            led_buf <= uart_buf;
        end
    end
    
    assign LED[7:0] = led_buf;
    assign LED [12:8] = state_cnt;
    assign LED[15:13] = state;
    
//    wire [6:0] CAT;
//    assign {CA, CB, CC, CD, CE, CF, CG} = CAT;
    
//    bits2cat_32 btc32(
//        .clk(CLK100MHZ),
//        .rst_i(BTNC),
//        .data_i(),
//        .CAT(CAT),
//        .AN(AN)
//    );
    
endmodule
