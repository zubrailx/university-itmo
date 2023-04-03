`timescale 1ns / 1ps

module FSM_4 // moore state machine
    #(parameter WIDTH = 32)
    (   
        input                   clk,
        input wire[WIDTH-1:0]   a,
        input wire[WIDTH-1:0]   b,
        input                   rst,
        output reg[WIDTH-1+4:0] out, // result is up to max(A,B)*16 then 4 bits additional needed
        output reg              rdy
    );
    
    localparam [3:0]
        S0 = 4'b0000, // reg1 = a, reg2 = b
        S1 = 4'b0001, // reg3 = reg1+reg2
        S2 = 4'b0010, // reg4 = reg3*2*2
        S3 = 4'b0011, // reg4 = reg4+reg2
        S4 = 4'b0100, // reg4 = reg4/2, reg1 = reg1*2
        S5 = 4'b0101, // reg2 = reg2/2, reg1 = reg1*2
        S6 = 4'b0110, // reg4 = reg4 + (reg2 + reg1)
        S7 = 4'b0111,
        S8 = 4'b1000,
        S9 = 4'b1001;
            
    reg[3:0] state;
 
    // registers to store intermediate values
    reg[WIDTH-1:0] reg1;
    reg[WIDTH-1:0] reg2;
    reg[WIDTH-1:0] reg3;
    reg[WIDTH-1:0] reg4;
    
    // regs and modules to connect to them with demultiplexor
    reg [WIDTH-1:0] a1_in_lsv;
    reg [WIDTH-1:0] a1_in_rsv;
    wire [WIDTH-1:0] a1_out;
    
    adder a1(
        .in_l(a1_in_lsv),
        .in_r(a1_in_rsv),
        .out(a1_out)
    );
    
    
    reg [WIDTH-1:0] a2_in_lsv;
    reg [WIDTH-1:0] a2_in_rsv;
    wire [WIDTH-1:0] a2_out;
    
    adder a2(
        .in_l(a2_in_lsv),
        .in_r(a2_in_rsv),
        .out(a2_out) 
    );
    
    
    reg [WIDTH-1:0] d1_in;
    wire [WIDTH-1:0] d1_out;
    
    div2 d1(
        .in(d1_in),
        .out(d1_out)
    );
    
    
    reg [WIDTH-1:0] m1_in;
    wire [WIDTH-1:0] m1_out;
    
    mul2 m1(
        .in(m1_in),
        .out(m1_out)
    );
    
    reg cnt_en;
    reg cnt_rst;
    wire[WIDTH-1:0] cnt_out;
    
    // counter for operations
    counter cnt(
        .clk(clk),
        .en(cnt_en),
        .rst(cnt_rst),
        .out(cnt_out)
    );
    
    always @(posedge clk, posedge rst) begin
        if (rst) begin
            state <= 0;
            out <= 0;
            rdy <= 0;
        end
        else begin
            case (state)
                S0: begin
                    reg1 <= a;
                    reg2 <= b;
                    // reset counter and disable it
                    cnt_rst <= 1;
                    cnt_en <= 0;
                    // update state
                    state <= S1;  
                end
                S1: begin
                    
                    reg3 <= a1_out;
                    // enable counter
                    cnt_rst <= 0;
                    cnt_en <= 1;
                    
                    state <= S2;
                end
                S2: begin         
                    if (cnt_out == 2) begin
                        state <= S3;
                        cnt_en <= 0;
                        cnt_rst <= 1;
                    end else begin
                        reg3 <= m1_out;
                    end
                end
                S3: begin
                    reg4 <= a2_out;
                    state <= S4;
                end
                S4: begin
                    reg4 <= d1_out;
                    reg1 <= m1_out;
                    state <= S5;
                end
                S5: begin
                    reg1 <= m1_out;
                    reg2 <= d1_out;
                    state <= S6;
                end
                S6: begin
                    out <= a2_out;
                    rdy <= 1;
                    state <= S0;
                end
            endcase
        end
    end
        
    // connect inputs of adder,mul2,div2
    always @(*) begin
        case (state)
            S1: begin
                a1_in_lsv = reg1;
                a1_in_rsv = reg2;
            end
            S2: begin
                m1_in = reg3;
            end
            S3: begin
                a2_in_lsv = reg3;
                a2_in_rsv = reg2;
            end
            S4: begin
                d1_in = reg4;
                m1_in = reg1;
            end
            S5: begin
                m1_in = reg1;
                d1_in = reg2;
            end
            S6: begin
                a1_in_lsv = reg2;
                a1_in_rsv = reg1;
                a2_in_lsv = reg4;
                a2_in_rsv = a1_out;  
            end
        endcase
    end
    
endmodule
