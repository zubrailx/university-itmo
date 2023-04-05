`timescale 1ns / 1ps


module buffer_lru_sv
    #(parameter WIDTH = 32,
                FREQ_DIV = 5,
                BUF_SIZE = 8)
    (   input                       clk,
        input                       en,
        input                       rst,
        input                       i_set,
        input wire [WIDTH-1:0]      i_val,
        output reg [WIDTH-1:0]      o_bufs[BUF_SIZE-1:0]
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
    
    logic val_found;
    logic [$clog2(BUF_SIZE)-1:0] idx_upd;
    
    integer i;
        
    always @(posedge fd_out, posedge rst) begin
        if (rst) begin
            for (i = 0; i < BUF_SIZE; i = i + 1) begin
                o_bufs[i] <= 0;
                cnt_data[i] <= ~i;
            end
        end else begin
            if (cnt_en) begin
                // update counter values  
                for (i = 0; i < BUF_SIZE; i = i + 1) begin
                    if (i == idx_upd) begin
                        o_bufs[i] <= i_val; // no different between writing same or not writing
                        cnt_data[i] <= ~BUF_SIZE + 1;
                    end else begin // update only those whose value is loser than idx_upd
                        if (cnt_data[i] < cnt_data[idx_upd]) begin
                            cnt_data[i] <= cnt_data[i] + 1;
                        end
                    end
                end
            end
        end
    end
    
    always @(*) begin
        val_found = 1'b0;
        
        // find idx which stores same value
        for (i = 0; i < BUF_SIZE; i = i + 1) begin
            if (i_val == o_bufs[i]) begin
                val_found = 1'b1;
                idx_upd = i;
            end
        end
        
        // find idx where to insert different value
        if (!val_found) begin
            for (i = 0; i < BUF_SIZE; i = i + 1) begin
                if (cnt_data[i] + 1 == 0) begin
                    idx_upd = i;
                end
            end
        end
    end
endmodule
