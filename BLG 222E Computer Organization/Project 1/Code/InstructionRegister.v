`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// 150210055 Taha Temiz
// 150210060 Mehmet Mert Ergin
//////////////////////////////////////////////////////////////////////////////////


module InstructionRegister(I,Write,LH,Clock,IROut);
input wire[7:0] I;
input wire Write;
input wire LH;
input wire Clock;
output reg [15:0] IROut;
always @(posedge Clock)
begin
if(Write)
begin
    case(LH)
     1'b0 :  IROut[7:0] <= I[7:0];
     1'b1 :  IROut[15:8] <= I[7:0];
     default : IROut <= IROut;
     endcase
 end
 else
 begin
 IROut <= IROut;
 end
 end      

endmodule
