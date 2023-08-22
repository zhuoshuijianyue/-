module ysyx_23060020_pc (
    input clk,rst,
    input[31:0] imm,
    input pccontr,
    output[31:0] pc
);
    wire[31:0] pcout_temp;
    assign pcout_temp=pccontr?(imm+pc):(pc+4);
    ysyx_23060020_Reg #(32, 32'h80000000) pc_reg (clk, rst, pcout_temp, pc, 1'b1);
endmodule
