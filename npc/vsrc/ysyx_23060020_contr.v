import "DPI-C" context function void ebreak();
module ysyx_23060020_contr (
    input[31:0] instw,
    input[31:0] aluout,
    input zero,
    output jump_addsel,
    output rfwen,memvalid,wen,
    output[1:0] rfwmux,
    output[31:0] imm,
    output alubmux,aluamux,jump_bool,
    output [3:0] alusel,
    output[3:0] wmask,
    output load_signed_extends
);
    wire Itype,Utype,Jtype,Stype,Btype,ISPtype,Rtype,invaild;
    wire instw_addi,instw_ebreak,instw_lui,instw_andi;
    wire instw_jal,instw_jalr;
    wire instw_auipc,instw_sub,instw_sltiu;
    wire instw_beq,instw_bne,instw_blt,instw_bge,instw_bgeu,instw_bltu;
    wire instw_add,instw_sltu,instw_xor,instw_or,instw_sll,instw_slt;
    wire instw_and,instw_xori,instw_srl,instw_sra;
    wire instw_sw,instw_lw,instw_sh,instw_sb,instw_lbu,instw_lh ,instw_lhu;
    wire instw_slli,instw_srli,instw_srai;
    wire instw_init;


    assign instw_init=(instw==32'h00000000);
    assign instw_ebreak=                            (instw==32'h00100073);
    assign instw_lui=                               (                           instw[6:0]==7'b0110111);
    assign instw_auipc=                             (                           instw[6:0]==7'b0010111);
    assign instw_jal=                               (                           instw[6:0]==7'b1101111);
    assign instw_jalr=                              (instw[14:12]==3'b000&&     instw[6:0]==7'b1100111);
    assign instw_addi=                              (instw[14:12]==3'b000&&     instw[6:0]==7'b0010011);
    assign instw_andi=                              (instw[14:12]==3'b111&&     instw[6:0]==7'b0010011);
    assign instw_xori=                              (instw[14:12]==3'b100&&     instw[6:0]==7'b0010011);

    assign instw_add=   (instw[31:25]==7'b0000000&&  instw[14:12]==3'b000&&     instw[6:0]==7'b0110011);
    assign instw_sub=   (instw[31:25]==7'b0100000&&  instw[14:12]==3'b000&&     instw[6:0]==7'b0110011);
    assign instw_sll=   (instw[31:25]==7'b0000000&&  instw[14:12]==3'b001&&     instw[6:0]==7'b0110011);
    assign instw_slt=   (instw[31:25]==7'b0000000&&  instw[14:12]==3'b010&&     instw[6:0]==7'b0110011);
    assign instw_and=   (instw[31:25]==7'b0000000&&  instw[14:12]==3'b111&&     instw[6:0]==7'b0110011);
    assign instw_srl=   (instw[31:25]==7'b0000000&&  instw[14:12]==3'b101&&     instw[6:0]==7'b0110011);
    assign instw_sra=   (instw[31:25]==7'b0100000&&  instw[14:12]==3'b101&&     instw[6:0]==7'b0110011);

    assign instw_sltiu=                             (instw[14:12]==3'b011&&     instw[6:0]==7'b0010011);

    assign instw_beq=                               (instw[14:12]==3'b000&&     instw[6:0]==7'b1100011);
    assign instw_bne=                               (instw[14:12]==3'b001&&     instw[6:0]==7'b1100011);
    assign instw_blt=                               (instw[14:12]==3'b100&&     instw[6:0]==7'b1100011);
    assign instw_bge=                               (instw[14:12]==3'b101&&     instw[6:0]==7'b1100011);
    assign instw_bltu=                              (instw[14:12]==3'b110&&     instw[6:0]==7'b1100011);
    assign instw_bgeu=                              (instw[14:12]==3'b111&&     instw[6:0]==7'b1100011);

    assign instw_sltu=  (instw[31:25]==7'b0000000&&  instw[14:12]==3'b011&&     instw[6:0]==7'b0110011);
    assign instw_xor=   (instw[31:25]==7'b0000000&&  instw[14:12]==3'b100&&     instw[6:0]==7'b0110011);
    assign instw_or=    (instw[31:25]==7'b0000000&&  instw[14:12]==3'b110&&     instw[6:0]==7'b0110011);
    assign instw_slli=  (instw[31:25]==7'b0000000&&  instw[14:12]==3'b001&&     instw[6:0]==7'b0010011);
    assign instw_srli=  (instw[31:25]==7'b0000000&&  instw[14:12]==3'b101&&     instw[6:0]==7'b0010011);
    assign instw_srai=  (instw[31:25]==7'b0100000&&  instw[14:12]==3'b101&&     instw[6:0]==7'b0010011);


    assign instw_lw=                                (instw[14:12]==3'b010&&     instw[6:0]==7'b0000011);
    assign instw_lh=                                (instw[14:12]==3'b001&&     instw[6:0]==7'b0000011);
    assign instw_lhu=                               (instw[14:12]==3'b101&&     instw[6:0]==7'b0000011);
    assign instw_lbu=                               (instw[14:12]==3'b100&&     instw[6:0]==7'b0000011);
    assign instw_sb=                                (instw[14:12]==3'b000&&     instw[6:0]==7'b0100011);
    assign instw_sh=                                (instw[14:12]==3'b001&&     instw[6:0]==7'b0100011);
    assign instw_sw=                                (instw[14:12]==3'b010&&     instw[6:0]==7'b0100011);
    
    //assign instw_lhu=                               (instw[14:12]==3'b101&&     instw[6:0]==7'b0000011);

    assign Itype=instw_addi     |   instw_jalr  |   instw_lw    |   instw_sltiu |   instw_lbu
                                |   instw_andi  |   instw_xori  |   instw_lh    |   instw_lhu;
    assign Utype=instw_auipc    |   instw_lui;
    assign Jtype=instw_jal;
    assign Stype=instw_sw       |   instw_sh    |   instw_sb;
    assign Btype=instw_beq      |   instw_bne   |   instw_blt   |   instw_bge   |   instw_bgeu
                |   instw_bltu;
    assign ISPtype=instw_slli   |   instw_srli  |   instw_srai;
    assign Rtype=instw_sub      |   instw_add   |   instw_sltu  |   instw_xor   
                                |   instw_or    |   instw_sll   |   instw_slt   |   instw_and
                                |   instw_srl   |   instw_sra;

    assign imm= ({32{Itype}}    &   {{21{instw[31]}},instw[30:20]})|
                ({32{Utype}}    &   {{instw[31:12]},{12'd0}})|
                ({32{Jtype}}    &   {{12{instw[31]}},instw[19:12],instw[20],instw[30:25],instw[24:21],1'b0})|
                ({32{Stype}}    &   {{21{instw[31]}},instw[30:25],instw[11:7]})|
                ({32{Btype}}    &   {{20{instw[31]}},instw[7],instw[30:25],instw[11:8],{1'b0}})|
                ({32{ISPtype}}  &   {{27'd0},{instw[24:20]}});

    assign jump_bool=   instw_jal   |   instw_jalr  |   (instw_beq&&zero)   |(instw_bne&&(!(zero)))
                    |   (instw_blt&&(aluout=='d1))  |   (instw_bge&&(aluout=='d0))
                    |   (instw_bgeu&&(aluout=='d0)) |   (instw_bltu&&(aluout=='d1));
    assign jump_addsel= instw_jalr;
    assign rfwmux[1]=instw_jal  |   instw_jalr  |   instw_lw    |   instw_lbu   |   instw_lh
                    |instw_lhu;
    assign rfwmux[0]=instw_lui  |   instw_jal   |   instw_jalr;
    assign rfwen=instw_addi | instw_auipc   |   instw_lui   | instw_jal |instw_jalr 
                            |   instw_lw    |   instw_sub   |   instw_sltiu |   instw_add
                            |   instw_sltu  |   instw_xor   |   instw_or    
                            |   instw_slli  |   instw_srli  |   instw_srai  
                            |   instw_andi  |   instw_lbu   |   instw_sll   |   instw_slt
                            |   instw_and   |   instw_xori  |   instw_lh    |   instw_lhu
                            |   instw_srl   |   instw_sra;

    assign aluamux=instw_auipc;
    assign alubmux=instw_addi   | instw_auipc   |   instw_sw    |   instw_lw    |   instw_sltiu
                                |   instw_sh    |   instw_andi  |   instw_lbu   |   instw_xori
                                |   instw_slli  |   instw_srli  |   instw_srai  |   instw_sb
                                |   instw_lh    |   instw_lhu;
    
    
    assign memvalid=instw_sw    |   instw_lw    |   instw_sh |   instw_lbu   |   instw_sb
                                |   instw_lh    |   instw_lhu;
    assign wen= instw_sw        |   instw_sh       |   instw_sb;
    assign wmask[3]=instw_lw    |   instw_sw;
    assign wmask[2]=instw_sw    |   instw_lw;
    assign wmask[1]=instw_sw    |   instw_sh    |   instw_lw    |   instw_lh    |   instw_lhu;
    assign wmask[0]=instw_sw    |   instw_sh    |   instw_lbu   |   instw_lw    |   instw_sb    |   instw_lh
                                |   instw_lhu;


    assign alusel[3]=   instw_sltiu |   instw_sltu  |   instw_srai  |   instw_slt   |   instw_blt
                        |instw_bge  |   instw_bgeu  |   instw_bltu  |   instw_sra;
    assign alusel[2]=   instw_xor   |   instw_or    |   instw_slli  |   instw_srli  
                       |instw_sll   |   instw_xori  |   instw_srl;
    assign alusel[1]=   instw_sub   |   instw_beq   |   instw_bne   |   instw_slli  |   instw_srli
                       |instw_sll   |   instw_slt   |   instw_and   |   instw_andi  |   instw_slt
                       |instw_bge   |   instw_blt   |   instw_srl;
    assign alusel[0]=   instw_addi  |   instw_auipc |   instw_sw    |   instw_lw    |   instw_sltiu
                    |   instw_add   |   instw_sltu  |   instw_xor   |   instw_srli  
                    |   instw_sh    |   instw_andi  |   instw_lbu   |   instw_and   |   instw_xori
                    |   instw_sb    |   instw_bgeu  |   instw_bltu  |   instw_lh    |   instw_lhu
                    |   instw_srl;
    assign load_signed_extends=instw_lh;


    

    assign invaild=Itype|ISPtype|Utype|Jtype|Stype|Btype|Rtype|instw_init;
    always @(*) begin
        if(instw_ebreak) ebreak();
    end
    always @(*) begin
        if(!invaild) ebreak();
    end
endmodule 

