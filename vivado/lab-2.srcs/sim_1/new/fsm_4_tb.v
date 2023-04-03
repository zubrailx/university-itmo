`timescale 1ns / 1ps

module FSM_4_tb;

    reg clk;
    reg[31:0] a;
    reg[31:0] b;
    reg rst;
    wire[31:0] out;
    wire rdy;
      
    FSM_4 fsm(
        .clk(clk),
        .a(a),
        .b(b),
        .rst(rst),
        .out(out),
        .rdy(rdy)
    );
        
    always #10 clk = ~clk;
    
    reg[31:0] tb_out;
    reg       tb_rdy;
    
    reg passed = 1'b1;
    integer test_calls = 0;
    
    task test;
        begin
            test_calls = test_calls + 1;
            if (tb_out !== out) begin
                $display("[T=%0t, call=%0d] Incorrect! expected_out=%0d, actual=%0d", $realtime, test_calls, tb_out, out);
                passed = 1'b0;
            end
            if (rdy !== tb_rdy) begin
                $display("[T=%0t, call=%0d] Incorrect! expected_rdy=%b, actual=%b", $realtime, test_calls, tb_out, out);
                passed = 1'b0;
            end
        end
    endtask
    
    task fsm_task(input reg[31:0] a, b, output reg[35:0] res);
        begin
            res = ((a + b)*4 + b)/2 + (b/2 + a*4);
        end
    endtask
    
    initial begin
    
        $monitor("T=%0t: state=%b, reg1=%0d, reg2=%0d, reg3=%00d, reg4=%0d, rdy=%b, out=%0d", 
            $time, fsm.state, fsm.reg1, fsm.reg2, fsm.reg3, fsm.reg4, fsm.rdy, fsm.out);
            
        clk <= 0;
        
        // test with a = 10 and b = 5
        @(negedge clk) rst <= 1;
        @(negedge clk) begin
            rst <= 0;
            a <= 32'd10;
            b <= 32'd5;
        end
        
        while(!rdy) begin
            @(posedge clk);
        end
                
        tb_rdy = 1;
        fsm_task(a, b, tb_out);
        
        test();
        
        // test reset
        @(negedge clk) rst <= 1;
        @(negedge clk) begin
            rst <= 0;
            a <= 32'd9999;
            b <= 32'd8888;
        end
        
        tb_out = 0;
        tb_rdy = 0;
        test();
        
        // test repeat again
        while(!rdy) begin
            @(posedge clk);
        end
                
        tb_rdy = 1;
        fsm_task(a, b, tb_out);
        
        test();
        
        // display test message    
        if (passed == 1'b1)
            $display("[T=%0t] All tests passed.", $realtime);
        else
            $display("[T=%0t] Tests failed.", $realtime);
        
        #10 $finish;
    end
    

endmodule
