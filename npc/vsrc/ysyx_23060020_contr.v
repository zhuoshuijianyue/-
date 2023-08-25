import "DPI-C" context function void ebreak();
module ysyx_23060020_contr (
    input[31:0] instw,
    output jump_addsel,
    output rfwen,ren,wen,
    output[1:0] rfwmux,
    output[31:0] imm,
    output alubmux,aluamux,jump_bool
);
    wire Itype,Utype,Jtype,Stype;
    wire instw_addi,instw_ebreak,instw_lui,instw_jal,instw_jalr;
    wire instw_auipc;
    wire instw_sw;

    assign instw_ebreak=    (instw==32'h00100073);
    assign instw_lui=       (                           instw[6:0]==7'b0110111);
    assign instw_auipc=     (                           instw[6:0]==7'b0010111);
    assign instw_jal=       (                           instw[6:0]==7'b1101111);
    assign instw_jalr=      (instw[14:12]==3'b000&&     instw[6:0]==7'b1100111);
    assign instw_addi=      (instw[14:12]==3'b000&&     instw[6:0]==7'b0010011);
    assign instw_sw=        (instw[14:12]==3'b010&&     instw[6:0]==7'b0100011);

    assign Itype=instw_addi     |   instw_jalr;
    assign Utype=instw_auipc    |   instw_lui;
    assign Jtype=instw_jal;
    assign Stype=instw_sw;
    
    
    assign imm= ({32{Itype}}    &   {{20{instw[31]}},instw[31:20]})|
                ({32{Utype}}    &   {{instw[31:12]},{12'd0}})|
                ({32{Jtype}}    &   {{12{instw[31]}},instw[19:12],instw[20],instw[30:25],instw[24:21],1'b0})|
                ({32{Stype}}    &   {{21{instw[31]}},instw[30:25],instw[24:21],instw[20]});

    assign jump_bool=   instw_jal   |   instw_jalr;
    assign jump_addsel= instw_jalr;
    assign rfwmux[1]=instw_jal  |   instw_jalr;
    assign rfwmux[0]=instw_lui  |   instw_jal   |instw_jalr;
    assign rfwen=instw_addi | instw_auipc   |   instw_lui   | instw_jal |instw_jalr ;

    assign aluamux=instw_auipc;
    assign alubmux=instw_addi   | instw_auipc   |   instw_sw;
    
    
    assign ren=1'b0;
    assign wen= instw_sw;
    
    always @(*) begin
        if(instw_ebreak) ebreak();
    end
endmodule 

