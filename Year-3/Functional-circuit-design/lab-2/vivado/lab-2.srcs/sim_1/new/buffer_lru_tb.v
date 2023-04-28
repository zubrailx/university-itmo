`timescale 1ns / 1ps

module buffer_lru_tb;

    parameter 
        WIDTH = 32,
        BUF_SIZE = 8;

    reg clk;
    reg en;
    reg rst;
    reg i_set;
    reg i_val;
    wire [BUF_SIZE-1:0] o_bufs;
    
    buffer_lru blru(
        .clk(clk),
        .en(en),
        .rst(rst),
        .i_set(i_set),
        .i_val(i_val),
        .o_bufs(o_bufs)
    );
    
    reg passed = 1'b1;
    integer test_calls = 0;
    
    integer i, j;
    
    reg [BUF_SIZE-1:0] tb_o_bufs;
    reg [WIDTH-1:0] tb_data_i_set = 32'b10101001110101, 
                    tb_data_i_val = 32'b11101110111111,
                    tb_data_tmp;
    
    task test();
        begin
            test_calls = test_calls + 1;
            if (tb_o_bufs !== o_bufs) begin
                $display("[T=%0t, call=%0d] Incorrect! expected=%b, actual=%b", 
                    $realtime, test_calls, tb_o_bufs, o_bufs);
                passed = 1'b0;
            end
        end
    endtask
            
    always #10 clk = ~clk;
    
    initial begin
            
        clk <= 0;
        rst <= 0;
        
        en <= 0;
        i_set <= 0;
        i_val <= 0;
        
        // test value is not set
        test();
        
        // test in reset state
        @(negedge clk) begin 
            rst <= 1;
            tb_o_bufs <= 0;
        end
        
        repeat (20) @(posedge clk) begin
            test();
        end
        
        // test enable without reset
        @(negedge clk) begin
            rst <= 0;
            en <= 1;
        end
        
        // test when i_set is not set than nothing changes
        repeat(20) @(posedge clk) test();
        
 
        for (i = 0, j = 0; i < 32; i = i + 1) begin
            @(negedge clk) begin
                i_set <= tb_data_i_set[i];
                i_val <= tb_data_i_val[i];
            end
            
            repeat (10) @(posedge clk);
            
            if (i_set) begin
                tb_o_bufs[j % 8] = i_val;
                j = j + 1;
            end
            
            test();
        end
        
        // test reset
        @(negedge clk) rst <= 1;
        @(negedge clk) rst <= 0;
        
        tb_o_bufs = 0;
        test();
        
        // test repeat again:
        // test when i_set is not set than nothing changes
        repeat(20) @(posedge clk) test();
        
        for (i = 0, j = 0; i < 32; i = i + 1) begin
            @(negedge clk) begin
                i_set <= tb_data_i_set[i];
                i_val <= tb_data_i_val[i];
            end
            
            repeat (10) @(posedge clk);
            
            if (i_set) begin
                tb_o_bufs[j % 8] = i_val;
                j = j + 1;
            end
            
            test();
        end
        
        // display test message    
        if (passed == 1'b1)
            $display("[T=%0t] All tests passed.", $realtime);
        else
            $display("[T=%0t] Tests failed.", $realtime);
        
        #10 $finish;
    end
endmodule
