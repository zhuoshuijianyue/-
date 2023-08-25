module ysyx_23060020_top (
  input clk,rst,
  input[31:0] instw,
  output[31:0] pc
);

  wire jump_addsel;
  wire[1:0] rfwmux;
  wire rfwen,jump_bool,ren,wen;
  wire[31:0] imm;
  wire[4:0]  w1a,r1a,r2a;
  wire[31:0] w1d,r1d,r2d;
  wire[31:0] aluout,alua,alub,mem_outdata;
  wire[31:0] jump_add;
  wire alubmux,aluamux;
  assign r1a=instw[19:15];
  assign r2a=instw[24:20];
  assign w1a=instw[11:7];

  
  //assign w1d=rfwmux?imm:aluout;
  ysyx_23060020_MuxKeyInternal #(4, 2, 32, 0) i0 (w1d, rfwmux, 32'd0, 
          {2'b00,aluout,
          2'b01,imm,
          2'b10,mem_outdata,
          2'b11,pc+4});

  assign alua=aluamux?pc:r1d;
  assign alub=alubmux?imm:r2d;
  assign jump_add=jump_bool?(jump_addsel?((r1d+imm)&{{31{1'b1}},1'b0}):pc+imm):32'd0;

ysyx_23060020_contr u_ysyx_23060020_contr(
    .instw       ( instw       ),
    .jump_addsel ( jump_addsel ),
    .rfwen       ( rfwen       ),
    .ren         ( ren         ),
    .wen         ( wen         ),
    .rfwmux      ( rfwmux      ),
    .imm         ( imm         ),
    .alubmux     ( alubmux     ),
    .aluamux     ( aluamux     ),
    .jump_bool   ( jump_bool   )
);


ysyx_23060020_rf u_ysyx_23060020_rf(
    .clk   ( clk   ),
    .rfwen ( rfwen ),
    .w1d   ( w1d   ),
    .r1a   ( r1a   ),
    .r2a   ( r2a   ),
    .w1a   ( w1a   ),
    .r1d   ( r1d   ),
    .r2d   ( r2d   )
);

ysyx_23060020_alu u_ysyx_23060020_alu(
    .alua ( alua ),
    .alub ( alub ),
    .aluout  ( aluout  )
);




ysyx_23060020_pc u_ysyx_23060020_pc(
    .clk       ( clk       ),
    .rst       ( rst       ),
    .jump_add  ( jump_add  ),
    .jump_bool ( jump_bool ),
    .pc        ( pc        )
);

ysyx_23060020_mem u_ysyx_23060020_mem(
    .ren      ( ren      ),
    .wen      ( wen      ),
    .mem_add  ( aluout  ),
    .mem_data ( r2d ),
    .mem_outdata  ( mem_outdata  )
);



always @(posedge clk ) begin
      if(instw==32'h00100073) ebreak();
    end

endmodule 

