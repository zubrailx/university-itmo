`timescale 1ns / 10ps
 
module counter_tb;
 
    reg clk;
    reg en;
    reg rst;
 
    reg[31:0] x;
    wire[31:0] y;
 
    counter cnt(
        .clk(clk),
        .en(en),
        .rst(rst),
        .in(x),
        .out(y)
    );
 
 
    integer i;
    reg[31:0] actual;
    initial begin
        en = 1;
        x = 0;
        for(i = 0; i < 32; i = i + 1) begin
            $display ("Current loop#%0d ", i);
            if (i == 16)
                rst = 1;
                #5
                rst = 0;
                #5
 
 
            if (i == 30)
                en = 0;
                #5
 
            clk = 1;
            #5
            clk = 0;
            #5
 
            if (i == 15) actual = 16;
            else if (i < 30) actual = (i + 1) % 16;
            else actual = 14;
 
            if (actual == y) begin
                $display("Correct! y=%b", y);
            end else begin
                $display("Incorrect! expected=%b, actual=%b", y, actual);
            end
        end
    #10 $stop;
    end
endmodule