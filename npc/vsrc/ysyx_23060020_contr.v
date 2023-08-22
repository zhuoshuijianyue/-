import "DPI-C" context function void ebreak();
module ysyx_23060020_contr (
    input[31:0] instw,
    output pccontr,
    output rfwen,
    output[31:0] imm,
    output alubmux,aluamux
);
    wire Itype,Utype;
    wire instw_addi,instw_ebreak;
    wire instw_auipc;

    assign instw_ebreak=    (instw==32'h00100073);
    assign instw_auipc=     (                           instw[6:0]==7'b0010111);
    assign instw_addi=      (instw[14:12]==3'b000&&     instw[6:0]==7'b0010011);

    assign Itype=instw_addi;
    assign Utype=instw_auipc;

    assign pccontr= 1'b0;
    assign rfwen=instw_addi | instw_auipc;
    assign imm= ({32{Itype}}    &   {{20{instw[31]}},instw[31:20]})|
                ({32{Utype}}    &   {{instw[31:12]},{12'd0}});
    assign alubmux=instw_addi   | instw_auipc;
    assign aluamux=instw_auipc;

    always @(*) begin
        if(instw_ebreak) ebreak();
    end
endmodule 

