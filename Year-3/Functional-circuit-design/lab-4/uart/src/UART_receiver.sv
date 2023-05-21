// by Sergey Vasiliev

module UART_receiver
#(
  parameter CLK_FREQ_MHZ    = 100_000_000,
  parameter UART_SPEED_BAUD = 9600
) (
  input logic clk,
  input logic rst_n,

  input logic TX_i,

  output logic data_valid_o,
  output logic [7:0] data_o

);

  localparam BIT_TIME           = CLK_FREQ_MHZ / UART_SPEED_BAUD;
  localparam BIT_TIME_CNT_WIDTH = $clog2(BIT_TIME);

  localparam STATE_IDLE     = 1'b0;
  localparam STATE_RECEIVE  = 1'b1;

  logic                          state_ff;
  logic                          state_next;

  logic                    [3:0] bit_counter_ff;
  logic                    [3:0] bit_counter_next;
  logic                          bit_counter_done;

  logic [BIT_TIME_CNT_WIDTH-1:0] bit_time_counter_ff;
  logic [BIT_TIME_CNT_WIDTH-1:0] bit_time_counter_next;

  logic                    [8:0] uart_buf_ff;
  logic                    [8:0] uart_buf_next;
  logic                          uart_buf_en;


  // Main FSM

  assign state_next = ((state_ff == STATE_IDLE) & (TX_i == 1'b0)          & STATE_RECEIVE)
                    | ((state_ff == STATE_IDLE) & (TX_i == 1'b1)          & STATE_IDLE)
                    | ((state_ff == STATE_RECEIVE) & (bit_counter_done)  & STATE_IDLE)
                    | ((state_ff == STATE_RECEIVE) & (~bit_counter_done) & STATE_RECEIVE);

  always_ff @(posedge clk, negedge rst_n) begin
    if (~rst_n) begin
      state_ff <= STATE_IDLE;
    end else begin
      state_ff <= state_next;
    end
  end

  // Bit time counter logic

  assign bit_time_counter_next  = ((state_ff == STATE_IDLE) | (bit_time_counter_ff == BIT_TIME)) ? 'b0 : bit_time_counter_ff + BIT_TIME_CNT_WIDTH'(1'b1);

  always_ff @(posedge clk, negedge rst_n) begin
    if (~rst_n) begin
      bit_time_counter_ff <= 'b0;
    end else begin
      bit_time_counter_ff <= bit_time_counter_next;
    end
  end

  // Bit counter

  assign bit_counter_en   = (bit_time_counter_ff == BIT_TIME) | (state_ff == STATE_IDLE);
  assign bit_counter_next = (state_ff == STATE_IDLE) ? 'b0 : bit_counter_ff + 4'b0001;
  assign bit_counter_done = (bit_counter_ff == 4'b1001);

  always_ff @(posedge clk, negedge rst_n) begin
    if (~rst_n) begin
      bit_counter_ff <= 'b0;
    end else if (bit_counter_en) begin
      bit_counter_ff <= bit_counter_next;
    end
  end

  // UART buffer logic

  assign uart_buf_en   = (bit_time_counter_ff == BIT_TIME);
  assign uart_buf_next = {uart_buf_ff[7:0], TX_i};

  always_ff @(posedge clk, negedge rst_n) begin
    if (~rst_n) begin
      uart_buf_ff <= 'b0;
    end else if (uart_buf_en) begin
      uart_buf_ff <= uart_buf_next;
    end
  end

  // Output logic

  assign data_valid_o = bit_counter_done;
  assign data_o = uart_buf_ff[8:1];

endmodule
