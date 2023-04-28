`timescale 1ns / 1ps

module buffer_lru
    #(parameter BUF_WIDTH = 16,
                BUF_SIZE = 8)
(

    input clk_i,
    input rst_i,
    input set_i,
    input wire [BUF_WIDTH-1:0] val_i,
    // each element in buffer
    output reg [BUF_SIZE*BUF_WIDTH-1:0] buf_array_o,
    output reg [BUF_SIZE-1:0] buf_pres_array_o
);
    
    localparam IDX_MSB = $clog2(BUF_SIZE) - 1;
    
    // counting elements to store in them insertion ordering
    reg  [IDX_MSB:0] cnt_array [BUF_SIZE-1:0];
    // index where to insert element
    wire [IDX_MSB:0] sel_idx;
    
    reg set_q;
    wire set_chg;
    
    always @(posedge clk_i) set_q <= set_i;
    
    assign set_chg = (set_q != set_i);
    
    integer i;
    
    // counters
    always @(posedge clk_i) begin
        if (rst_i) begin
            for (i = 0; i < BUF_SIZE; i = i + 1) begin
                cnt_array[i] <= i;
            end
        end else if (set_chg && set_i) begin
            for (i = 0; i < BUF_SIZE; i = i + 1) begin
                if (i == sel_idx) begin
                    cnt_array[i] <= BUF_SIZE - 1;
                end
                else if (cnt_array[i] > cnt_array[sel_idx]) begin
                    cnt_array[i] <= cnt_array[i] - 1;
                end
            end
        end
    end
    
    // buf_array
    always @(posedge clk_i) begin
        if (rst_i) begin
            for (i = 0; i < BUF_SIZE; i = i + 1) begin
                buf_array_o[i*BUF_WIDTH +: BUF_WIDTH] <= 0;
                buf_pres_array_o[i] <= 0;
            end
        end else if (set_chg && set_i) begin
            buf_array_o[sel_idx*BUF_WIDTH +: BUF_WIDTH] <= val_i;
            buf_pres_array_o[sel_idx] <= 1;
        end
    end
    
    reg val_present;
    reg [IDX_MSB:0] idx;
    
    assign sel_idx = idx;
    
    // comb logic for calculating index for selection
    always @(*) begin
        val_present = 0;
        idx = 0;
        
        for (i = 0; i < BUF_SIZE; i = i + 1) begin
            if (buf_pres_array_o[i] && val_i == buf_array_o[i*BUF_WIDTH +: BUF_WIDTH]) begin
                val_present = 1;
                idx = i;
            end
        end
        
        if (!val_present) begin
            for (i = 0; i < BUF_SIZE; i = i + 1) begin
                if (cnt_array[i] == 0) begin
                    idx = i;
                end
            end
        end
    end
    
endmodule
