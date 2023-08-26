/*module ysyx_23060020_rf (
    input clk,rfwen,
    input[31:0] w1d,
    input[4:0] r1a,r2a,w1a,
	output reg[31:0] r1d,r2d);
    reg[31:0] regs[31:0];

    always @(posedge clk ) begin
        if(rfwen&&w1a!=5'd0) regs[w1a]<=w1d;
    end

    always @(posedge clk ) begin
        if(rfwen==1'b1&&w1a==r1a)begin
            r1d<=w1d;
        end
        else r1d<=regs[r1a];
    end

    always @(posedge clk ) begin
        if(rfwen==1'b1&&w1a==r2a)begin
            r2d<=w1d;
        end
        else r2d<=regs[r2a];
    end
    assign regs[0]=32'd0;
endmodule*/

module ysyx_23060020_rf (
    input clk,rfwen,
    input[31:0] w1d,
    input[4:0] r1a,r2a,w1a,
	output[31:0] r1d,r2d);
    reg[31:0] regs[31:0]/* verilator public_flat_rd */;

    always @(posedge clk ) begin
        if(rfwen) begin
        if(w1a!=5'd0) regs[w1a]<=w1d;
        end
    end
    assign r1d=regs[r1a];
    assign r2d=regs[r2a];
endmodule
