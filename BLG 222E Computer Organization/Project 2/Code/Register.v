`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// 150210055 Taha Temiz
// 150210060 Mehmet Mert Ergin
//////////////////////////////////////////////////////////////////////////////////

module Register(I,E,FunSel,Clock,Q);
  input wire [15:0] I;
  input wire E;
  input wire [2:0] FunSel;
  input wire Clock;
  output reg [15:0] Q;
    
    always @(posedge Clock)
    begin
    if(E)
    begin
    case(FunSel)
        3'b000: Q <= Q - 16'b0000000000000001;
        3'b001: Q <= Q + 16'b0000000000000001;
        3'b010: Q <= I[15:0];
        3'b011: Q <= 16'b0000000000000000;
        3'b100: Q <= {{8'b00000000},{I[7:0]}};
        3'b101: Q[7:0] <= I[7:0];
        3'b110: Q[15:8] <= I[7:0];
        3'b111: Q <= {{8{I[7]}},{I[7:0]}};
       default: Q <= Q;
      endcase
       end
       else
       begin
       Q <= Q;
       end
       end
        
    endmodule
    
    
