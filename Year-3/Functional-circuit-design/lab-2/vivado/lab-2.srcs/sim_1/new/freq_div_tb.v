`timescale 1ns / 1ps


module freq_div_tb;

    reg clk;
    reg en;
    reg rst;
    wire out;
    
    reg tb_out;
    integer i;
    
    freq_div frdiv(
        .clk(clk),
        .en(en),
        .rst(rst),
        .out(out)
    );
    
    reg passed = 1'b1;
    integer test_calls = 0;

    task test;
        begin
            test_calls = test_calls + 1;
            if (tb_out !== out) begin
                $display("[T=%0t, call=%0d] Incorrect! expected=%b, actual=%b", $realtime, test_calls, tb_out, out);
                passed = 1'b0;
            end
        end
    endtask
    
    always #10 clk = ~clk;
    
    initial begin
        
        $monitor("T=%0t: en=%b, rst=%b, cnt=%0d, out=%b", 
            $time, frdiv.en, frdiv.rst, frdiv.cnt, frdiv.out);
            
        clk <= 0;
        en <= 0;
        rst <= 0;
        
        // test value not set
        test();
        
        // reset and check value 
        @(negedge clk) rst <= 1;
        @(negedge clk) rst <= 0;
        
        tb_out = 0;
        
        repeat (10) @(posedge clk)
            test();
            
        // test normal behaviour
        @(negedge clk) begin
            en <= 1;
        end
        
        for (i = 1; i < 50; i = i + 1) begin
            @(posedge clk) begin
                if (i % 5 == 0)
                    tb_out <= ~tb_out;
            end
                
            test();          
        end
        
        // reset and test again
        @(negedge clk) begin
            rst <= 1;
            tb_out <= 0;
        end
        
        // test when reset freq_div is zero
        repeat (10) @(posedge clk) begin
            test();
        end
        
        @(negedge clk) rst <= 0;
        
        for (i = 1; i < 50; i = i + 1) begin
            @(posedge clk) begin
                if (i % 5 == 0)
                    tb_out <= ~tb_out;
            end
                
            test();           
        end
    
        // display test message    
        if (passed == 1'b1)
            $display("[T=%0t] All tests passed.", $realtime);
        else
            $display("[T=%0t] Tests failed.", $realtime);
            
        // finish execution
        #10 $finish;
    end
endmodule
