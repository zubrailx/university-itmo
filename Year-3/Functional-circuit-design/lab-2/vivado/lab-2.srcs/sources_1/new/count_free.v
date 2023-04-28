`timescale 1ns / 1ps

// new updated shifter has consequent input and parallel output
module shift_left_upd
    #(parameter WIDTH = 32)
   (input                  clk,
    input                  en,
    input                  rst,
    input                  in,
    output reg[WIDTH-1:0]  out);
    
    always @(posedge clk, posedge rst) begin
        if (rst) begin
            out <= 0;
        end else if (en) begin
            out <= {out[WIDTH-2:0], in};
        end
    end
endmodule

// updated counter with loading and output carry flag
module counter_upd
    #(parameter WIDTH = 32)
   (input                 clk,
    input                 en,
    input                 rst,
    input                 l,
    input  [WIDTH-1:0]    in,
    output wire           out);
    
    reg [WIDTH:0] data;
 
    always @(posedge clk, posedge rst) begin
        if (rst) begin
            data <= 0;
        end else if (l) begin
            data <= in;
        end else if (en) begin
            data <= data + 1;
        end
    end
    
    assign out = data[WIDTH];
    
endmodule

// count free module
module count_free
    #(parameter WIDTH = 32,
                FREQ_DIV = 5)
    (   input clk,
        input en,
        input rst,
        input start_req_i,
        input start_data_i,
        input ready_i,
        output wire result_rsp_o,
        output reg busy_o
    );
    
    localparam [1:0]
        S_UNDF= 3'b00,
        S_WAIT = 3'b01,
        S_READ = 3'b10,
        S_RUN = 3'b11;
    
    reg [2:0] state;
    
    wire fd_out;
    
    freq_div fd(
        .clk(clk),
        .en(en),
        .rst(rst),
        .out(fd_out)
    );
    
    reg slu_rst;
    reg slu_en;
    wire[WIDTH-1:0] slu_out;
    
    shift_left_upd slu(
        .clk(fd_out),
        .en(slu_en),
        .rst(slu_rst),
        .in(start_data_i),
        .out(slu_out)
    );
    
    reg cu_en;
    reg cu_rst;
    reg cu_l;
    reg[WIDTH-1:0] cu_in;
    wire cu_out;
    
    counter_upd cu(
        .clk(fd_out),
        .en(cu_en),
        .rst(cu_rst),
        .l(cu_l),
        .in(cu_in),
        .out(cu_out)
    );
    
    always @(posedge fd_out, posedge rst) begin
        if (rst) begin  
            slu_en <= 1;
            
            cu_rst <= 1;
            cu_l <= 0;
            cu_en <= 0;
            
            busy_o <= 0;
            state <= S_WAIT;
        end
        if (en) begin
            case (state)
                S_WAIT: begin
                    cu_rst <= 0;
                    
                    if (start_req_i) begin
                        cu_l <= 1; // start loading value from slu
                        
                        busy_o <= 1;
                        state <= S_READ;
                    end
                end
                S_READ: begin
                    if (!start_req_i) begin                        
                        slu_en <= 0;
                        
                        cu_en <= 1; // start counting on next tick
                        cu_l <= 0; // stop loading value from slu   
                        
                        state <= S_RUN;
                    end
                end
                S_RUN: begin
                    if (cu_out) begin
                        cu_en <= 0;
                        
                        if (ready_i) begin
                            cu_rst <= 1;
                            
                            busy_o <= 0;
                            state <= S_WAIT;
                        end
                    end
                end
            endcase
        end
    end
    
    assign result_rsp_o = cu_out;
    
    // combinational logic
    always @(*) begin
        slu_rst = rst;
        cu_in = ~slu_out + 1; // dopcode
        
        case (state)
            S_WAIT: begin
            end
            S_READ: begin
            end
            S_RUN: begin
            end
        endcase
    end
endmodule
