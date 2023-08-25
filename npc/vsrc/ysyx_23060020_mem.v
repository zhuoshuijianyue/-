import "DPI-C" function int pmem_read(
  input int raddr);
import "DPI-C" function void pmem_write(
  input int waddr, input int wdata);
module ysyx_23060020_mem (
    input ren,wen,
    input[31:0] mem_add,
    input[31:0] mem_data,
    output reg[31:0] mem_outdata
);
    always @(*) begin
      if(ren) begin
        mem_outdata=pmem_read(mem_add);
         if(wen) begin
          pmem_write(mem_add,mem_data);
         end
      end
        else  mem_outdata=32'd0;
    end
endmodule

