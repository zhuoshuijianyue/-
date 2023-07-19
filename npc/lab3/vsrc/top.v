module top (
    input clk,rst,
    input[3:0] alua,alub,
    input[2:0] aluop,
    output[3:0] result,
    output overflow
);
    wire[3:0] alu_add,alu_sub,alubnot_temp;
    wire[3:0] alu_not,alu_and,alu_or,alu_xor,alu_less2one,alu_equa;
  
    assign alubnot_temp=~alub;
    assign alu_add=alua+alub;
    assign alu_sub=alua+alubnot_temp+4'd1;
    assign alu_not=~alua;
    assign alu_and=alua&alub;
    assign alu_or=alua|alub;
    assign alu_xor=alua^alub;
    assign alu_less2one=alu_sub[3]^overflow?4'd1:4'd0;;
    assign alu_equa=(|alu_sub[3:0])?4'd0:4'd1;
    assign overflow=(alua[3]==alub[3]  && alu_add[3]!=alua[3] &&  aluop==3'b000)||
                    (alua[3]==alubnot_temp[3] &&  alu_sub[3]!=alua[3] &&  aluop==3'b001);

      //nothing change

    MuxKey #(8, 3, 4) u_mux(
  result,aluop,{
    3'b000,alu_add,
    3'b001,alu_sub,
    3'b010,alu_not,
    3'b011,alu_and,
    3'b100,alu_or,
    3'b101,alu_xor,
    3'b110,alu_less2one,
    3'b111,alu_equa
  }
);

endmodule
