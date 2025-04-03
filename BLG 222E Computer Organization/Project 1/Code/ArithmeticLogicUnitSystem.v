`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// 150210055 Taha Temiz
// 150210060 Mehmet Mert Ergin
//////////////////////////////////////////////////////////////////////////////////


module MUX4to1(I0,I1,I2,I3,Select,Output);
    input wire[15:0] I0;
    input wire[15:0] I1;
    input wire[7:0] I2;
    input wire[7:0] I3;
    input wire[1:0] Select;
    output reg[15:0] Output;
    
    
    always@(*)
    begin
        case(Select)
            2'b00: Output <= I0; 
            2'b01: Output <= I1; 
            2'b10: Output <= {{8{I2[7]}},{I2[7:0]}};
            2'b11: Output <= {{8{I3[7]}},{I3[7:0]}};
            default: Output <= I0;
         endcase
     end       
endmodule
module MUXC(Input,Select,Output);
    input wire[15:0] Input;
    input wire Select;
    output reg[7:0] Output;
    
    always@(*)
    begin
        case(Select)
            1'b0: Output <= Input[7:0];
            1'b1: Output <= Input[15:8];
            default: Output <= Input[7:0];
         endcase
     end       
endmodule


module ArithmeticLogicUnitSystem(
RF_OutASel,
RF_OutBSel,
RF_FunSel,
RF_RegSel,
RF_ScrSel,
ALU_FunSel,
ALU_WF,
ARF_OutCSel,
ARF_OutDSel,
ARF_FunSel,
ARF_RegSel,
IR_LH,
IR_Write,
Mem_WR,
Mem_CS,
MuxASel,
MuxBSel,
MuxCSel,
Clock);

    input wire[2:0] RF_OutASel;
    input wire[2:0] RF_OutBSel;
    input wire[2:0] RF_FunSel;
    input wire[3:0] RF_RegSel;
    input wire[3:0] RF_ScrSel;
    wire [15:0] OutA;
    wire [15:0] OutB;
    
    input wire[4:0] ALU_FunSel;
    input wire[0:0] ALU_WF;
    wire[15:0] ALUOut;
    wire[3:0] FlagsOut;
    
    
    input wire[1:0] ARF_OutCSel;
    input wire[1:0] ARF_OutDSel;
    input wire[2:0] ARF_FunSel;
    input wire[2:0] ARF_RegSel;
    wire [15:0] OutC;
    wire [15:0] Address;
    
    
    input wire[0:0] IR_LH;
    input wire[0:0] IR_Write;
    wire[15:0] IROut;
    
    input wire[0:0] Mem_WR;
    input wire[0:0] Mem_CS;
    wire[7:0] MemOut;
    
    input wire[1:0] MuxASel;
    wire[15:0] MuxAOut;
    
    input wire[1:0] MuxBSel;
    wire[15:0] MuxBOut;
    
    input wire MuxCSel;
    wire [7:0] MuxCOut;
    
    
    input wire Clock;
    
    RegisterFile RF(
    .I(MuxAOut),
    .OutASel(RF_OutASel),
    .OutBSel(RF_OutBSel),
    .FunSel(RF_FunSel),
    .RegSel(RF_RegSel),
    .ScrSel(RF_ScrSel),
    .Clock(Clock),
    .OutA(OutA),
    .OutB(OutB)
    );
    
    ArithmeticLogicUnit ALU(
    .A(OutA),
    .B(OutB),
    .FunSel(ALU_FunSel),
    .WF(ALU_WF),
    .Clock(Clock),
    .ALUOut(ALUOut),
    .FlagsOut(FlagsOut)
    );
    
    MUXC MuxC(
    .Input(ALUOut),
    .Select(MuxCSel),
    .Output(MuxCOut)
    );
    
    Memory MEM(
    .Address(Address),
    .Data(MuxCOut),
    .WR(Mem_WR),
    .CS(Mem_CS),
    .Clock(Clock),
    .MemOut(MemOut)
    );
    
    InstructionRegister IR(
    .I(MemOut),
    .Write(IR_Write),
    .LH(IR_LH),
    .Clock(Clock),
    .IROut(IROut)
    );
    
    MUX4to1 MuxA(
    .I0(ALUOut),
    .I1(OutC),
    .I2(MemOut),
    .I3(IROut[7:0]),
    .Select(MuxASel),
    .Output(MuxAOut)
    );
    
    MUX4to1 MuxB(
    .I0(ALUOut),
    .I1(OutC),
    .I2(MemOut),
    .I3(IROut[7:0]),
    .Select(MuxBSel),
    .Output(MuxBOut)
    );
    
    AddressRegisterFile ARF(
    .I(MuxBOut),
    .OutCSel(ARF_OutCSel),
    .OutDSel(ARF_OutDSel),
    .FunSel(ARF_FunSel),
    .RegSel(ARF_RegSel),
    .Clock(Clock),
    .OutC(OutC),
    .OutD(Address)
    );
    
    
    

endmodule
