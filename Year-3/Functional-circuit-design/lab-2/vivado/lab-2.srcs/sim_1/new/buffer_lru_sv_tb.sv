`timescale 1ns / 1ps

module buffer_lru_sv_tb;

    parameter 
        WIDTH = 32,
        BUF_SIZE = 8;

    reg clk;
    reg en;
    reg rst;
    reg i_set;
    reg [WIDTH-1:0] i_val;
    wire [WIDTH-1:0] o_bufs[BUF_SIZE-1:0];
    
    buffer_lru_sv blru(
        .clk(clk),
        .en(en),
        .rst(rst),
        .i_set(i_set),
        .i_val(i_val),
        .o_bufs(o_bufs)
    );
    
    reg passed = 1'b1;
    integer test_calls = 0;
    
    integer i, j, t;
    
    reg [WIDTH-1:0] tb_o_bufs [BUF_SIZE-1:0];
    
    reg [WIDTH-1:0] tb_data_1 [15] = { 100, 101, 102, 103, 101, 104, 105, 106, 107, 108, 109, 110, 110, 111, 112};
    reg [WIDTH-1:0] tb_expected_bufs [15][BUF_SIZE] = {
        { 100, 0, 0, 0, 0, 0, 0, 0 },
        { 100, 101, 0, 0, 0, 0, 0, 0 },
        { 100, 101, 102, 0, 0, 0, 0, 0 },
        { 100, 101, 102, 103, 0, 0, 0, 0 },
        { 100, 101, 102, 103, 0, 0, 0, 0 },
        { 100, 101, 102, 103, 104, 0, 0, 0 },
        { 100, 101, 102, 103, 104, 105, 0, 0 },
        { 100, 101, 102, 103, 104, 105, 106, 0 },
        { 100, 101, 102, 103, 104, 105, 106, 107 },
        { 108, 101, 102, 103, 104, 105, 106, 107 },
        { 108, 101, 109, 103, 104, 105, 106, 107 },
        { 108, 101, 109, 110, 104, 105, 106, 107 },
        { 108, 101, 109, 110, 104, 105, 106, 107 },
        { 108, 111, 109, 110, 104, 105, 106, 107 },
        { 108, 111, 109, 110, 112, 105, 106, 107 }
    };
    
    task test();
        begin
            test_calls = test_calls + 1;
            for (t = 0; t < BUF_SIZE; t = t + 1) begin
                if (tb_o_bufs[t] !== o_bufs[t]) begin
                    $display("[T=%0t, call=%0d] Incorrect! expected[%0d]=%0d, actual[%0d]=%0d", 
                        $realtime, test_calls, t, tb_o_bufs[t], t, o_bufs[t]);
                    passed = 1'b0;
                end
            end
        end
    endtask
            
    always #10 clk = ~clk;
    
    initial begin
        // test data values
        tb_data_1[0] = 32'd100;
            
        clk <= 0;
        rst <= 0;
        
        en <= 0;
        i_set <= 0;
        i_val <= 0;
        
        // test value is not set
        test();
        
       repeat (20) @(posedge clk);
        
        // test in reset state
        @(negedge clk) begin 
            rst <= 1;
            for (i = 0; i < BUF_SIZE; i = i + 1) tb_o_bufs[i] <= 0;
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
        // tested i_set in buffer_lru (here just testing algorithm of inserting values)
        repeat(20) @(posedge clk) test();
        
 
        for (i = 0; i < 15; i = i + 1) begin
            @(negedge clk) begin
                i_set <= 1;
                i_val <= tb_data_1[i];
            end
            
            repeat (10) @(posedge clk);
            
            for (j = 0; j < BUF_SIZE; j = j + 1) begin
                tb_o_bufs[j] = tb_expected_bufs[i][j];
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
