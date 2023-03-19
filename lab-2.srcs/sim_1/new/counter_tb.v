`timescale 1ns / 10ps

module counter_tb;

    reg clk;
    reg en;
    reg rst;
    wire[31:0] y;

    counter cnt(
        .clk(clk),
        .en(en),
        .rst(rst),
        .out(y)
    );


    integer i;
    reg[31:0] expected;
    initial begin
        // reset counter before the start
        #5 rst = 1;
        #5 rst = 0;

        // start counting
        en = 1;

        for(i = 0; i < 32; i = i + 1) begin
            $display ("Current loop#%0d ", i);
            if (i == 16)
                #5 rst = 1;
                #5 rst = 0;

            if (i == 30)
                #5 en = 0;

            #5 clk = 1;
            #5 clk = 0;

            if (i == 15) expected = 16;
            else if (i < 30) expected = (i + 1) % 16;
            else expected = 14;

            if (expected === y) begin
                $display("Correct! y=%b", y);
            end else begin
                $display("Incorrect! expected=%b, actual=%b", expected, y);
            end
        end
    #10 $stop;
    end
endmodule
