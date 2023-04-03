`timescale 1ns / 10ps

module shift_right_tb;

    reg clk;
    reg en;
    reg rst;
    reg l;
    reg[31:0] in;
    wire out;

    shift_right shr (
        .clk(clk),
        .en(en),
        .rst(rst),
        .l(l),
        .in(in),
        .out(out)
    );
    
    always #10 clk = ~clk;
    
    reg tb_out;
    integer i;
    
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
    
    reg [31:0] test_in1 = 32'hFF0FFAFF;
    reg [31:0] test_tmp;
    
    initial begin
        clk <= 0;
        en <= 0;
        rst <= 0;
        l <= 0;
        
        repeat (5) @(posedge clk) begin
            tb_out <= 0;
            test();
        end
            
        // test out is zero when buffer is zero
        @(posedge clk) begin
            rst <= 1;
            tb_out <= 0;
        end
        
        test();
        
        @(negedge clk) begin
            rst <= 0;
            en <= 1;
        end
        
        // test shifting when buffer is eq 0
        for (i = 1; i <= 32; i = i + 1) begin
            @(posedge clk)
                tb_out <= 0;
                
            test();  
        end
        
        // test when loading value
        @(negedge clk) begin
            l <= 1;
            in <= test_in1;
            test_tmp <= test_in1;
        end
        
        @(negedge clk) begin
            l <= 0;
        end
        
        for (i = 0; i <= 32; i = i + 1) begin
            @(posedge clk) begin
                {test_tmp[31:0], tb_out} <= {1'b0, test_tmp[31:0]};
            end
            
            test();
        end
        
        // test reset
        @(negedge clk) begin
            l <= 1;
            in <= test_in1;
            test_tmp <= test_in1;
        end
        
        @(negedge clk) begin
            l <= 0;
        end
        
        for (i = 0; i <= 16; i = i + 1) begin
            @(posedge clk) begin
                {test_tmp[31:0], tb_out} <= {1'b0, test_tmp[31:0]};
            end
            
            test();
        end
        
        @(negedge clk) begin 
            rst <= 1;
            tb_out <= 0;
        end
        
        @(posedge clk) test();
       
        // display test message    
        if (passed == 1'b1)
            $display("[T=%0t] All tests passed.", $realtime);
        else
            $display("[T=%0t] Tests failed.", $realtime);
            
        // finish execution
        #10 $finish;
    end
endmodule
