import "DPI-C" function int add (input int a, input int b);
module top (
    input a,b,clock,rst,
    output[1:0] out
);
    always @(posedge clk ) begin
        out=add(a,b);
    end
endmodule
