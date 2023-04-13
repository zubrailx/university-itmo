`timescale 1ns / 1ps

module button_debouncer (
    input clk_i,
    input btnu_i, btnd_i, btnl_i, btnr_i, btnc_i,
    output btnu_o, btnd_o, btnl_o, btnr_o, btnc_o
);
    
    debouncer deb_u (
        .clk_i(clk_i),
        .btn_i(btnu_i),
        .btn_o(btnu_o)
    );
    debouncer deb_d (
        .clk_i(clk_i),
        .btn_i(btnd_i),
        .btn_o(btnd_o)
    );
    debouncer deb_l (
        .clk_i(clk_i),
        .btn_i(btnl_i),
        .btn_o(btnl_o)
    );
    debouncer deb_r (
        .clk_i(clk_i),
        .btn_i(btnr_i),
        .btn_o(btnr_o)
    );
    debouncer deb_c (
        .clk_i(clk_i),
        .btn_i(btnc_i),
        .btn_o(btnc_o)
    );
    
endmodule


module debouncer
    #(parameter WAIT_CLOCKS = 1_000_000)
    (
        input clk_i,
        input btn_i,
        output reg btn_o
    );
    
    reg [$clog2(WAIT_CLOCKS)-1:0] cnt;
    
    always @(posedge clk_i) begin
        if (btn_i) begin
            if (cnt == WAIT_CLOCKS) begin
                btn_o <= 1;
            end else begin
                cnt <= cnt + 1;
            end
        end else begin
            cnt <= 0;
            btn_o <= 0;
        end
    end
endmodule
