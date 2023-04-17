`timescale 1ns / 1ps

module logic (
    input clk_i,
    input rst_i,
    // buttons
    input mod_rst_i,
    input set_i,
    input [2:0] state_i,
    // switch
    input [15:0] sw_i,
    // result
    output [31:0] data_o
);

    localparam [2:0]
        COND_CNT_EN = 3'b100,
        COND_LRU_WR = 3'b010,
        COND_LRU_RD = 3'b001;

    wire [31:0] evc_out;
    wire evc_rst;
    wire evc_set;
   
    ev_counter evc (
        .clk_i(clk_i),
        .rst_i(evc_rst),
        .set_i(evc_set),
        .cnt_o(evc_out)
    );
    
    wire lru_rst;
    wire lru_set;
    wire [127:0] lru_buf_array;
    wire [7:0] lru_buf_pres_array;
    
    buffer_lru lru(
        .clk_i(clk_i),
        .rst_i(lru_rst),
        .set_i(lru_set),
        .val_i(sw_i),
        .buf_array_o(lru_buf_array),
        .buf_pres_array_o(lru_buf_pres_array)
    );
            
    // count elements inside buffer
    reg [15:0] buf_ones;
    integer i;
    
    always @(*) begin
        buf_ones = 0;
        for (i = 0; i < 8; i = i + 1) begin
            buf_ones = buf_ones + lru_buf_pres_array[i];
        end
    end
    
    // lru
    assign lru_rst = (rst_i) ? rst_i
        : (state_i == COND_LRU_RD || state_i == COND_LRU_WR) ? mod_rst_i : 1'b0;
        
    assign lru_set = (state_i == COND_LRU_RD || state_i == COND_LRU_WR) ? set_i : 1'b0;
        
    // evc     
    assign evc_rst = (rst_i) ? rst_i
        : (state_i == COND_CNT_EN) ? mod_rst_i : 1'b0;
        
    assign evc_set = (state_i == COND_CNT_EN) ? set_i : 1'b0;
    
    // output
    reg [31:0] data;
    assign data_o = data;
    
    wire [2:0] buf_idx;
    assign buf_idx = sw_i[2:0];
    
    always @(*) begin
        case (state_i)
            COND_CNT_EN: begin
                data = evc_out;
            end
            COND_LRU_RD: begin
                data[15:0] = lru_buf_array[16*buf_idx +: 16];
                data[31:16] = buf_ones;
            end
            COND_LRU_WR: begin
                data = sw_i;
                data[31:16] = buf_ones;
            end
            default: begin
                data = 32'hffffffff;
            end
        endcase
    end
    
endmodule


module ev_counter(
    input clk_i,
    input rst_i,
    // whether event is on
    input set_i,
    // output digit
    output reg [31:0] cnt_o
);

    reg set_q;
    wire set_posedge;
    
    always @(posedge clk_i) set_q <= set_i;
    assign set_posedge = (set_q != set_i) && set_i;
    
    always @(posedge clk_i) begin
        if (rst_i) begin
            cnt_o <= 0;
        end else if (set_posedge) begin
            cnt_o <= cnt_o + 1; 
        end
    end

endmodule
