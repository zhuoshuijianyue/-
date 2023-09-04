/*import "DPI-C"  function  void pmem_read(
  input unsigned mem_add,output unsigned mem_data);
import "DPI-C"  function void pmem_write(
  input unsigned mem_add, input unsigned mem_data);*/

  /* verilator lint_off WIDTHTRUNC */
  /* verilator lint_off UNUSEDSIGNAL */
module ysyx_23060020_top (
  input clk,rst,
  input[31:0] instw,
  output[31:0] pc
);

  wire jump_addsel;
  wire[1:0] rfwmux;
  wire rfwen,jump_bool,wen;
  wire memvalid,zero;
  wire[31:0] imm;
  wire[4:0]  w1a,r1a,r2a;
  wire[3:0] alusel;
  wire[3:0] wmask;
  wire[31:0] w1d,r1d,r2d;
  wire[31:0] aluout,alua,alub;
  wire[31:0] mem_outdata,mem_to_reg,full_mask;

  wire[31:0] jump_add;
  wire alubmux,aluamux;
  wire load_signed_extends;
  
  assign full_mask={{8{wmask[3]}},{8{wmask[2]}},{8{wmask[1]}},{8{wmask[0]}}};
  assign r1a=instw[19:15];
  assign r2a=instw[24:20];
  assign w1a=instw[11:7];
  assign mem_to_reg=load_signed_extends?((wmask==4'd3)?(
    {{16{mem_outdata[15]}},mem_outdata[15:0]}):({{24{mem_outdata[7]}},mem_outdata[7:0]})
          ):(
          (mem_outdata)&full_mask);


    assign w1d= (rfwmux==2'b00)?(aluout):(
                (rfwmux==2'b01)?(imm):(
                (rfwmux==2'b10)?(mem_to_reg):(
                (rfwmux==2'b11)?(pc+4):(32'd0)
                )

    ));

  assign alua=aluamux?pc:r1d;
  assign alub=alubmux?imm:r2d;
  assign jump_add=jump_bool?(jump_addsel?((r1d+imm)&{{31{1'b1}},1'b0}):pc+imm):32'd0;

ysyx_23060020_contr u_ysyx_23060020_contr(
    .instw       ( instw       ),
    .aluout      (aluout       ),
    .jump_addsel ( jump_addsel ),
    .rfwen       ( rfwen       ),
    .memvalid    ( memvalid    ),
    .wen         ( wen         ),
    .rfwmux      ( rfwmux      ),
    .imm         ( imm         ),
    .alubmux     ( alubmux     ),
    .aluamux     ( aluamux     ),
    .jump_bool   ( jump_bool   ),
    .alusel      (alusel       ),
    .zero        (zero         ),
    .wmask       (wmask        ),
    .load_signed_extends(load_signed_extends)
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
    .alusel(alusel),
    .aluout  ( aluout  ),
    .zero (zero)
);




ysyx_23060020_pc u_ysyx_23060020_pc(
    .clk       ( clk       ),
    .rst       ( rst       ),
    .jump_add  ( jump_add  ),
    .jump_bool ( jump_bool ),
    .pc        ( pc        )
);

ysyx_23060020_mem u_ysyx_23060020_mem(
    .memvalid       ( memvalid      ),
    .wen            ( wen      ),
    .wmask          (wmask    ),
    .mem_add        ( aluout  ),
    .mem_data       ( r2d ),
    .mem_outdata    ( mem_outdata  )
);



always @(posedge clk ) begin
      if(instw==32'h00100073) ebreak();
    end

endmodule 

