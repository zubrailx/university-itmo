`timescale 1ns / 1ps

// Для однобитового компаратора
module one_bit_part(
  input a, b,
  output out
);

wire not_a, not_b;
wire v1, v2;
  nand(not_a, a, a);
  nand(not_b, b, b);
  
  nand(v1, b, a);
  nand(v2, not_a, not_b);
 
  nand(out, v1, v2); 
endmodule


module full_comparator4(
  input[3:0] a,
  input[3:0] b,
  output y
);

wire[3:0] bcomp_out;
wire v1, v2;
  one_bit_part obp0(a[0], b[0], bcomp_out[0]);
  one_bit_part obp1(a[1], b[1], bcomp_out[1]);
  one_bit_part obp2(a[2], b[2], bcomp_out[2]);
  one_bit_part obp3(a[3], b[3], bcomp_out[3]);
  
wire not_v1, not_v2; 
  nand(v1, bcomp_out[0], bcomp_out[1]);
  nand(not_v1, v1, v1);
  
  nand(v2, bcomp_out[2], bcomp_out[3]);
  nand(not_v2, v2, v2);
  
wire not_y;
  nand(not_y, not_v1, not_v2);
  nand(y, not_y, not_y);
  
endmodule
