import "DPI-C"  function  void pmem_read(
  input int mem_add,output int mem_data);
import "DPI-C"  function void pmem_write(
  input int mem_add, input int mem_data, input byte wmask_temp);

module ysyx_23060020_mem (
    input memvalid,wen,
    input[3:0] wmask,
    input[31:0] mem_add,
    input[31:0] mem_data,
    output reg[31:0] mem_outdata
);

    wire[7:0] wmask_temp;

    assign wmask_temp={4'd0,wmask};


  always @(*) begin
     if(memvalid) begin
        pmem_read(mem_add,mem_outdata);
         if(wen) begin
          pmem_write(mem_add,mem_data,wmask_temp);
         end
      end
        else  begin mem_outdata=32'd0; end
  end

endmodule

