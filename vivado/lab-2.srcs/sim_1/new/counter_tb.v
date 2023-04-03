`timescale 1ns / 10ps

module counter_tb;

    reg clk;
    reg en;
    reg rst;
    wire[31:0] out;

    counter cnt(
        .clk(clk),
        .en(en),
        .rst(rst),
        .out(out)
    );

    always # 10 clk = ~clk;

    reg [31:0] tb_out;
    integer i;
    reg passed = 1'b1;
    
    integer test_calls = 0;
    
    task test;
        begin
            test_calls = test_calls + 1;
            if (tb_out !== out) begin
                $display("[T=%0t] Incorrect! expected=%b, actual=%b", $realtime, tb_out, out);
                passed = 1'b0;
            end
        end
    endtask
    
    
    initial begin
        clk <= 0;
        rst <= 0;
        
        repeat (5) @(posedge clk)
            test();
        
        @(posedge clk) begin // wait until posedge
            rst <= 1;
            tb_out <= 0;
        end
           
        test();
        
        @(negedge clk) begin
            rst <= 0;
            en <= 1;
        end

        // test when counting
        for (i = 1; i <= 16; i = i + 1) begin
            @(posedge clk)
                tb_out <= i;
                
            test();  
        end
        
        // test not counting
        @(negedge clk) begin
            en <= 0;
            tb_out <= 16;
        end
            
        repeat (16) @(posedge clk)
            test();
        
        // continue counting
        @(negedge clk)
            en <= 1;
             
        for (i = i; i <= 32; i = i + 1) begin
            @(posedge clk)
                tb_out <= i;
                
            test();   
        end
        
        // reset counter
        @(posedge clk) begin
            rst <= 1;
            tb_out <= 0;
        end
        
        test();
        
        // display test message    
        if (passed == 1'b1)
            $display("[T=%0t] All tests passed.", $realtime);
        else
            $display("[T=%0t] Tests failed.", $realtime);
            
        // finish execution
        #10 $finish;
    end
endmodule
