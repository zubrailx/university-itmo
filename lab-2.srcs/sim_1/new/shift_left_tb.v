`timescale 1ns / 10ps

module shift_left_tb;

    reg clk;
    reg en;
    reg rst;
    reg[31:0] x;
    wire carry;

    shift_right shr (
        .clk(clk),
        .en(en),
        .rst(rst),
        .in(x),
        .carry(carry)
    );

    integer i;
    reg [32:0] value = 65536; // on iter 16 should be 1
    reg expected;
    initial begin
        x = value;

        rst = 0;
        en = 0; // load value
        #5 clk = 1;
        #5 clk = 0;

        en = 1; // start shifting
        for (i = 0; i < 32; i = i + 1) begin
            $display ("Current loop#%0d ", i);

            expected = value[32];

            if (i == 20) begin
                #5 en = 0;
            end

            if (i == 30) begin
                #5 rst = 1;
                #5 rst = 0;
            end

            #5 clk = 1;
            #5 clk = 0;

            if (i < 20) value = value * 2;
            else if (i >= 30) value = 0;

            if (expected == carry) begin
                $display("Correct! y=%b", carry);
            end else begin
                $display("Incorrect! expected=%b, actual=%b", expected, carry);
            end

        end
        #10 $stop;
    end
endmodule
