import "DPI-C"  function  void pmem_read(
  input int mem_add,output int mem_data);
import "DPI-C"  function void pmem_write(
  input int mem_add, input int mem_data);
    /* verilator lint_off WIDTHTRUNC */
module ysyx_23060020_mem (
    input memvalid,wen,
    input[31:0] mem_add,
    input[31:0] mem_data,
    output reg[31:0] mem_outdata
);
    always @(*) begin
      if(memvalid) begin
        pmem_read(mem_add,mem_outdata);
         if(wen) begin
          pmem_write(mem_add,mem_data);
         end
      end
        else  mem_outdata=32'b0;
    end
endmodule

