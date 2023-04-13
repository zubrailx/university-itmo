`timescale 1ns / 1ps

module buffer_lru_sv_tb;

    parameter 
        WIDTH = 16,
        BUF_SIZE = 8;

    reg clk;

    reg rst;
    reg set_i;
    reg [WIDTH-1:0] val_i;
    wire [BUF_SIZE-1:0][WIDTH-1:0] bufs_o;
    
    buffer_lru blru(
        .clk_i(clk),
        .rst_i(rst),
        .set_i(set_i),
        .val_i(val_i),
        .buf_array_o(bufs_o)
    );
    
    reg passed = 1'b1;
    integer test_calls = 0;
    
    integer i, j, t;
    
    reg [BUF_SIZE-1:0][WIDTH-1:0] tb_bufs_o;
    
    reg [WIDTH-1:0] tb_data_1 [0:14] = '{ 
        100, 101, 102, 103, 101, 104, 105, 106, 107, 108, 109, 110, 110, 111, 112
    };
    reg [WIDTH-1:0] tb_expected_bufs [0:14][BUF_SIZE] = '{
        '{ 100, 0, 0, 0, 0, 0, 0, 0 },
        '{ 100, 101, 0, 0, 0, 0, 0, 0 },
        '{ 100, 101, 102, 0, 0, 0, 0, 0 },
        '{ 100, 101, 102, 103, 0, 0, 0, 0 },
        '{ 100, 101, 102, 103, 0, 0, 0, 0 },
        '{ 100, 101, 102, 103, 104, 0, 0, 0 },
        '{ 100, 101, 102, 103, 104, 105, 0, 0 },
        '{ 100, 101, 102, 103, 104, 105, 106, 0 },
        '{ 100, 101, 102, 103, 104, 105, 106, 107 },
        '{ 108, 101, 102, 103, 104, 105, 106, 107 },
        '{ 108, 101, 109, 103, 104, 105, 106, 107 },
        '{ 108, 101, 109, 110, 104, 105, 106, 107 },
        '{ 108, 101, 109, 110, 104, 105, 106, 107 },
        '{ 108, 111, 109, 110, 104, 105, 106, 107 },
        '{ 108, 111, 109, 110, 112, 105, 106, 107 }
    };
    
    task test();
        begin
            test_calls = test_calls + 1;
            for (t = 0; t < BUF_SIZE; t = t + 1) begin
                if (tb_bufs_o[t] !== bufs_o[t]) begin
                    $display("[T=%0t, call=%0d] Incorrect! expected[%0d]=%0d, actual[%0d]=%0d", 
                        $realtime, test_calls, t, tb_bufs_o[t], t, bufs_o[t]);
                    passed = 1'b0;
                end
            end
        end
    endtask
          
    always #1 clk = ~clk;
    
    initial begin
    
        // test data values
        tb_data_1[0] = 32'd100;
            
        clk <= 0;
        rst <= 0;
        
        set_i <= 0;
        val_i <= 0;
        
        // test value is not set
        test();
        
       repeat (20) @(posedge clk);
        
        // test in reset state
        @(negedge clk) begin 
            rst <= 1;
            for (i = 0; i < BUF_SIZE; i = i + 1) tb_bufs_o[i] <= 0;
        end
        
        repeat (20) @(negedge clk) begin
            test();
        end
        
        // test enable without reset
        @(negedge clk) begin
            rst <= 0;
        end
        
        // test when i_set is not set than nothing changes
        // tested i_set in buffer_lru (here just testing algorithm of inserting values)
        repeat(20) @(posedge clk) test();
        
 
        for (i = 0; i < 15; i = i + 1) begin
        
            repeat (5) @(posedge clk);
            
            set_i <= 1;
            val_i <= tb_data_1[i];
           
            
            repeat (5) @(posedge clk);
            
            set_i = 0;
            
            for (j = 0; j < BUF_SIZE; j = j + 1) begin
                tb_bufs_o[j] = tb_expected_bufs[i][j];
            end
            
            test();
        end
        
        repeat (10) @(posedge clk);
        
        // reset
        @(negedge clk) begin 
            rst <= 1;
            for (i = 0; i < BUF_SIZE; i = i + 1) tb_bufs_o[i] <= 0;
        end
        
        repeat (10) @(negedge clk);
        
        rst <= 0;
          
        @(posedge clk) test();
        
        // test same again
        for (i = 0; i < 15; i = i + 1) begin
        
            repeat (5) @(posedge clk);
            
            set_i <= 1;
            val_i <= tb_data_1[i];
           
            
            repeat (5) @(posedge clk);
            
            set_i = 0;
            
            for (j = 0; j < BUF_SIZE; j = j + 1) begin
                tb_bufs_o[j] = tb_expected_bufs[i][j];
            end
            
            test();
        end
        
        repeat (10) @(posedge clk);
        
        // display test message    
        if (passed == 1'b1)
            $display("[T=%0t] All tests passed.", $realtime);
        else
            $display("[T=%0t] Tests failed.", $realtime);
        
        #10 $finish;
    end
endmodule