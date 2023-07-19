module top (
    input clk,rst,en,
    input[7:0] datain,
    output en_out,
    output[2:0] b,
    output[7:0] seg
);
    wire[7:0] segnot;

    assign b=   en       ? 0    :(
                datain[7]? 3'd7 :(
                datain[6]? 3'd6 :(
                datain[5]? 3'd5 :(
                datain[4]? 3'd4 :(
                datain[3]? 3'd3 :(
                datain[2]? 3'd2 :(
                datain[1]? 3'd1 :(
                datain[0]? 3'd0 :0    
                ))))))));
    assign en_out=|datain;
MuxKey#(8,3,8) u_MuxKey(
    segnot,b,{
        3'd0,8'b11111101,
        3'd1,8'b01100000,
        3'd2,8'b11011010,
        3'd3,8'b11110010,
        3'd4,8'b01100110,
        3'd5,8'b10110110,
        3'd6,8'b10111110,
        3'd7,8'b11100000
    } );
assign seg=~segnot;
endmodule