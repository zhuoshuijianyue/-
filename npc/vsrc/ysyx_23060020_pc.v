module ysyx_23060020_pc (
    input clk,rst,
    input[31:0] jump_add,
    input jump_bool,
    output reg[31:0] pc
);
    always @(posedge clk ) begin
        if(rst) pc<=32'h80000000;
        else if(jump_bool) pc<=jump_add;
        else pc<=pc+4;
    end
    
endmodule
