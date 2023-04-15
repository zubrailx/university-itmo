`timescale 1ns / 1ps

module clap_detector
(
    input clk_i,
    input rst_i,
    // ports to micro
    input M_DATA,
    output M_LRSEL,
    inout M_CLK,
    // clap
    output clap_pulse_o
);
  
  mic_configurator mc(
    .clk_i(clk_i),
    .rst_i(rst_i),
    .M_CLK(M_CLK),
    .M_LRSEL(M_LRSEL)
  );
  
  clap_detector_7bit cd(
    .M_CLK(M_CLK),
    .rst_i(rst_i),
    .M_DATA(M_DATA),
    .clap_pulse_o(clap_pulse_o)
  );

endmodule


module mic_configurator
    // 12Hz (max_freq) * 128 (precision) * 2 (>= 2 - Nyquist) = 3072000 Hz
    // 100MHz / 3072000Hz <= 32 - less than 3.3MHz
    #(parameter FREQ_DIV = 32)
(
    input clk_i,
    input rst_i,
    output M_CLK,
    output M_LRSEL
);

    freq_div #(.FREQ_DIV(FREQ_DIV)) fd(
        .clk_i(clk_i),
        .rst_i(rst_i),
        .fdclk_o(M_CLK)
    );
    
    assign M_LRSEL = 1'b0;

endmodule


module clap_detector_7bit
    #(parameter  CLAP_REPEAT_MIN = 300_000,
      parameter CLAP_AMP_THR = 12)
(
    input M_CLK,
    input rst_i,
    input M_DATA,
    output clap_pulse_o
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
    
    // calculate amplitude [0-128] and set window
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

    // calculate amplitude as offset from center [0-64]
    reg [7:0] amp_off_abs;
    
    always @(posedge M_CLK) begin
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
            on_clap <= (amp_off_abs > CLAP_AMP_THR) ? 1'b1 : 1'b0;
        end
    end
        
    // if clap front
    reg clap_q;
    wire clap_posedge;
    
    always @(posedge M_CLK) clap_q <= on_clap;
    assign clap_posedge = (on_clap != clap_q) && on_clap;
    
    // detect claps on fronts that exceed CLAP_REPEAT_MIN
    reg [$clog2(CLAP_REPEAT_MIN + 1)-1:0] clap_cnt;
    
    reg clap_pulse;
    
    always @(posedge M_CLK) begin
        if (clap_posedge && clap_cnt == CLAP_REPEAT_MIN) begin
            clap_pulse <= 1;
            clap_cnt <= 0;
        end else begin
            if (clap_cnt != CLAP_REPEAT_MIN) begin
                clap_cnt <= clap_cnt + 1;
            end
            clap_pulse <= 0;
        end
    end
    
    // set ouput signals   
    assign clap_pulse_o = clap_pulse;
    
endmodule