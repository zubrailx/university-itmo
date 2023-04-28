`timescale 1ns / 1ps

module count_free_tb;
    
    reg clk;
    reg en;
    reg rst;
    reg start_req_i;
    reg start_data_i;
    reg ready_i;
    
    wire result_rsp_o;
    wire busy_o;
    
    count_free cf(
        .clk(clk),
        .en(en),
        .rst(rst),
        .start_req_i(start_req_i),
        .start_data_i(start_data_i),
        .ready_i(ready_i),
        .result_rsp_o(result_rsp_o),
        .busy_o(busy_o)
    );
    
    reg[31:0] tb_result_rsp_o;
    reg       tb_busy_o;
    
    reg passed = 1'b1;
    integer test_calls = 0;
    
    integer i;
    
    reg[31:0] tb_data_irev_1 = 6'b11; // so actual value will be 101011
    reg[31:0] tb_data_tmprev;
    
    task test;
        begin
            test_calls = test_calls + 1;
            if (tb_result_rsp_o !== result_rsp_o) begin
                $display("[T=%0t, call=%0d] Incorrect! expected_rsp=%0d, actual=%0d", 
                    $realtime, test_calls, tb_result_rsp_o, result_rsp_o);
                passed = 1'b0;
            end
            if (tb_busy_o !== busy_o) begin
                $display("[T=%0t, call=%0d] Incorrect! expected_busy=%b, actual=%b", 
                    $realtime, test_calls, tb_busy_o, busy_o);
                passed = 1'b0;
            end
        end
    endtask
    
    task fsm_task(input reg[31:0] a, b, output reg[35:0] res);
        begin
            res = ((a + b)*4 + b)/2 + (b/2 + a*4);
        end
    endtask
            
    always #10 clk = ~clk;
    
    initial begin
    
//        $monitor("T=%0t: state=%b, reg1=%0d, reg2=%0d, reg3=%00d, reg4=%0d, rdy=%b, out=%0d", 
//            $time, fsm.state, fsm.reg1, fsm.reg2, fsm.reg3, fsm.reg4, fsm.rdy, fsm.out);
            
        clk <= 0;
        rst <= 0;
        
        en <= 0;
        start_req_i <= 0;
        start_data_i <= 0;
        ready_i <= 0;
        
        // test in reset state
        @(negedge clk) begin 
            rst <= 1;
            tb_busy_o <= 0;
            tb_result_rsp_o <= 0;
        end
        
        repeat (20) @(posedge clk) begin
            test();
        end
        
        // test enable without reset
        @(negedge clk) begin
            rst <= 0;
            en <= 1;
        end
        
        repeat (20) @(posedge clk);
        
        // run whole states
        @(negedge clk) begin
            start_req_i <= 1;
        end
        
        for (i = 0; i < 2; i = i + 1) begin
            @(posedge clk) begin
                start_data_i <= tb_data_irev_1[0];
                tb_data_irev_1 <= tb_data_irev_1 >> 1;
            end
            repeat(9) @(posedge clk);
            
            tb_busy_o = 1;
            test();
        end
        
        @(posedge clk) begin
            start_req_i <= 0;
            start_data_i <= 0;
        end
        
        repeat (70) @(posedge clk);
        
        tb_busy_o = 1;
        tb_result_rsp_o = 1;
        
        test();
        
        @(posedge clk) begin
            ready_i <= 1;
        end
        
        repeat (10) @(posedge clk);
        
        tb_busy_o = 0;
        tb_result_rsp_o = 0;
        
        test();
        
        // reset and test
        @(negedge clk) rst <= 1;
        @(posedge clk) rst <= 0;
        
        repeat(20) begin
            @(posedge clk) test();
        end
        
        // display test message    
        if (passed == 1'b1)
            $display("[T=%0t] All tests passed.", $realtime);
        else
            $display("[T=%0t] Tests failed.", $realtime);
        
            #10 $finish;
    end
    

endmodule
