`timescale 1ns / 1ps

module full_comparator4_tb;

reg [3:0] test_a;
reg [3:0] test_b;
wire test_res;

reg ex_res;

integer i, j;

full_comparator4 comparator(test_a, test_b, test_res);

// not synthesizable
initial begin
  for (i = 0; i < 16; i = i + 1) begin
    test_a = i;
    
    for (j = 0; j < 16; j = j + 1) begin
      test_b = j;
      
      ex_res = (i == j);
      
      #1 // wait for 10 nanoseconds
      
      if (test_res == ex_res) begin
        $display("[CORRECT]: a = %d, b = %d, test_res = %d", test_a, test_b, test_res);
      end else begin
        $display("[INCORRECT]: a = %d, b = %d, test_res = %d, ex_res = %d", test_a, test_b, test_res, ex_res);
      end 
    end
  end
  
  #1 $stop;
  
end

endmodule
