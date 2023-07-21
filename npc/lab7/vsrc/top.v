module top (
    input clk,rst,ps2_clk,ps2_data,
    output[7:0] seg0,
    output[7:0] seg1,
    output[7:0] seg2,
    output[7:0] seg3
);
    
    wire[7:0] seg_key,key_count;
    wire[7:0] seg0_not,seg1_not,seg2_not,seg3_not;
    reg[7:0] seg_key_temp;
    wire state;
    ps2_keyboard my_keyboard(
    .clk(clk),
    .resetn(~rst),
    .ps2_clk(ps2_clk),
    .ps2_data(ps2_data),
    .seg_key(seg_key),
    .key_count(key_count),
    .state(state)
);
   
  MuxKey #(2, 1, 8) s0_mux (seg_key_temp, state,{
    1'b0,seg_key,
    1'b1,8'd0
  });

   MuxKey #(16, 4, 8) u0_mux (seg0_not, seg_key_temp[3:0],{
    4'd0, 8'b00000000,
    4'd1, 8'b01100000,
    4'd2, 8'b11011010,
    4'd3, 8'b11110010,
    4'd4, 8'b01100110,
    4'd5, 8'b10110110,
    4'd6, 8'b10111110,
    4'd7, 8'b11100000,
    4'd8, 8'b11111110,
    4'd9, 8'b11110110,
    4'd10,8'b11101110,
    4'd11,8'b00111110,
    4'd12,8'b10011100,
    4'd13,8'b01111010,
    4'd14,8'b10011110,
    4'd15,8'b10001110
  });
  MuxKey #(16, 4, 8) u1_mux (seg1_not, seg_key_temp[7:4], {
    4'd0, 8'b00000000,
    4'd1, 8'b01100000,
    4'd2, 8'b11011010,
    4'd3, 8'b11110010,
    4'd4, 8'b01100110,
    4'd5, 8'b10110110,
    4'd6, 8'b10111110,
    4'd7, 8'b11100000,
    4'd8, 8'b11111110,
    4'd9, 8'b11110110,
    4'd10,8'b11101110,
    4'd11,8'b00111110,
    4'd12,8'b10011100,
    4'd13,8'b01111010,
    4'd14,8'b10011110,
    4'd15,8'b10001110
  });
  MuxKey #(16, 4, 8) u2_mux (seg2_not, key_count[3:0], {
    4'd0, 8'b11111100,
    4'd1, 8'b01100000,
    4'd2, 8'b11011010,
    4'd3, 8'b11110010,
    4'd4, 8'b01100110,
    4'd5, 8'b10110110,
    4'd6, 8'b10111110,
    4'd7, 8'b11100000,
    4'd8, 8'b11111110,
    4'd9, 8'b11110110,
    4'd10,8'b11101110,
    4'd11,8'b00111110,
    4'd12,8'b10011100,
    4'd13,8'b01111010,
    4'd14,8'b10011110,
    4'd15,8'b10001110
  });
  MuxKey #(16, 4, 8) u3_mux (seg3_not, key_count[7:4], {
    4'd0, 8'b11111100,
    4'd1, 8'b01100000,
    4'd2, 8'b11011010,
    4'd3, 8'b11110010,
    4'd4, 8'b01100110,
    4'd5, 8'b10110110,
    4'd6, 8'b10111110,
    4'd7, 8'b11100000,
    4'd8, 8'b11111110,
    4'd9, 8'b11110110,
    4'd10,8'b11101110,
    4'd11,8'b00111110,
    4'd12,8'b10011100,
    4'd13,8'b01111010,
    4'd14,8'b10011110,
    4'd15,8'b10001110
  });
  assign seg0=~seg0_not;
  assign seg1=~seg1_not;
  assign seg2=~seg2_not;
  assign seg3=~seg3_not;
endmodule

module ps2_keyboard(clk,resetn,ps2_clk,ps2_data,seg_key,key_count,state);
    input clk,resetn,ps2_clk,ps2_data;
    output reg [7:0] seg_key;
    output reg [7:0] key_count;
    output reg state;
    reg [9:0] buffer;        // ps2_data bits
    reg [3:0] count;  // count ps2_data bits
    reg [2:0] ps2_clk_sync;
    reg [7:0] seg_key_temp;
    

    always @(posedge clk) begin
        ps2_clk_sync <=  {ps2_clk_sync[1:0],ps2_clk};
    end

    wire sampling = ps2_clk_sync[2] & ~ps2_clk_sync[1];
    always @(posedge clk ) begin
      if (resetn == 0) begin // reset
            key_count<=0;
        end
    end

    always @(posedge clk) begin
        if (resetn == 0) begin // reset
            count <= 0;
        end
        else begin
            if (sampling) begin
              if (count == 4'd10) begin
                if ((buffer[0] == 0) &&  // start bit
                    (ps2_data)       &&  // stop bit
                    (^buffer[9:1])) begin      // odd  parity
                    $display("receive %x", buffer[8:1]);
                    seg_key<=buffer[8:1];
                    seg_key_temp<=seg_key;
                    if(seg_key==buffer[8:1]&&seg_key_temp==8'hf0) 
                    begin
                      key_count<=key_count+1;
                      state<=0;
                    end
                    else if(seg_key!=buffer[8:1]&&seg_key!=8'hf0&&buffer[8:1]!=8'hf0) 
                    begin
                      key_count<=key_count+1;
                      state<=0;
                    end
                    else if(seg_key==buffer[8:1]&&seg_key_temp!=8'hf0)
                    begin
                      state<=state;
                    end
                    else if(seg_key!=buffer[8:1]&&(seg_key==8'hf0||buffer[8:1]==8'hf0))
                    begin
                      state<=1;
                    end
                end
                count <= 0;     // for next
              end else begin
                buffer[count] <= ps2_data;  // store ps2_data
                count <= count + 3'b1;
              end
            end
        end
    end
endmodule