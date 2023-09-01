/*import "DPI-C"  function  void pmem_read(
  input unsigned mem_add,output unsigned mem_data);
import "DPI-C"  function void pmem_write(
  input unsigned mem_add, input unsigned mem_data);*/

  /* verilator lint_off WIDTHTRUNC */
module ysyx_23060020_top (
  input clk,rst,
  input[31:0] instw,
  output[31:0] pc
);

  wire jump_addsel;
  wire[1:0] rfwmux;
  wire rfwen,jump_bool,wen;
  wire memvalid/* verilator public_flat */;
  wire[31:0] imm;
  wire[4:0]  w1a,r1a,r2a;
  wire[31:0] w1d,r1d,r2d;
  wire[31:0] aluout,alua,alub;
  wire[31:0] mem_outdata;
  //wire[31:0] mem_data;
  wire[31:0] mem_add/* verilator public_flat */;
  wire[31:0] jump_add;
  wire alubmux,aluamux;
  assign r1a=instw[19:15];
  assign r2a=instw[24:20];
  assign w1a=instw[11:7];

    assign w1d= (rfwmux==2'b00)?(aluout):(
                (rfwmux==2'b01)?(imm):(
                (rfwmux==2'b10)?(mem_outdata):(
                (rfwmux==2'b11)?(pc+4):(32'd0)
                )

    ));

  assign alua=aluamux?pc:r1d;
  assign alub=alubmux?imm:r2d;
  assign jump_add=jump_bool?(jump_addsel?((r1d+imm)&{{31{1'b1}},1'b0}):pc+imm):32'd0;
  //assign mem_add=aluout;
  //assign mem_data=r2d;

ysyx_23060020_contr u_ysyx_23060020_contr(
    .instw       ( instw       ),
    .jump_addsel ( jump_addsel ),
    .rfwen       ( rfwen       ),
    .memvalid    ( memvalid         ),
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
    .memvalid      ( memvalid      ),
    .wen      ( wen      ),
    .mem_add  ( aluout  ),
    .mem_data ( r2d ),
    .mem_outdata  ( mem_outdata  )
);

/*always @(*) begin
      if(memvalid) begin
        pmem_read(mem_add,mem_outdata);
         if(wen) begin
          pmem_write(mem_add,mem_data);
         end
      end
        else  mem_outdata=32'd0;
    end*/



always @(posedge clk ) begin
      if(instw==32'h00100073) ebreak();
    end

endmodule 

