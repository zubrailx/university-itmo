`timescale 1ns / 1ps

module logic (
   input clk_i,
   input rst_i,
   // counter
   input cnt_i,
   // buffer-lru
   input wr_i,
   input rd_i,
   // event
   input ev_i,
   // result and what to display 
   output [31:0] data_o,
   output [3:0]  layout_o
);

    wire [31:0] evc_out;
   
    ev_counter evc (
        .clk_i(clk_i),
        .rst_i(rst_i),
        .ev_i(ev_i),
        .cnt_o(evc_out)
    );
    
    assign data_o = evc_out;
    
endmodule


module ev_counter(
    input clk_i,
    input rst_i,
    // whether event is on
    input ev_i,
    // output digit
    output reg [31:0] cnt_o
);

    reg ev_q;
    wire ev_chg;
    
    always @(posedge clk_i) ev_q <= ev_i;
    
    assign ev_chg = (ev_q != ev_i);
    
    always @(posedge clk_i) begin
        if (rst_i) begin
            cnt_o <= 0;
        end else if (ev_chg && ev_i) begin
            cnt_o <= cnt_o + 1; 
        end
    end

endmodule
