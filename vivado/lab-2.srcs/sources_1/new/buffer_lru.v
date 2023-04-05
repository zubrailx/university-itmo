`timescale 1ns / 1ps

module buffer_lru
    #(parameter WIDTH = 32,
                FREQ_DIV = 5,
                BUF_SIZE = 8)
    (   input                       clk,
        input                       en,
        input                       rst,
        input                       i_set,
        input                       i_val,
        output reg [BUF_SIZE-1:0]   o_bufs
    );
        
    wire fd_out;
    
    freq_div fd(
        .clk(clk),
        .en(en),
        .rst(rst),
        .out(fd_out)
    );
    
    reg [WIDTH-1:0] cnt_data[BUF_SIZE-1:0];
    wire cnt_en;
    
    assign cnt_en = i_set & en;
    
    integer i;
        
    always @(posedge fd_out, posedge rst) begin
        if (rst) begin
            o_bufs <= 0;
            
            for (i = 0; i < BUF_SIZE; i = i + 1) begin
                cnt_data[i] = ~i;
            end
        end else begin
            if (cnt_en) begin
                for (i = 0; i < BUF_SIZE; i = i + 1) begin
                    if (cnt_data[i] + 1 == 0) begin
                        cnt_data[i] <= ~BUF_SIZE + 1;
                        o_bufs[i] <= i_val;
                    end else begin
                        cnt_data[i] <= cnt_data[i] + 1;
                    end
                end
            end
        end
    end

endmodule
