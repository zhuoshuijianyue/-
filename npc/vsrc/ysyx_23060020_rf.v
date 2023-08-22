module ysyx_23060020_rf (
    input clk,rfwen,
    input[31:0] w1d,
    input[4:0] r1a,r2a,w1a,
	output 	[31:0] r1d,r2d);
    reg[31:0] regs[31:0]/* verilator public_flat */;

    always @(posedge clk ) begin
        if(rfwen) regs[w1a]<=w1d;
    end
    assign r1d=(r1a==5'd0)?32'd0:regs[r1a];
    assign r2d=(r2a==5'd0)?32'd0:regs[r2a];

endmodule
