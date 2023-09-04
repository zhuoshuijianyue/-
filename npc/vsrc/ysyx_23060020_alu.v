module ysyx_23060020_alu (
	input	[31:0] alua,alub, 
	input [3:0] alusel,
	output 	[31:0] aluout,
	output zero
    );
	wire[31:0] result_add,result_sub,result_lessto1,result_lessto1_signed,result_or;
	wire[31:0] result_left_l,result_right_l,result_right_a,result_xor,result_and;

	assign	result_add=alua+alub;
	assign 	result_sub=alua-alub;
	assign  result_lessto1=(alua<alub)?1:0;
	assign  result_lessto1_signed=(($signed(alua))<($signed(alub)))?1:0;
	assign 	result_left_l=alua<<(alub%32);
	assign 	result_right_l=alua>>(alub%32);
	assign 	result_right_a=($signed(alua))>>>(alub%32);
	assign 	result_xor=alua^alub;
	assign  result_and=alua&alub;
	assign 	result_or=alua|alub;
	ysyx_23060020_MuxKeyInternal #(10, 4, 32, 1) i0 (aluout, alusel, 32'd0, {
		4'b0001,result_add,
		4'b0010,result_sub,
		4'b0011,result_and,
		4'b0100,result_or,
		4'b0101,result_xor,
		4'b0110,result_left_l,
		4'b0111,result_right_l,
		4'b1000,result_right_a,
		4'b1001,result_lessto1,
		4'b1010,result_lessto1_signed
	});
	assign	zero=(alusel==4'b0010)&&(!(|result_sub));
endmodule
