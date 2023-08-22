module ysyx_23060020_top (
  input clk,rst,
  input[31:0] instw,
  output[31:0] pc
);

  wire pccontr;
  wire rfwen;
  wire[31:0] imm;
  wire[4:0]  w1a,r1a,r2a;
  wire[31:0] w1d,r1d,r2d;
  wire[31:0] aluout,alua,alub;
  wire alubmux,aluamux;
  assign r1a=instw[19:15];
  assign r2a=instw[24:20];
  assign w1a=instw[11:7];

  
  assign w1d=aluout;
  assign alua=aluamux?pc:r1d;
  assign alub=alubmux?imm:r2d;

ysyx_23060020_contr u_ysyx_23060020_contr(
    .instw   ( instw   ),
    .pccontr ( pccontr ),
    .rfwen   ( rfwen   ),
    .imm     ( imm     ),
    .alubmux (alubmux ),
    .aluamux (aluamux )
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
    .clk     ( clk     ),
    .rst     ( rst     ),
    .imm     ( imm     ),
    .pccontr ( pccontr ),
    .pc   ( pc  )
);


always @(posedge clk ) begin
      if(instw==32'h00100073) ebreak();
    end

endmodule 

