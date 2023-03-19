`timescale 1ns / 1ps

module FSM_4(
    input calc,
    input wire[31:0] a,
    input wire[31:0] b,
    output reg[31:0] res
    );
    
    reg clk = 0;
    reg en = 1;
    reg rst = 0;
    
    reg[31:0] x1, x2, x3;
    wire[31:0] y1, y2, y3;
    
    counter cnt(
            .clk(clk),
            .en(en),
            .rst(rst),
            .in(x1),
            .out(y1)
        );
        
    shift_right shr(
        .clk(clk),
        .en(en),
        .rst(rst),
        .in(x2),
        .out(y2)
    );
    
    shift_left shl(
        .clk(clk),
        .en(en),
        .rst(rst),
        .in(x3),
        .out(y3)
    );
        
    integer i;
    reg[31:0] s1, s2, s3;
    
    always @(posedge calc) begin
        //a+b
        x1 = a;
        for(i = 0; i < b; i = i + 1) begin
            #5 clk = 1;
            #5 clk = 0;
        end
        
        //*4
        x3 = y1;
        #5 clk = 1;
        #5 clk = 0;
        #5 clk = 1;
        #5 clk = 0;
        
        //+b
        x1 = y3;
        for(i = 0; i < b; i = i + 1) begin
            #5 clk = 1;
            #5 clk = 0;
        end
    
        //:2
        x2 = y1;
        #5 clk = 1;
        #5 clk = 0;
        s1 = y2; // s1 = ((a + b) * 4 + b) / 2
        
        
        x2 = b;
        x3 = a;
        #5 clk = 1;
        #5 clk = 0;
        s2 = y2; //s2 = b / 2
        #5 clk = 1;
        #5 clk = 0;
        s3 = y3; //s3 = a * 4
      
        // s1 += s2
        x1 = s1;
        for(i = 0; i < s2; i = i + 1) begin
            #5 clk = 1;
            #5 clk = 0;
        end
        
        //s1 += s3
        for(i = 0; i < s3; i = i + 1) begin
            #5 clk = 1;
            #5 clk = 0;
        end
        
        //res = ((a + b) * 4 + b) / 2 + (b/2 + a*4)
        res = y1;
     end
endmodule
