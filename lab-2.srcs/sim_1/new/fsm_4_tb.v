`timescale 1ns / 1ps

module FSM_4_tb;

    reg calc;

    reg[31:0] a;
    reg[31:0] b;
    wire[31:0] y;
    
    FSM_4 fsm(
        .calc(calc),
        .a(a),
        .b(b),
        .res(y)
    );
    
    integer i;
    initial begin
        for(i = 1; i < 32; i = i + 1) begin
            $display ("Current loop#%0d ", i);
            a = i;
            b = i * 2;
              
            #5 calc = 1;
            #5 calc = 0;
           
            $display("Correct! y=%0d, 12i=%0d", y, 12 * i);
        end
    #10 $stop;
    end


endmodule
