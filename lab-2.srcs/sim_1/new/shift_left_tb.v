`timescale 1ns / 10ps

module shift_left_tb;

    reg clk;
    reg en;
    reg rst;

    reg[31:0] x;
    wire[31:0] y;
    wire carry;
    
    shift_left shl (
        .clk(clk),
        .en(en),
        .rst(rst),
        .in(x),
        .out(y),
        .carry(carry)
    );
  
    
    integer i;
    reg[31:0] actual = 1;
    initial begin
        en = 1;
        x = actual;
        for(i = 0; i < 32; i = i + 1) begin
            $display ("Current loop#%0d ", i);
            if (i == 16)
               en = 0;
               #5

            if (i == 30)
                rst = 1;
                #5
                rst = 0;
                #5
                
            clk = 1;
            #5
            clk = 0;
            #5
            
            if (i < 16) actual = actual * 2;
            else if (i >= 30) actual = 0;

            if (actual == y) begin
                $display("Correct! y=%b", y);
            end else begin
                $display("Incorrect! expected=%b, actual=%b", y, actual);
            end
        end
    #10 $stop;
    end
endmodule
