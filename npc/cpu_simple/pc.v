module pc (
	input	 clk, rst,
	output 	[31:0] pcout);

	Reg #(32, 32'h80000000) pc_reg (clk, rst, pcout+4, pcout, 1'b1);

endmodule
