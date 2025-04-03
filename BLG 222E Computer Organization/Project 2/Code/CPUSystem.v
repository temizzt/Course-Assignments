`timescale 1ns / 1ps
//150210055 Taha TEMÝZ
//150210060 Mehmet Mert ERGÝN
//

module Counter(Clock,Reset,Count,O);
input wire Clock,Reset,Count;
output reg [2:0] O = 3'b0; // MAX count is 7. So it can not reach T8. After T7(111) it would be T0(1 000) 

    always@(posedge Clock)
        begin
            if(!Reset) // It resets when SC_Reset <= 0;
                O <= 0;
            else if(Count)
                O <= O + 1;
             else
             O <= O;
         end
endmodule

module Decoder_6to64(E,I,O); // To decode opcodes
    input wire E;
    input wire [5:0] I;
    output reg [63:0]O;
    
    always@(*)
    begin
        if(E)
            begin
                case(I)
                    6'h0 : O =  64'h0000000000000001;
                    6'h1 : O =  64'h000000000000002;
                    6'h2 : O =  64'h000000000000004;
                    6'h3 : O =  64'h0000000000000008;
                    6'h4 : O =  64'h0000000000000010;
                    6'h5 : O =  64'h0000000000000020;
                    6'h6 : O =  64'h0000000000000040;
                    6'h7 : O =  64'h0000000000000080;
                    6'h8 : O =  64'h0000000000000100;
                    6'h9 : O =  64'h0000000000000200;
                    6'hA : O =  64'h0000000000000400;
                    6'hB : O =  64'h0000000000000800;
                    6'hC : O =  64'h0000000000001000;
                    6'hD : O =  64'h0000000000002000;
                    6'hE : O =  64'h0000000000004000;
                    6'hF : O =  64'h0000000000008000;
                    6'h10 : O = 64'h0000000000010000;
                    6'h11 : O = 64'h0000000000020000;
                    6'h12 : O = 64'h0000000000040000;
                    6'h13 : O = 64'h0000000000080000;
                    6'h14 : O = 64'h0000000000100000;
                    6'h15 : O = 64'h0000000000200000;
                    6'h16 : O = 64'h0000000000400000;
                    6'h17 : O = 64'h0000000000800000;
                    6'h18 : O = 64'h0000000001000000;
                    6'h19 : O = 64'h0000000002000000;
                    6'h1A : O = 64'h0000000004000000;
                    6'h1B : O = 64'h0000000008000000;
                    6'h1C : O = 64'h0000000010000000;
                    6'h1D : O = 64'h0000000020000000;
                    6'h1E : O = 64'h0000000040000000;
                    6'h1F : O = 64'h0000000080000000;
                    6'h20 : O = 64'h0000000100000000;
                    6'h21 : O = 64'h0000000200000000;
                    default : O = 0;
               endcase
               end
               else
               begin
                O = O;
               end
            end
endmodule

module Decoder_3to8(E,I,O); // To decode Tx
    input wire E;
input wire [2:0] I;
output reg [7:0]O;

always@(*)
begin
    if(E)
        begin
            case(I)
                3'h0 : O =  8'b00000001;
                3'h1 : O =  8'b00000010;
                3'h2 : O =  8'b00000100;
                3'h3 : O =  8'b00001000;
                3'h4 : O =  8'b00010000;
                3'h5 : O =  8'b00100000;
                3'h6 : O =  8'b01000000;
                3'h7 : O =  8'b10000000;
                default : O = 0;
           endcase
           end
           else
           begin
            O = O;
           end
        end
endmodule



module InstructionDecoder(T2,S,IR_H,D,RSEL);
    input wire [7:0] IR_H;
    input wire T2;
    output wire [63:0] D;
    output reg S;
    output reg [1:0] RSEL;
    Decoder_6to64 opcode_decode(T2,IR_H[7:2],D);
        always @(*)
        begin
            if(T2)
            begin
                RSEL <= IR_H[1:0];
                S <= IR_H[1];
            end    
        end
endmodule

module control_unit(Clock, IROut, ARF_FunSel,ARF_RegSel,ARF_OutCSel,ARF_OutDSel,ALU_WF,ALU_FlagsOut,ALU_FunSel,RF_OutASel,RF_OutBSel,RF_FunSel,RF_RegSel,RF_ScrSel,MuxASel,MuxBSel,MuxCSel,Mem_CS,Mem_WR,IR_LH,IR_Write,ResetFromOut);
input wire Clock;
input wire [15:0] IROut;
input wire [3:0] ALU_FlagsOut;

input wire ResetFromOut; // This was for considering the reset input.But it did not work.

reg SC_Reset;

output reg ALU_WF;

output reg [2:0] ARF_FunSel;
output reg [2:0] ARF_RegSel;
output reg [1:0] ARF_OutCSel;
output reg [1:0] ARF_OutDSel;
output reg [4:0] ALU_FunSel;
output reg [2:0] RF_OutASel;
output reg [2:0] RF_OutBSel;
output reg [2:0] RF_FunSel;
output reg [3:0] RF_RegSel;
output reg [3:0] RF_ScrSel;

output reg [1:0] MuxASel;
output reg [1:0] MuxBSel;
output reg MuxCSel;

output reg Mem_CS;
output reg Mem_WR;

output reg IR_LH;
output reg IR_Write;

wire [7:0] T;
wire [2:0] tempdata;




Counter SC(Clock,SC_Reset,1'b1,tempdata); // To determine T it should always be enable. It only counts with clock signal, so this should not be a problem.
Decoder_3to8 SCDecoder(1'b1,tempdata,T);

initial begin
    Mem_WR <= 1'b0;
    Mem_CS <= 1'b0;
    ARF_FunSel <= 3'b011;
    RF_FunSel <= 3'b011;
    RF_RegSel <= 4'b0000;
    ARF_RegSel <= 3'b000;
    RF_ScrSel <= 4'b0000;
    ALU_WF <= 1'b0;
    SC_Reset <= ResetFromOut;
    
    #50;
    RF_RegSel <= 4'b1111;
    ARF_RegSel <= 3'b111;
    RF_ScrSel <=4'b1111;
    SC_Reset <= 1;
    #50;
end


always @(posedge Clock)begin // Reseting the system
    if(SC_Reset == 0)begin
        RF_RegSel <= 4'bZ;
        RF_ScrSel <= 4'bZ;
        RF_FunSel <= 3'bZ;
        ALU_FunSel <= 5'bZ;
        ALU_WF <= 1'bZ;
        MuxCSel <= 1'bZ;
        MuxASel <= 2'bZ;
        MuxBSel <= 2'bZ;
        ARF_FunSel <= 3'bZ;
        ARF_RegSel <= 3'bZ;
        Mem_WR <= 0;
        Mem_CS <= 0;
        end
        end
        
        
//Fetching low
always @(T[0])
begin
    if(T[0] && SC_Reset == 1)
    begin
        ARF_OutCSel <= 2'bZ;
        ARF_OutDSel <= 2'b00;
        Mem_CS <= 1'b0;
        Mem_WR <= 1'b0;
        IR_LH <= 1'b0;
        IR_Write <= 1'b1;
        ARF_RegSel <= 3'b011;
        ARF_FunSel <= 3'b001;
     end
     else
     begin   
        ARF_OutCSel <= 2'bZ;
        ARF_OutDSel <= 2'bZ;
        Mem_CS <= 1'bZ;
        Mem_WR <= 1'bZ;
        IR_LH <= 1'bZ;
        IR_Write <= 1'bZ;
        ARF_RegSel <= 3'b111;
        ARF_FunSel <= 3'bZ;  
      end
    end
//Fetching high 
always @(T[1])
    begin
        if(T[1])
        begin
            ARF_OutCSel <= 2'bZ;
            ARF_OutDSel <= 2'b00;
            Mem_CS <= 1'b0;
            Mem_WR <= 1'b0;
            IR_LH <= 1'b1;
            IR_Write <= 1'b1;
            ARF_RegSel <= 3'b011;
            ARF_FunSel <= 3'b001;
         end
         else
         begin   
            ARF_OutCSel <= 2'bZ;
            ARF_OutDSel <= 2'bZ;
            Mem_CS <= 1'bZ;
            Mem_WR <= 1'bZ;
            IR_LH <= 1'bZ;
            IR_Write <= 1'bZ;
            ARF_RegSel <= 3'b111;
            ARF_FunSel <= 3'bZ;  
          end
        end    
        
wire [63:0] D;
wire S;
wire [1:0] RSEL;        
        
InstructionDecoder ID(T[2],S,IROut[15:8],D,RSEL); // Decoding the opcode

wire Z = ALU_FlagsOut[3];
wire [2:0] DSTREG = {RSEL[0],IROut[7:6]};
wire [2:0] SREG1 = IROut[5:3];
wire [2:0] SREG2 = IROut[2:0];

reg[1:0] Sources = 2'b11; // One bit for SREG1 one bit for SREG2. If it is in ARF it is 0, if it is in RF it is 1.

always @(*) begin 
    if(T[0]) 
        begin
            SC_Reset <= 1'b1;
            Mem_WR <= 1'b0;
            Mem_CS <= 1'b0;
            ARF_FunSel <= 3'bZZZ;
            RF_FunSel <= 3'bZZZ;
            RF_RegSel <= 4'bZZZZ;
            ARF_RegSel <= 3'bZZZ;
            RF_ScrSel <= 4'bZZZZ;
            ALU_WF <= 1'bZ;
            ALU_FunSel <= 5'bZZZZZ;
         end
         
         case (D) // According to the opcode. We adjust ALU function.
            64'h0000000000000001: ALU_FunSel <= 5'b10100; // BRA
            64'h000000000000002:  ALU_FunSel <= 5'b10100; //BNE
            64'h000000000000004: ALU_FunSel <= 5'b10100; //BEQ
            64'h0000000000000008: ALU_FunSel <= 5'b10000; // POP
            64'h0000000000000010: ALU_FunSel <= 5'b10000; //PSH
            64'h0000000000000020: ALU_FunSel <= 5'b10000; //INC //First, load data into DST, then increment.So, ALU operation is A.
            64'h0000000000000040: ALU_FunSel <= 5'b10000; //DEC /First, load data into DST, then decrement.So, ALU operation is A.
            64'h0000000000000080: ALU_FunSel <= 5'b11011; //LSL
            64'h0000000000000100: ALU_FunSel <= 5'b11100; //LSR
            64'h0000000000000200: ALU_FunSel <= 5'b11101; //ASR
            64'h0000000000000400: ALU_FunSel <= 5'b11110; //CSL
            64'h0000000000000800: ALU_FunSel <= 5'b11111; //CSR
            64'h0000000000001000: ALU_FunSel <= 5'b10111; //AND
            64'h0000000000002000: ALU_FunSel <= 5'b11000; //ORR
            64'h0000000000004000: ALU_FunSel <= 5'b10010; //NOT
            64'h0000000000008000: ALU_FunSel <= 5'b11001; //XOR
            64'h0000000000010000: ALU_FunSel <= 5'b11010; //NAND
            64'h0000000000020000: ALU_FunSel <= 5'b10000; //MOVH
            64'h0000000000040000: ALU_FunSel <= 5'b10000; //LDR
            64'h0000000000080000: ALU_FunSel <= 5'b10000; //STR
            64'h0000000000100000: ALU_FunSel <= 5'b10000; //MOVL
            64'h0000000000200000: ALU_FunSel <= 5'b10100; //ADD
            64'h0000000000400000: ALU_FunSel <= 5'b10101; //ADDC
            64'h0000000000800000: ALU_FunSel <= 5'b10110; //SUB
            64'h0000000001000000: ALU_FunSel <= 5'b10000; //MOVS
            64'h0000000002000000: ALU_FunSel <= 5'b10100; //ADDS
            64'h0000000004000000: ALU_FunSel <= 5'b10110; //SUBS
            64'h0000000008000000: ALU_FunSel <= 5'b10111; //ANDS
            64'h0000000010000000: ALU_FunSel <= 5'b11000; //ORRS
            64'h0000000020000000: ALU_FunSel <= 5'b11001; //XORS
            64'h0000000040000000: ALU_FunSel <= 5'b10000; //BX
            64'h0000000080000000: ALU_FunSel <= 5'b10000; //BL
            64'h0000000100000000: ALU_FunSel <= 5'b10000; //LDRIM
            64'h0000000200000000: ALU_FunSel <= 5'b10100; //STRIM
          endcase 
          
    if(D[18] || D[19] || D[20] || D[21] || D[22] || D[23])begin // To check the S value for "Flags will change" opcodes.
        ALU_WF <= S;
        end       
          
   //SREG1 from RF  It does not matter if the second source is ARF or not.We can send the value to ALU in T2.    
   if((D[5] || D[6]|| D[7]|| D[8]|| D[9]|| D[10]|| D[11]|| D[12]|| D[13]|| D[14]|| D[15]|| D[16] || D[21]|| D[22]|| D[23]|| D[24]|| D[25]|| D[26]|| D[27]|| D[28]|| D[29]) && T[2] && SREG1[2] == 1)         
     begin
        RF_OutASel <= {1'b0,SREG1[1:0]};
        Sources <= {SREG1[2],Sources[0]};
      end 
    //SREG2 from RF  It does not matter if the first source is ARF or not.We can send the value to ALU in T2. 
    if((D[12]|| D[13]|| D[15]|| D[16]|| D[21]|| D[22]|| D[23]|| D[25]|| D[26]|| D[27]|| D[28]|| D[29]) && T[2] && SREG2[2] ==1)   
      begin
       RF_OutBSel <=  {1'b0,SREG2[1:0]};
       Sources <= {Sources[1], SREG2[2]};
      end

 //SREG1 or SREG2 is ARF, we need to check if it is a double ARF or not.
 //SREG1 from ARF, SREG2 from RF. Also, If we only need one source. This code is okay we guess.
       if((D[7]|| D[8]|| D[9]|| D[10]|| D[11]|| D[12]|| D[13]|| D[14]|| D[15]|| D[16] || D[21]|| D[22]|| D[23]|| D[24]|| D[25]|| D[26]|| D[27]|| D[28]|| D[29]) && T[2] && SREG1[2] == 0 && SREG2[2] == 1)         
      begin
        case (SREG1[1:0])
            2'b00: ARF_OutCSel <= 2'b00; //SREG1  is PC
            2'b01: ARF_OutCSel <= 2'b00; //SREG1  is PC   
            2'b10: ARF_OutCSel <= 2'b11; //SREG1  is SP
            2'b11: ARF_OutCSel <= 2'b10; //SREG1  is AR
            default: ARF_OutCSel <= 2'b00;
         endcase
         MuxASel<= 2'b01;
         RF_ScrSel <= 4'b0111;
         RF_FunSel <= 3'b010;
           
         Sources <= {SREG1[2],Sources[0]};
       end
       
         //SREG2 from ARF, SREG1 from RF
           if((D[12]|| D[13]|| D[15]|| D[16]|| D[21]|| D[22]|| D[23]|| D[25]|| D[26]|| D[27]|| D[28]|| D[29]) && T[2] && SREG2[2] == 0 && SREG1[2] ==1)   
      begin
             case (SREG2[1:0])
                 2'b00: ARF_OutCSel <= 2'b00; //SREG2  is PC
                 2'b01: ARF_OutCSel <= 2'b00; //SREG2  is PC   
                 2'b10: ARF_OutCSel <= 2'b11; //SREG2  is SP
                 2'b11: ARF_OutCSel <= 2'b10; //SREG2  is AR
                 default: ARF_OutCSel <= 2'b00;
              endcase
              MuxASel<= 2'b01;
              RF_ScrSel <= 4'b0111;
              RF_FunSel <= 3'b010;
                
              Sources <= {SREG1[2],SREG2[2]}; 
            end
         //If the sources are ARF and ARF. We need to first load the data from SREG1 into S1, then we need to load the data from SREG2 into S2. We can not do both operations in the same clock cycle. 
         if((D[7]|| D[8]|| D[9]|| D[10]|| D[11]|| D[12]|| D[13]|| D[14]|| D[15]|| D[16] || D[21]|| D[22]|| D[23]|| D[24]|| D[25]|| D[26]|| D[27]|| D[28]|| D[29]) && T[2] && SREG1[2] == 0 && SREG2[2] == 0)         
           begin
             case (SREG1[1:0])
                 2'b00: ARF_OutCSel <= 2'b00; //SREG1  is PC
                 2'b01: ARF_OutCSel <= 2'b00; //SREG1  is PC   
                 2'b10: ARF_OutCSel <= 2'b11; //SREG1  is SP
                 2'b11: ARF_OutCSel <= 2'b10; //SREG1  is AR
                 default: ARF_OutCSel <= 2'b00;
              endcase
              MuxASel<= 2'b01;
              RF_ScrSel <= 4'b0111; // Loading SREG1 to S1 Reg
              RF_FunSel <= 3'b010;
                
              Sources <= {SREG1[2],SREG2[2]};
            end  
            
      if((D[12]|| D[13]|| D[15]|| D[16]|| D[21]|| D[22]|| D[23]|| D[25]|| D[26]|| D[27]|| D[28]|| D[29]) && T[3] && SREG1[2] == 0 && SREG2[2] ==0)   
       begin
              case (SREG2[1:0])
                  2'b00: ARF_OutCSel <= 2'b00; //SREG2  is PC
                  2'b01: ARF_OutCSel <= 2'b00; //SREG2  is PC   
                  2'b10: ARF_OutCSel <= 2'b11; //SREG2  is SP
                  2'b11: ARF_OutCSel <= 2'b10; //SREG2  is AR
                  default: ARF_OutCSel <= 2'b00;
               endcase
               MuxASel<= 2'b01;
               RF_ScrSel <= 4'b1011;  //Loading SREG2 to S2 Reg
               RF_FunSel <= 3'b010;
                 
               Sources <= {SREG1[2],SREG2[2]};
             end
 
 
 //OPERATIONS WITH ONE SOURCE
 //RF <- RF
    if((D[5] || D[6] || D[7]|| D[8]|| D[9]|| D[10]|| D[11]|| D[14]|| D[24]) && T[2] && Sources[1]==1 && DSTREG[2] == 1)
        begin
            MuxASel <= 2'b00;
            RF_FunSel <= 3'b010;
         case(DSTREG[1:0])
            2'b00: RF_RegSel <= 4'b0111;
            2'b01: RF_RegSel <= 4'b1011;
            2'b10: RF_RegSel <= 4'b1101;
            2'b11: RF_RegSel <= 4'b1110;
          endcase
          
          SC_Reset <= 0; //Reset when SC_Reset = 0
      
        end
        
        //RF <- ARF
            if((D[7]|| D[8]|| D[9]|| D[10]|| D[11]|| D[14]|| D[24]) && T[3] && Sources[1]==0 && DSTREG[2] == 1)
            begin
                RF_OutASel <= 3'b100;
                MuxASel <= 2'b00;
                RF_FunSel <= 3'b010;
             case(DSTREG[1:0])
                2'b00: RF_RegSel <= 4'b0111;
                2'b01: RF_RegSel <= 4'b1011;
                2'b10: RF_RegSel <= 4'b1101;
                2'b11: RF_RegSel <= 4'b1110;
              endcase
            
            
                SC_Reset <= 0;
            end
            
           //ARF <- RF 
             if((D[5] || D[6] || D[7]|| D[8]|| D[9]|| D[10]|| D[11]|| D[14]|| D[24]) && T[2] && Sources[1]==1 && DSTREG[2] == 0)
                begin     
               MuxBSel <= 2'b00;
               ARF_FunSel <= 3'b010;
               
               case(DSTREG[1:0])
                2'b00: ARF_RegSel <= 3'b011; //PC is enabled
                2'b01: ARF_RegSel <= 3'b011; //PC is enabled
                2'b10: ARF_RegSel <= 3'b110; //SP is enabled
                2'b11: ARF_RegSel <= 3'b101; //AR is enabled
                endcase
                
                SC_Reset <=0;
                end 
                
         //ARF <- ARF       
                if((D[7]|| D[8]|| D[9]|| D[10]|| D[11]|| D[14]|| D[24]) && T[3] && Sources[1]==0 && DSTREG[2] == 0)
                   begin
                  RF_OutASel <= 3'b100;
                  MuxBSel <= 2'b00;
                  ARF_FunSel <= 3'b010;
                  
                  case(DSTREG[1:0])
                   2'b00: ARF_RegSel <= 3'b011; //PC is enabled
                   2'b01: ARF_RegSel <= 3'b011; //PC is enabled
                   2'b10: ARF_RegSel <= 3'b110; //SP is enabled
                   2'b11: ARF_RegSel <= 3'b101; //AR is enabled
                   endcase
                   
                   SC_Reset <=0;
                   end   
     //FOLLOWING IF BLOCKS ARE FOR THESE OPERATIONS 
    //RF<- RF op RF // Loading into DST in T2
    //RF <- RF op ARF // T3
    //RF <- ARF op RF // T3
    //RF <- ARF op ARF //T4
    //ARF<- RF op RF // T2
    //ARF <- RF op ARF //T3
    //ARF <- ARF op ARF //T3
    //ARF <- ARF op ARF  //T4
    if((D[12] || D[13]|| D[15]|| D[16]|| D[21]|| D[22]|| D[23]|| D[25]|| D[26]|| D[27]|| D[28]|| D[29]) && T[2] && Sources == 2'b11 && DSTREG[2] == 1)
    begin
    MuxASel <= 2'b00;
    RF_FunSel <= 3'b010;

    case(DSTREG[1:0])
        2'b00: RF_RegSel <= 4'b0111;
        2'b01: RF_RegSel <= 4'b1011;
        2'b10: RF_RegSel <= 4'b1101;
        2'b11: RF_RegSel <= 4'b1110;
    endcase
    
    SC_Reset <= 0;
    end
       
 if((D[12] || D[13]|| D[15]|| D[16]|| D[21]|| D[22]|| D[23]|| D[25]|| D[26]|| D[27]|| D[28]|| D[29]) && T[3] && Sources == 2'b10 && DSTREG[2] == 1)
     begin
 MuxASel <= 2'b00;
 RF_FunSel <= 3'b010;
     RF_OutBSel <= 3'b100;
 case(DSTREG[1:0])
     2'b00: RF_RegSel <= 4'b0111;
     2'b01: RF_RegSel <= 4'b1011;
     2'b10: RF_RegSel <= 4'b1101;
     2'b11: RF_RegSel <= 4'b1110;
 endcase
 
 SC_Reset <=0;
 end
 if((D[12] || D[13]|| D[15]|| D[16]|| D[21]|| D[22]|| D[23]|| D[25]|| D[26]|| D[27]|| D[28]|| D[29]) && T[3] && Sources == 2'b01 && DSTREG[2] == 1)
     begin
 MuxASel <= 2'b00;
 RF_FunSel <= 3'b010;
     RF_OutASel <= 3'b100;
 case(DSTREG[1:0])
     2'b00: RF_RegSel <= 4'b0111;
     2'b01: RF_RegSel <= 4'b1011;
     2'b10: RF_RegSel <= 4'b1101;
     2'b11: RF_RegSel <= 4'b1110;
 endcase
 
  SC_Reset <=0;
 end
 if((D[12] || D[13]|| D[15]|| D[16]|| D[21]|| D[22]|| D[23]|| D[25]|| D[26]|| D[27]|| D[28]|| D[29]) && T[4] && Sources == 2'b00 && DSTREG[2] == 1)
     begin
 MuxASel <= 2'b00;
 RF_FunSel <= 3'b010;
     RF_OutASel <= 3'b100;
         RF_OutBSel <= 3'b101;
 case(DSTREG[1:0])
     2'b00: RF_RegSel <= 4'b0111;
     2'b01: RF_RegSel <= 4'b1011;
     2'b10: RF_RegSel <= 4'b1101;
     2'b11: RF_RegSel <= 4'b1110;
 endcase
 
  SC_Reset <=0;
 end
 if((D[12] || D[13]|| D[15]|| D[16]|| D[21]|| D[22]|| D[23]|| D[25]|| D[26]|| D[27]|| D[28]|| D[29]) && T[2] && Sources == 2'b11 && DSTREG[2] == 0)
                    begin
MuxBSel <= 2'b00;
ARF_FunSel <= 3'b010;

case(DSTREG[1:0])
 2'b00: ARF_RegSel <= 3'b011; //PC is enabled
 2'b01: ARF_RegSel <= 3'b011; //PC is enabled
 2'b10: ARF_RegSel <= 3'b110; //SP is enabled
 2'b11: ARF_RegSel <= 3'b101; //AR is enabled
 endcase
  SC_Reset <=0;
 end 
 if((D[12] || D[13]|| D[15]|| D[16]|| D[21]|| D[22]|| D[23]|| D[25]|| D[26]|| D[27]|| D[28]|| D[29]) && T[3] && Sources == 2'b10 && DSTREG[2] == 0)
                    begin
MuxBSel <= 2'b00;
ARF_FunSel <= 3'b010;
    RF_OutBSel <= 3'b100;

case(DSTREG[1:0])
 2'b00: ARF_RegSel <= 3'b011; //PC is enabled
 2'b01: ARF_RegSel <= 3'b011; //PC is enabled
 2'b10: ARF_RegSel <= 3'b110; //SP is enabled
 2'b11: ARF_RegSel <= 3'b101; //AR is enabled
 endcase
 
  SC_Reset <=0;
 end 
 if((D[12] || D[13]|| D[15]|| D[16]|| D[21]|| D[22]|| D[23]|| D[25]|| D[26]|| D[27]|| D[28]|| D[29]) && T[3] && Sources == 2'b01 && DSTREG[2] == 0)
                    begin
MuxBSel <= 2'b00;
ARF_FunSel <= 3'b010;
    RF_OutASel <= 3'b100;

case(DSTREG[1:0])
 2'b00: ARF_RegSel <= 3'b011; //PC is enabled
 2'b01: ARF_RegSel <= 3'b011; //PC is enabled
 2'b10: ARF_RegSel <= 3'b110; //SP is enabled
 2'b11: ARF_RegSel <= 3'b101; //AR is enabled
 endcase
  SC_Reset <=0;
 end 
 if((D[12] || D[13]|| D[15]|| D[16]|| D[21]|| D[22]|| D[23]|| D[25]|| D[26]|| D[27]|| D[28]|| D[29]) && T[4] && Sources == 2'b00 && DSTREG[2] == 0)
                    begin
MuxBSel <= 2'b00;
ARF_FunSel <= 3'b010;
    RF_OutASel <= 3'b100;
        RF_OutBSel <= 3'b101;

case(DSTREG[1:0])
 2'b00: ARF_RegSel <= 3'b011; //PC is enabled
 2'b01: ARF_RegSel <= 3'b011; //PC is enabled
 2'b10: ARF_RegSel <= 3'b110; //SP is enabled
 2'b11: ARF_RegSel <= 3'b101; //AR is enabled
 endcase
  SC_Reset <=0;
 end
 
 //Increment operation when
 //RF <-RF+-1 Already loaded to DSTREG 
 //RF <-ARF+-1
 //ARF <- RF+-1 Already loaded to DSTREG 
 //ARF <- ARF+-1
 if(D[5] && T[3] && SREG1[2] == 1 && DSTREG[2] == 1) //RF <-RF+1
    begin
    case(DSTREG[1:0])
        2'b00: RF_RegSel <= 4'b0111;
        2'b01: RF_RegSel <= 4'b1011;
        2'b10: RF_RegSel <= 4'b1101;
        2'b11: RF_RegSel <= 4'b1110;
     endcase
     RF_FunSel <= 3'b001;
      SC_Reset <=0;
     end
  if(D[6] && T[3] && SREG1[2] == 1 && DSTREG[2] == 1) //RF <-RF-1
        begin
        case(DSTREG[1:0])
            2'b00: RF_RegSel <= 4'b0111;
            2'b01: RF_RegSel <= 4'b1011;
            2'b10: RF_RegSel <= 4'b1101;
            2'b11: RF_RegSel <= 4'b1110;
         endcase
         RF_FunSel <= 3'b000;
          SC_Reset <=0;
         end
 if(D[5] && T[3] && SREG1[2] == 1 && DSTREG[2] == 0) //ARF <-RF+1
            begin
            case(DSTREG[1:0])
                2'b00: ARF_RegSel <= 3'b011;
                2'b01: ARF_RegSel <= 3'b011;
                2'b10: ARF_RegSel <= 3'b110;
                2'b11: ARF_RegSel <= 3'b101;
             endcase
             ARF_FunSel <= 3'b001;
              SC_Reset <=0;
             end
          if(D[6] && T[3] && SREG1[2] == 1 && DSTREG[2] == 0) //ARF <-RF-1
                begin
                case(DSTREG[1:0])
                    2'b00: ARF_RegSel <= 3'b011;
                    2'b01: ARF_RegSel <= 3'b011;
                    2'b10: ARF_RegSel <= 3'b110;
                    2'b11: ARF_RegSel <= 3'b101;
                 endcase
                 ARF_FunSel <= 3'b000;
                  SC_Reset <=0;
                 end         
if((D[5] || D[6]) && T[2] && SREG1[2]==0 && DSTREG[2] ==1) //ARF <- ARF +- 1 (Loading part)
begin
    case(SREG1[1:0])
        2'b00: ARF_OutCSel <= 2'b00;
        2'b01: ARF_OutCSel <= 2'b00;
        2'b10: ARF_OutCSel <= 2'b11;
        2'b11: ARF_OutCSel <= 2'b10;
     endcase
     MuxBSel <= 2'b01;
     ARF_FunSel <= 3'b010;
     case(DSTREG[1:0])
        2'b00: ARF_RegSel <= 3'b011;
        2'b01: ARF_RegSel <= 3'b011;
        2'b10: ARF_RegSel <= 3'b110;
        2'b11: ARF_RegSel <= 3'b101;
     endcase
end     
           
if((D[5] || D[6]) && T[3] && SREG1[2]==0 && DSTREG[2] ==1) //ARF <- ARF + 1 (Incrementing or decrementing part part) 
begin           
  case(DSTREG[1:0])
   2'b00: ARF_RegSel <= 3'b011;
   2'b01: ARF_RegSel <= 3'b011;
   2'b10: ARF_RegSel <= 3'b110;
   2'b11: ARF_RegSel <= 3'b101;
    endcase
  if(D[5])begin
    ARF_FunSel <= 3'b001;
    end
   if(D[6])begin
   ARF_FunSel <= 3'b000;
    SC_Reset <=0;
   end
 end
 
 
 if((D[5] || D[6]) && T[2] && SREG1[2]==0 && DSTREG[2] == 1) //RF <- ARF +-1 (Loading part)
 begin
    case(SREG1[1:0])
     2'b00: ARF_OutCSel <= 2'b00;
     2'b01: ARF_OutCSel <= 2'b00;
     2'b10: ARF_OutCSel <= 2'b11;
     2'b11: ARF_OutCSel <= 2'b10;
  endcase
  MuxASel <= 2'b01;
  RF_FunSel <= 3'b010;
    case(DSTREG[1:0])
      2'b00: RF_RegSel <= 4'b0111;
      2'b01: RF_RegSel <= 4'b1011;
      2'b10: RF_RegSel <= 4'b1101;
      2'b11: RF_RegSel <= 4'b1110;
   endcase
   end
  if((D[5] || D[6]) && T[3] && SREG1[2]==0 && DSTREG[2] == 1)    //RF <- ARF +- 1 (Incrementing or decrementing part)
  begin
  case(DSTREG[1:0])
    2'b00: RF_RegSel <= 4'b0111;
    2'b01: RF_RegSel <= 4'b1011;
    2'b10: RF_RegSel <= 4'b1101;
    2'b11: RF_RegSel <= 4'b1110;
   endcase 
   if(D[5])begin
     RF_FunSel <= 3'b001;
     end
    if(D[6])begin
    RF_FunSel <= 3'b000;
    end
     SC_Reset <=0;
   end
   
//BRA
if(D[0] && T[2])begin
    MuxASel<= 2'b11;
    RF_FunSel <= 3'b010;
    RF_ScrSel <= 4'b0111;
    RF_OutASel <= 3'b100;
    end
if(D[0] && T[3])begin
      MuxASel<= 2'b01;
      RF_ScrSel <= 4'b1011;
      RF_FunSel <= 3'b010;
      RF_OutBSel <= 3'b101;
    end
if(D[0] && T[4])begin
    MuxBSel <= 2'b00;
    ARF_FunSel <= 3'b010;
    ARF_RegSel <= 3'b011;
     SC_Reset <=0;
    end
    
    
//BNE
if(D[1] && T[2] && Z == 0)begin
    MuxASel<= 2'b11;
    RF_FunSel <= 3'b010;
    RF_ScrSel <= 4'b0111;
    RF_OutASel <= 3'b100;
    end
if(D[1] && T[3]&& Z == 0)begin
      MuxASel<= 2'b01;
      RF_ScrSel <= 4'b1011;
      RF_FunSel <= 3'b010;
      RF_OutBSel <= 3'b101;
    end
if(D[1] && T[4]&& Z == 0)begin
    MuxBSel <= 2'b00;
    ARF_FunSel <= 3'b010;
    ARF_RegSel <= 3'b011;
     SC_Reset <=0;
    end
 
 //BEQ   
 if(D[2] && T[2]&& Z == 1)begin
        MuxASel<= 2'b11;
        RF_FunSel <= 3'b010;
        RF_ScrSel <= 4'b0111;
        RF_OutASel <= 3'b100;
        end
    if(D[2] && T[3]&& Z == 1)begin
          MuxASel<= 2'b01;
          RF_ScrSel <= 4'b1011;
          RF_FunSel <= 3'b010;
          RF_OutBSel <= 3'b101;
        end
    if(D[2] && T[4]&& Z == 1)begin
        MuxBSel <= 2'b00;
        ARF_FunSel <= 3'b010;
        ARF_RegSel <= 3'b011;
         SC_Reset <=0;
        end   
           
//MOVH
if(D[17] && T[2])
    begin
    MuxASel <= 2'b11;
    RF_FunSel <= 3'b110;
    case(RSEL)
        2'b00: RF_RegSel <= 4'b0111;
        2'b01: RF_RegSel <= 4'b1011;
        2'b10: RF_RegSel <= 4'b1101;
        2'b11: RF_RegSel <= 4'b1110;
    endcase
     SC_Reset <=0;  
  end

//MOVL
if(D[20] && T[2])
    begin
    MuxASel <= 2'b11;
    RF_FunSel <= 3'b101;
    case(RSEL)
        2'b00: RF_RegSel <= 4'b0111;
        2'b01: RF_RegSel <= 4'b1011;
        2'b10: RF_RegSel <= 4'b1101;
        2'b11: RF_RegSel <= 4'b1110;
    endcase 
     SC_Reset <=0; 
  end
 
 //POP While POP, we are going to load low the first data from memory then load high the next data(M[SP+1]) from memory. Then increment SP. So It would be like Rx <- {M[SP+1],M[SP]}
if(D[3] && T[2])begin
    ARF_FunSel <= 3'b001; // Incrementing SP
    ARF_RegSel <= 3'b110;
    ARF_OutDSel <= 2'b11;
    Mem_CS <= 0;
    Mem_WR <= 0;
    IR_Write <= 0;
    MuxASel <= 2'b10;
    RF_FunSel <= 3'b101; //Write low
    case (RSEL)
        2'b00: RF_RegSel <= 4'b0111;
        2'b01: RF_RegSel <= 4'b1011;  
        2'b10: RF_RegSel <= 4'b1101;
        2'b11: RF_RegSel <= 4'b1110;
     endcase
   end        
 if(D[3] && T[3])begin
 ARF_FunSel <= 3'b001; // Incrementing SP
 ARF_RegSel <= 3'b110;
 ARF_OutDSel <= 2'b11;
 Mem_CS <= 0;
 Mem_WR <= 0;
 IR_Write <= 0;
 MuxASel <= 2'b10;
 RF_FunSel <= 3'b110; //Write high  
  case (RSEL)
     2'b00: RF_RegSel <= 4'b0111;
     2'b01: RF_RegSel <= 4'b1011;  
     2'b10: RF_RegSel <= 4'b1101;
     2'b11: RF_RegSel <= 4'b1110;
  endcase
   SC_Reset <=0;
end 

//PSH Reverse of the POP operation. First we write the high part([15:8]) of the RSEL to the SP address in memory.Then we write the low part of the RSEL to the SP-1 address in memory.
if(D[4] && T[2])begin
    ARF_OutDSel <= 2'b11;
    ARF_FunSel <= 3'b000; //Decrement SP
    ARF_RegSel <=3'b110; 
    Mem_CS <= 0;
    Mem_WR <= 1;  
       case (RSEL)
       2'b00: RF_OutASel <= 3'b000;
       2'b01: RF_OutASel <= 3'b001;  
       2'b10: RF_OutASel <= 3'b010;
       2'b11: RF_OutASel <= 3'b011;
    endcase
    MuxCSel <= 1; //Writing the high part.
    end
 if(D[4] && T[3])begin
        ARF_OutDSel <= 2'b11;
        ARF_FunSel <= 3'b000; //Decrement SP
        ARF_RegSel <=3'b110; 
        Mem_CS <= 0;
        Mem_WR <= 1;  
           case (RSEL)
           2'b00: RF_OutASel <= 3'b000;
           2'b01: RF_OutASel <= 3'b001;  
           2'b10: RF_OutASel <= 3'b010;
           2'b11: RF_OutASel <= 3'b011;
        endcase
        MuxCSel <= 0; //Writing the low part.
         SC_Reset <=0;
        end      

//LDR 
if(D[18] && T[2])begin
    ARF_OutDSel <= 2'b10;
    ARF_FunSel <= 3'b001;
    ARF_RegSel <= 3'b101; //Incrementing AR
    Mem_CS <= 0;
    Mem_WR <= 0;
    MuxASel <= 2'b10;
    RF_FunSel <= 3'b101; // Rx[7:0] <- M[AR]
    case (RSEL)
        2'b00: RF_RegSel <= 4'b0111;
        2'b01: RF_RegSel <= 4'b1011;
        2'b10: RF_RegSel <= 4'b1101;
        2'b11: RF_RegSel <= 4'b1110;
    endcase
    end
 if(D[18] && T[3])begin
        ARF_OutDSel <= 2'b10;
        ARF_FunSel <= 3'b000;
        ARF_RegSel <= 3'b101; //Decrementing AR to keep the AR value the same as before the operation.
        Mem_CS <= 0;
        Mem_WR <= 0;
        MuxASel <= 2'b10;
        RF_FunSel <= 3'b110; // Rx[15:8] <- M[AR+1]
        case (RSEL)
            2'b00: RF_RegSel <= 4'b0111;
            2'b01: RF_RegSel <= 4'b1011;
            2'b10: RF_RegSel <= 4'b1101;
            2'b11: RF_RegSel <= 4'b1110;
        endcase
         SC_Reset <=0;
        end 
        
        
//STR
if(D[19] && T[2])begin
    RF_OutASel <= {1'b0,RSEL};
    MuxCSel <=0; //M[AR] <- Rx[7:0]
    Mem_CS <= 0;
    Mem_WR <= 1;
    ARF_OutDSel <= 2'b10;
    ARF_FunSel <= 3'b001;
    ARF_RegSel <= 3'b101; //Incrementing AR
    end
    if(D[19] && T[3])begin
        RF_OutASel <= {1'b0,RSEL};
        MuxCSel <=1; //M[AR+1] <- Rx[15:8]
        Mem_CS <= 0;
        Mem_WR <= 1;
        ARF_OutDSel <= 2'b10;
        ARF_FunSel <= 3'b000;
        ARF_RegSel <= 3'b101; //Decrementing AR to keep the AR value same.
        ARF_OutDSel <= 2'b10;
         SC_Reset <=0;
        end 
        
//BX We should store the PC into S1. Then write into memory part by part.
if(D[30] && T[2])begin
        ARF_OutCSel <= 2'b00; // Loading PC to S1
        MuxASel <= 2'b01;
        RF_FunSel <= 3'b010;
        RF_ScrSel <= 4'b0111;
        
        RF_OutASel <= {1'b0,RSEL}; // Loading Rx to PC
        MuxBSel <= 2'b00;
        ARF_FunSel <= 3'b010;
        ARF_RegSel <= 3'b011;
end 

if(D[30] && T[3])begin
    RF_OutASel <= 3'b100;
    MuxCSel <= 1; // Writing the RSEL[15:8]
    Mem_CS <= 0;
    Mem_WR <= 1;
    
    ARF_OutDSel <= 2'b11;
    
    ARF_FunSel <= 3'b000; //SP - 1
    ARF_RegSel <= 3'b110;           
end             
if(D[30] && T[4])begin
    RF_OutASel <= 3'b100;
    MuxCSel <= 0; // Writing the RSEL[7:0]
    Mem_CS <= 0;
    Mem_WR <= 1;
    
    ARF_OutDSel <= 2'b11;
    
    ARF_FunSel <= 3'b000; //SP - 1
    ARF_RegSel <= 3'b110;
    SC_Reset <=0;           
end

//BL
if(D[31] && T[2])begin
    ARF_OutDSel <= 2'b11;
    Mem_CS <= 0;
    Mem_WR <= 0;
    ARF_FunSel <= 3'b001;
    ARF_RegSel <= 3'b110; //SP + 1
    
    MuxASel <= 2'b10;
    RF_ScrSel <= 4'b0111; // Loading low to S1
    RF_FunSel <= 3'b101;
end    
if(D[31] && T[3])begin
    ARF_OutDSel <= 2'b11;
    Mem_CS <= 0;
    Mem_WR <= 0;
    ARF_FunSel <= 3'b001;
    ARF_RegSel <= 3'b110; //SP + 1
    
    MuxASel <= 2'b10;
    RF_ScrSel <= 4'b0111; // Loading high to S1
    RF_FunSel <= 3'b110;
    RF_OutASel <= 3'b100; 
end 
if(D[31] && T[4])begin
MuxBSel <= 2'b00;
ARF_RegSel <= 3'b011;
ARF_FunSel <= 3'b010; //Loading S1 into PC (PC <- {M[SP+1],M[SP]})
SC_Reset <=0;

end 

//LDRIM
if(D[32] && T[2])begin
MuxASel <= 2'b11;
RF_FunSel <= 3'b111; //Loading value from IR[7:0] to RSEL with sign extension.
case(RSEL)
    2'b00: RF_RegSel <= 4'b0111;
    2'b01: RF_RegSel <= 4'b1011;        
    2'b10: RF_RegSel <= 4'b1101;
    2'b11: RF_RegSel <= 4'b1110;
endcase
 SC_Reset <=0;
end

//STRIM
if(D[33] && T[2])begin
    ARF_OutCSel <= 2'b10;
    MuxASel <= 2'b01;
    RF_ScrSel <= 4'b0111;
    RF_FunSel <= 3'b010; //Loading AR to S1
end
if(D[33] && T[3])begin
    MuxASel <= 2'b11;
    RF_FunSel <= 3'b010;
    RF_ScrSel <= 4'b1011; //Loading OFFSET to S2
end
if(D[33] && T[4])begin
    MuxBSel <= 2'b00;
    ARF_FunSel <= 3'b010;
    ARF_RegSel <= 3'b101; //Loading AR+OFFSET to AR
end
if(D[33] && T[5])begin
    Mem_CS <= 0;
    Mem_WR <= 1;
    ALU_FunSel <= 5'b10000;
    RF_OutASel <= {1'b0,RSEL};
    
    ARF_OutDSel <= 2'b10;
    
    MuxCSel <= 0; //Writing Rx[7:0] to memory
    
    ARF_RegSel <= 3'b101;
    ARF_FunSel <= 3'b001; //Incrementing AR to get the next address for Rx[15:8]
    end
if(D[33] && T[6])begin
    Mem_CS <= 0;
    Mem_WR <= 1;
    ALU_FunSel <= 5'b10000;
    RF_OutASel <= {1'b0,RSEL};
    ARF_OutDSel <= 2'b10;
    
    MuxCSel <= 1; //Writing Rx[15:8] to memory
    
    ARF_RegSel <= 3'b101;
    ARF_FunSel <= 3'b000; //Decrementing AR to keep the value same.
    SC_Reset <=0;
end


       
  //In the last clock cycle of every operation, We changed SC_Reset to 0. With the next clock signal, We hoped that operations are completed and counter is reset.           
                 
    
 
 
 
 
 
 
      
      
                       
end



    
                 
endmodule
                 
         
         
         
         
                


   
    
    
                  
                



                 
    

                     
                    
     
            
                


module CPUSystem(
    input [0:0] Clock,
    input [0:0] Reset,
    input [7:0] T
    );
    wire [2:0] RF_OutASel;
    wire [2:0] RF_OutBSel;
    wire [2:0] RF_FunSel;
    wire [3:0] RF_ScrSel;
    wire [3:0] RF_RegSel;
    wire [4:0] ALU_FunSel;
    wire ALU_WF;
    wire [3:0] ALU_FlagsOut;
    wire MuxCSel;
    wire [1:0] MuxASel;
    wire [1:0] MuxBSel;
    wire [1:0] ARF_OutCSel;
    wire [1:0] ARF_OutDSel;
    wire [2:0] ARF_FunSel;
    wire [2:0] ARF_RegSel;
    wire IR_Write;
    wire IR_LH;
    wire [15:0] IROut;
    wire Mem_CS;
    wire Mem_WR;
    

    //There is a problem with connecting ALUSystem and controlunit.
    ArithmeticLogicUnitSystem _ALUSystem(
    .RF_OutASel(RF_OutASel),
  .RF_OutBSel(RF_OutBSel),
  .RF_FunSel(RF_FunSel),
  .RF_RegSel(RF_RegSel),
  .RF_ScrSel(RF_ScrSel),
  .ALU_FunSel(ALU_FunSel),
  .ALU_WF(ALU_WF),
  .ARF_OutCSel(ARF_OutCSel),
  .ARF_OutDSel(ARF_OutDSel),
  .ARF_FunSel(ARF_FunSel),
  .ARF_RegSel(ARF_RegSel),
  .IR_LH(IR_LH),
  .IR_Write(IR_Write),
  .Mem_WR(Mem_WR),
  .Mem_CS(Mem_CS),
  .MuxASel(MuxASel),
  .MuxBSel(MuxBSel),
  .MuxCSel(MuxCSel),
  .Clock(Clock));
  
 control_unit _controlunit(
.Clock(Clock), 
.IROut(_ALUSystem.IROut), 
.ARF_FunSel(ARF_FunSel),
.ARF_RegSel(ARF_RegSel),
.ARF_OutCSel(ARF_OutCSel),
.ARF_OutDSel(ARF_OutDSel),
.ALU_WF(ALU_WF),
.ALU_FlagsOut(ALU_FlagsOut),
.ALU_FunSel(ALU_FunSel),
.RF_OutASel(RF_OutASel),
.RF_OutBSel(RF_OutBSel),
.RF_FunSel(RF_FunSel),
.RF_RegSel(RF_RegSel),
.RF_ScrSel(RF_ScrSel),
.MuxASel(MuxASel),
.MuxBSel(MuxBSel),
.MuxCSel(MuxCSel),
.Mem_CS(Mem_CS),
.Mem_WR(Mem_WR),
.IR_LH(IR_LH),
.IR_Write(IR_Write),
.ResetFromOut(Reset));
  
    
    
endmodule
