module rf (
	input[4:0]	 r1a,r2a,
	output[31:0] r1d,r2d
	);

	Reg #(32, 32'h80000000) pc_reg (clk, rst, pcout+4, pcout, 1'b1);

endmodule


