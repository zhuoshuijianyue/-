module ysyx_23060020_alu (
	input	[31:0] alua,alub, 
	output 	[31:0] aluout
    );

	assign aluout=alua+alub;
endmodule
