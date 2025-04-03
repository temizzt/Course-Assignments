`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// 150210055 Taha Temiz
// 150210060 Mehmet Mert Ergin
//////////////////////////////////////////////////////////////////////////////////


module ArithmeticLogicUnit(A,B,FunSel,WF,Clock,ALUOut,FlagsOut);
    input wire [15:0] A;
    input wire [15:0] B;
    input wire [4:0] FunSel;
    input wire WF;
    input wire Clock;
    output reg [15:0] ALUOut;
    output reg [3:0] FlagsOut;
    reg Z = 0;
    reg C = 0;
    reg N = 0;
    reg O = 0;
    
    reg [15:0]temp;
    
    always@(*)	
    begin 
	{Z,C,N,O}<= FlagsOut; 
        case(FunSel)
        5'b00000: ALUOut = {8'd0,{A[7:0]}};
                
        5'b00001: ALUOut = {8'd0,{B[7:0]}};
        
        5'b00010: ALUOut = {8'd0,{~A[7:0]}};
        
        5'b00011: ALUOut = {8'd0,{~B[7:0]}};
        
        5'b00100:
        begin 
            ALUOut = {8'd0,A[7:0]} + {8'd0,B[7:0]};
            ALUOut = {8'd0,{ALUOut[7:0]}};
        end
           
        5'b00101:
        begin
            ALUOut = {8'd0,A[7:0]} + {8'd0,B[7:0]}+ {15'd0,FlagsOut[2]};
            ALUOut = {8'd0,{ALUOut[7:0]}};
        end
            
        5'b00110:
        begin
            ALUOut = {8'd0,A[7:0]} + {8'd0,~B[7:0]} + 16'd1;
            ALUOut = {8'd0,{ALUOut[7:0]}};
        end
            
        5'b00111:
        begin
            ALUOut = {8'd0,A[7:0]} & {8'd0,B[7:0]};
            ALUOut = {8'd0,{ALUOut[7:0]}};
        end
            
        5'b01000:
        begin
            ALUOut = {8'd0,A[7:0]} | {8'd0,B[7:0]};
            ALUOut = {8'd0,{ALUOut[7:0]}};
        end
            
        5'b01001:
        begin
            ALUOut = {8'd0,A[7:0]} ^ {8'd0,B[7:0]};
            ALUOut = {8'd0,{ALUOut[7:0]}};
        end
            
        5'b01010:
        begin
            ALUOut = ~({8'd0,A[7:0]} & {8'd0,B[7:0]});
            ALUOut = {8'd0,{ALUOut[7:0]}};
        end
            
        5'b01011:
        begin
            ALUOut = A;
            ALUOut = ALUOut << 1;
            ALUOut = {8'd0,{ALUOut[7:0]}};
        end
            
        5'b01100:
        begin
            ALUOut = A;
            ALUOut = ALUOut >> 1;
            ALUOut = {8'd0,{ALUOut[7:0]}};
        end
            
        5'b01101:
        begin
            ALUOut = A;
            ALUOut = ALUOut >> 1;
            ALUOut[7] = ALUOut[6];
            ALUOut = {8'd0,{ALUOut[7:0]}};
        end
            
        5'b01110:
        begin
		ALUOut = {8'd0,A[6:0],FlagsOut[2]};
        end
           
        5'b01111:
        begin
            ALUOut = {8'd0,FlagsOut[2],A[7:1]};
        end
            
        
        5'b10000: ALUOut = A;
        
        5'b10001: ALUOut = B;
        
        5'b10010: ALUOut = ~A;
        
        5'b10011: ALUOut = ~B;
        
        5'b10100: ALUOut = A + B;
        
        5'b10101: ALUOut = A + B + FlagsOut[2];
        
        5'b10110: ALUOut = A + ~B + 16'd1;
        
        5'b10111: ALUOut = A & B;
        
        5'b11000: ALUOut = A | B;
        
        5'b11001: ALUOut = A ^ B;
        
        5'b11010: ALUOut = ~(A & B);
        
        5'b11011:
            begin
                ALUOut = A;
                ALUOut = ALUOut << 1;
            end
                        
        5'b11100:
            begin
                ALUOut = A;
                ALUOut = ALUOut >> 1;
            end
                
        5'b11101:
            begin
                ALUOut = A;
                ALUOut = ALUOut >> 1;
                ALUOut[15] = ALUOut[14];
            end 
               
        5'b11110:
           begin
            ALUOut = {A[14:0],FlagsOut[2]};
            end

                
        5'b11111:
        begin
            ALUOut = {FlagsOut[2],A[15:1]};
        end
        endcase
      end  
      
     always @(posedge Clock)
     begin
        if(WF)
            begin
                case(FunSel)
                    5'b00000: N = ALUOut[7];
                    
                    5'b00001: N = ALUOut[7];
                    
                    5'b00010: N = ALUOut[7];
                    
                    5'b00011: N = ALUOut[7];
                    
                    5'b00100:
                    begin
                        O = 0;
                        N = ALUOut[7];
                        temp = {8'b0,A[7:0]} + {8'b0,B[7:0]};
                        C = temp[8];
                        if ((A[7] == B[7]) && (A[7] != ALUOut[7]))
                        begin
                        O = 1;
                        end
                    end
                    
                    5'b00101:
                    begin
                        O = 0;
                        N = ALUOut[7];
                        temp = {8'd0,A[7:0]} + {8'd0,B[7:0]} + {15'd0,FlagsOut[2]};
                        C = temp[8];
                        if ((A[7] == B[7]) && (A[7] != ALUOut[7]))
                        begin
                        O = 1;
                        end
                     end
                     
                     5'b00110:
                     begin
                        O = 0;
                        N = ALUOut[7];
                        temp = {8'd0,A[7:0]} + {8'd0,~B[7:0]} + 16'd1;
                        C = temp[8];
                        if((B[7] == ALUOut[7]) && (B[7] != A[7]))
                        begin
                        O = 1;
                        end
                     end
                     
                     5'b00111: N = ALUOut[7];
                     
                     5'b01000: N = ALUOut[7];
                     
                     5'b01001: N = ALUOut[7];
                     
                     5'b01010: N = ALUOut[7];
                     
                     5'b01011:
                     begin
                        N = ALUOut[7];
                        C = A[7];
                     end
                     
                     5'b01100:
                     begin
                        N = ALUOut[7];
                        C = A[0];
                     end
                     
                     5'b01101: C = A[0];
                     
                     5'b01110:
                     begin
                        N = ALUOut[7];
                        C = A[7];
                     end   
                     
                     5'b01111:
                     begin
                        N = ALUOut[7];
                        C = A[0];
                     end
                     
                     5'b10000: N = ALUOut[15];
                     5'b10001: N = ALUOut[15];
                     5'b10010: N = ALUOut[15];
                     5'b10011: N = ALUOut[15];
                     
                     5'b10100:
                     begin
                        O = 0;
                        N = ALUOut[15];
                        {C,ALUOut} = {1'b0,A} + {1'b0,B};
                        if((A[15] == B[15]) && (A[15] != ALUOut[15]))
                        begin
                        O = 1;
                        end
                     end
                     
                     5'b10101:
                     begin
                        O = 0;
                        N = ALUOut[15];
                        {C,ALUOut} = {1'b0,A} + {1'b0,B} + FlagsOut[2];
                        if((A[15] == B[15]) && (A[15] != ALUOut[15]))
                        begin
                        O = 1;
                        end
                     end
                     
                     5'b10110:
                     begin
                        O = 0;
                        N = ALUOut[15];
                        {C,ALUOut} = {1'b0,A} + {1'b0,~B} + 16'd1;
                        if ((B[15] == ALUOut[15]) && (B[15] != A[15]))
                        begin
                        O = 1;
                        end
                     end
                     
                     5'b10111: N = ALUOut[15];
                     
                     5'b11000: N = ALUOut[15];
                     
                     5'b11001: N = ALUOut[15];
                     
                     5'b11010: N = ALUOut[15];
                     
                     5'b11011:
                     begin
                        N = ALUOut[15];
                        C = A[15];
                     end
                     
                     5'b11100:
                     begin
                        N = ALUOut[15];
                        C = A[0];
                     end
                     
                     5'b11101: C = A[0];
                     
                     5'b11110:
                     begin
                        N = ALUOut[15];
                        C = A[15];
                     end
                     
                     5'b11111:
                     begin
                        N = ALUOut[15];
                        C = A[0];
                     end
                    default: FlagsOut <= FlagsOut;
                    endcase
                    
                    if(ALUOut == 16'd0)
                    begin
                    Z = 1;
                    end
                    else
                    begin
                    Z = 0;
                    end
              end
		else
		begin
		FlagsOut <= FlagsOut;
		end

              
                      FlagsOut <= {Z,C,N,O};            
              end                  
                                               
                              
                     
                     
                             
        
endmodule
