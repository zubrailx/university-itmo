`timescale 1ns / 1ps

module amplituder_7bit
(
    input M_CLK,
    input rst_i,
    input M_DATA,
    output [8:0] amplitude_o // [0-128]
);

    reg [6:0] cnt_ff;
    wire [6:0] cnt_next;
    
    assign cnt_next = (cnt_ff == 127) ? {7'b0} : cnt_ff + 1'b1;
    
    always @(posedge M_CLK) begin
        if (rst_i) begin
            cnt_ff <= 0;   
        end else begin
            cnt_ff <= cnt_next;
        end
    end

    reg [127:0] window;
    reg [8:0] amplitude;
    reg prev_bit;
    
    always @(posedge M_CLK) begin
        if (rst_i) begin
            window <= 0;
            amplitude <= 0;
            prev_bit <= 0;
        end else begin
            prev_bit <= window[cnt_next];
            // overwrite prev_bit and update
            amplitude <= (amplitude - prev_bit + M_DATA);
            window[cnt_ff] <= M_DATA;
        end
    end

    assign amplitude_o = amplitude;
    
endmodule

module amp2off_7bit(
    input [8:0] amplitude_i,
    output [7:0] amp_off_abs_o
);

    reg [7:0] amp_off_abs;
    
    always @(*) begin
        if (amplitude_i >= 64) begin
            amp_off_abs = amplitude_i - 64;
        end else begin
            amp_off_abs = 64 - amplitude_i;
        end
    end
    
    assign amp_off_abs_o = amp_off_abs;
    
endmodule