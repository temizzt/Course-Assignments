; Name:Taha Temiz
; Student Number:150210055
; Date:10.11.2024


Capacity EQU 0x32		;Capacity = 50
Size	EQU 0x3			;Size = 3

	AREA knapsackrecursive, CODE,READONLY
	ENTRY
	THUMB
	ALIGN
__main	FUNCTION
		EXPORT __main
		MOVS R3,#Capacity		;R3 = W = 50
		MOVS R4,#Size			;R4 = n = 3			
		PUSH {R3,R4}			;Pushing first inputs onto stack
		BL knapsack				;Going into function with (50,3) inputs
		
		LDR R1,=profit_array	;Loading the start address of profit array to R1
		LDR R2,=weight_array	;Loading the start address of weight array to R2
		B stop					;Go into stop loop



knapsack	POP {R3,R4}		;Getting inputs W and n
			PUSH {LR}		;To get back where we got here from
			CMP R3,#0		;Checking if W == 0
			BEQ Zero
			CMP R4,#0		;Checking if n ==0
			BEQ Zero
			
			SUBS R5,R4,#1	;R5 = n-1
			LSLS R5,R5,#2	;Multiplying with 4 because of 4bytes of space
			LDR R6,=weight_array	;R6 = starting of weight array
			LDR R6,[R6,R5]		;R6 = weight[n-1]
			LSRS R5,R5,#2		;Converting R5 back to integer form
			CMP R6,R3			;Comparing weight[n-1] with W
			BLE elsecond		;If weight[n-1] <= W then we need to go else condition
			PUSH {R3,R5}		;If not then push W and n-1 as inputs into knapsack function
			BL knapsack			;Calling knapsack function for W and n-1 inputs.Because they are on the top of the stack.
			POP {PC}			;When we come back from a knapsack function then there is a returning address on the top of the stack.So we go back where we came from.

Zero		MOVS R0,#0			;If n or W is zero then return 0
			POP {PC}			;Go back where we come from

elsecond	LSLS R5,R5,#2		;Multiplying n-1 with 4 because it will be used as index
			SUBS R1,R3,R6		;R1 = W-weight[n-1]
			LDR R6,=profit_array	;R6 = starting adress of profit array
			LDR R6,[R6,R5]			;R6 = profit[n-1]
			LSRS R5,R5,#2			;Converting R5 to number
			PUSH {R6}				;profit[n-1] will be needed so we push it.I think it is not a problem because our function does not pop unnecessarily so profit[n-1] is safe.
			PUSH {R1,R5}			;W-weight[n-1],n-1 as inputs for knapsack.We are pushing two times.Because we need to save the values
			PUSH {R3,R5}			;W,n-1 as inputs for knapsack
			BL knapsack				;To get the value of knapSack(W,n-1)
			MOVS R6,R0				;The result is in R0 so R6 = knapSack(W,n-1)
			BL knapsack				;To get the value of knapSack(W-weight[n-1],n-1)
			MOVS R7,R0				;The result is in R0 so R7=knapSack(W-weight[n-1],n-1)
			POP {R1}				;If the program came this line then it means that the two input pairs that we pushed onto stack are popped so profit[n-1] is on the top of the stack.R1 = profit[n-1] from the stack
			ADDS R7,R7,R1			;R7 = profit[n-1] + knapSack(W-weight[n-1],n-1)
			CMP R6,R7				;comparing R6 and R7 to choose max.I did not create a function to decide which value is max.
			BLT swap				;if knapSack(W,n-1) is less than profit[n-1] +knapSack(W-weight[n-1],n-1) go swap
			MOVS R0,R6				;if not then return knapSack(W,n-1)
			POP {PC}				;go back to where we came from

swap		MOVS R0,R7				;return profit[n-1] + knapSack(W - weight[n-1],n-1)
			POP {PC}				;go back to where we came from
			
			
stop B stop							;stop loop
	ALIGN
	ENDFUNC




weight_array DCD 10,20,30
weight_end
profit_array DCD 60,100,120
profit_end
			END	

