`timescale 1ns / 1ps

module clap_detector
(
    input M_CLK,
    input rst_i,
    input [8:0] amplitude_i,
    output clap_pulse_o
);

  clap_detector_7bit cd(
    .M_CLK(M_CLK),
    .rst_i(rst_i),
    .amplitude_i(amplitude_i),
    .clap_pulse_o(clap_pulse_o)
  );

endmodule


module clap_detector_7bit
    #(parameter  CLAP_REPEAT_MIN = 300_000,
      parameter CLAP_AMP_THR = 16)
(
    input M_CLK,
    input rst_i,
    input [8:0] amplitude_i,
    output clap_pulse_o
);
    
    // calculate amplitude as offset from center [0-64]
    wire [7:0] amp_off_abs;
    
    amp2off_7bit ato7bit(
        .amplitude_i(amplitude_i),
        .amp_off_abs_o(amp_off_abs)
    );
    
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