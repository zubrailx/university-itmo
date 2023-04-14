`timescale 1ns / 1ps

// algorithm to detect whether clap occured
// 7 bits - 128 levels
module clap_detector_7bit
    #(parameter CLAP_REPEAT_MIN = 10_000,
      parameter CLAP_AMP_THR = 16)
(
    input clk_i,
    input rst_i,
    // ports to micro
    input M_DATA,
    output M_LRSEL,
    output M_CLK,
    // clap
    output clap_pulse_o
);
    // configure freq_div
    wire fdclk;
    
    // 12Hz (max_freq) * 128 (precision) * 2 (>= 2 - Nyquist) = 3072000 Hz
    // 100MHz / 3072000Hz <= 32 - less than 3.3MHz
    freq_div #(.FREQ_DIV(32)) fd(
        .clk_i(clk_i),
        .rst_i(rst_i),
        .fdclk_o(fdclk)
    );
    
    // configure mic
    assign M_LRSEL = 1'b0;
    assign M_CLK = fdclk;
    
    // amplitude and window
    reg [6:0] cnt_ff;
    wire [6:0] cnt_next;
    
    // counter logic
    assign cnt_next = (cnt_ff == 127) ? {7'b0} : cnt_ff + 1'b1;
    
    always @(posedge M_CLK) begin
        if (rst_i) begin
            cnt_ff <= 0;   
        end else begin
            cnt_ff <= cnt_next;
        end
    end
    
    // calculate amplitude and set window
    reg [127:0] window;
    // [0-128]
    reg [8:0] amplitude;
    reg prev_bit;
    
    always @(posedge M_CLK) begin
        if (rst_i) begin
            window <= 0;
            amplitude <= 0;
            prev_bit <= 0;
        end else begin
            cnt_ff <= cnt_next;
            prev_bit <= window[cnt_next];
            // overwrite prev_bit and update
            amplitude <= (amplitude - prev_bit + M_DATA);
            window[cnt_ff] <= M_DATA;
        end
    end

    reg [7:0] amp_off_abs;
    integer i;
    
    // calculate amp_off_abs
    always @(posedge M_CLK) begin
        // calculate amplitude from center offset [0-64]
        if (amplitude >= 64) begin
            amp_off_abs <= amplitude - 64;
        end else begin
            amp_off_abs <= 64 - amplitude;
        end
    end
    
    // detect if on clap logic
    reg on_clap;
    
    always @(posedge M_CLK) begin
        if (rst_i) begin
            on_clap <= 0;
        end else begin
            if (amp_off_abs > CLAP_AMP_THR) begin
                on_clap <= 1;
            end
        end
    end
        
    // store if clap front
    reg clap_q;
    always @(posedge M_CLK) clap_q <= on_clap;
    
    wire clap_chg;
    assign clap_chg = (on_clap != clap_q);
    
    // logic to detect two clap fronts and with impulses and exceed CLAP_REPEAT_MIN
    reg [$clog2(CLAP_REPEAT_MIN + 1)-1:0] clap_cnt;
    
    reg clap_pulse;
    
    always @(posedge M_CLK) begin
        // if on clap pulse and clap_cnt_ff >= CLAP_REPEAT_MIN (it stops)
        if (clap_chg && on_clap && clap_cnt == CLAP_REPEAT_MIN) begin
            clap_pulse <= 1;
            clap_cnt <= 0;
        end else begin
            if (clap_cnt != CLAP_REPEAT_MIN) begin
                clap_cnt <= clap_cnt + 1;
            end
            clap_pulse <= 0;
        end
    end
    
    assign clap_pulse_o = clap_pulse;

endmodule


module freq_div
    #(parameter FREQ_DIV = 32)
(
    input clk_i,
    input rst_i,
    output reg fdclk_o
);

    localparam CNT_WIDTH = $clog2(FREQ_DIV);
    
    reg [CNT_WIDTH-1:0] cnt_ff;
    wire [CNT_WIDTH-1:0] cnt_next;
    
    // counter logic
    assign cnt_next = (cnt_ff == FREQ_DIV - 1) ? {CNT_WIDTH{1'b0}} : cnt_ff + 1'b1;
    
    always @(posedge clk_i) begin
        if (rst_i) begin
            cnt_ff <= {CNT_WIDTH{1'b0}};
        end else begin
            cnt_ff <= cnt_next;
        end
    end
    
    // fdclk logic
    always @(posedge clk_i) begin
        if (rst_i) begin
            fdclk_o <= 0;
        end else if (cnt_ff == FREQ_DIV - 1) begin
            fdclk_o <= ~fdclk_o;
        end
    end

endmodule