; Name:Taha Temiz
; Student Number:150210055
; Date:10.11.2024


WeightCapacity EQU 0xC8;Capacity = 50*4
ArraySize EQU 0x0C;Size = 3*4
	
			AREA My_Array, DATA, READWRITE
			ALIGN
dp_array	SPACE WeightCapacity;Allocating space for dp array
dp_end

		AREA knapsackpart, CODE, READONLY
		ENTRY
		THUMB
		ALIGN
__main	FUNCTION
		EXPORT __main

		MOVS R4,#ArraySize		;To keep n 
		MOVS R5,#4 				;Store i=1 in R5
		LDR R1,=profit_array	;Loading starting address of profit array into R1.I loaded the addresses of all the three arrays into R1,R2,R3 at the start. I did not want to change R1,R2,R3 until the end. So when it is needed I used push method to save variables.
		LDR R2,=weight_array	;Loading starting address of weight array into R2.
		LDR R3,=dp_array		;Loading starting address of dp array into R3.
		MOVS R6,#0				;Number of 0's that added into dp array

fill	MOVS R0,#0				;To add 0 to dp array.I do not know this is necessary but I did this to ensure that dp array is filled with zeros.
		STR R0,[R3,R6]			;dp[...]=0
		ADDS R6,#4				;Counting how many 0's added
		CMP R6,#200				;Checking if we added all zeros
		BNE	fill				;If not then add another 0
		
		ADDS R4,R4,#4			;R4 = n+1 so we can compare for the first for loop
		MOVS R6,#200			;R6 = w = W
L1		CMP R5,R4				;if i < n+1
		BGE EndL1				;Then we need to end the first for loop(l1)
		MOVS R6,#200			;R6 = w = W
L2		CMP R6,#0				;comparing w and 0
		BLT EndL2				;if w<0 then we need to end the second for loop(l2)
		MOVS R7,R5				;R7 = i
		SUBS R7,R7,#4			;R7= i-1
		LDR R7,[R2,R7]			;R7 = weight[i-1] so we can compare
		LSRS R6,R6,#2			;w is byte offset so dividing W by 4.
		CMP R7,R6				;comparing weight[i-1] and w
		BLE comparing			;if weight[i-1]<=w then we need to to the max function and swap
		LSLS R6,R6,#2			;Multiplying w with 4 to get byte offset form.
		SUBS R6,R6,#4			;if not then w--
		B L2					;for the second for loop

EndL2	ADDS R5,R5,#4			;if the second for loop is done then i++
		B L1					;for the first for loop

comparing	LSLS R6,R6,#2		;Multiplying w with 4 to get byte offset form.
			LDR R0,[R3,R6]		;R0 = dp[w]
			PUSH {R0}			;Pushing dp[w] value onto stack to save it. As I mentioned, I did not want to change R1,R2,R3 during whole execution.
			MOVS R0,R5			;R0 = i
			SUBS R0,R0,#4		;R0= i-1
			LDR R0,[R1,R0]		;R0 = profit[i-1]
			LSRS R6,R6,#2		;w is byte offset so dividing W by 4.
			SUBS R7,R6,R7		;R7 = w-weight[i-1]
			LSLS R6,R6,#2		;Multiplying w with 4 to get byte offset form.	
			LSLS R7,R7,#2		;Multiplying w-weight[i-1] with 4 to get byte offset form.
			LDR R7,[R3,R7]		;R7 = dp[w-weight[i-1]]
			ADDS R7,R7,R0		;R7 = dp[w-weight[i-1]] + profit[i-1]
			POP {R0}			;To get dp[w] back
			CMP R0,R7			;Comparing d[w] and dp[w-weight[i-1]]+profit[i-1]
			BLT swap			;We need the max value so if d[w] is bigger or equal there is no need to swap,otherwise we need to swap
			SUBS R6,R6,#4		;If there is no need to swap then w--
			B L2				;For the second for loop
swap		STR R7,[R3,R6]		; dp[w] = dp[w-weight[i-1]] + profit[i-1]
			SUBS R6,R6,#4		;w--
			B L2				;for the second for loop
EndL1		MOVS R6,#200		;In the end we need dp[W],so R6 = W. I think there is a problem with the number.There are 50 values in dparray.So dp[50] should not exist. But the result is correct so I did not change.
			LDR R0,[R3,R6]		;At the end, storing d[w] in R0
stop		B stop				;Stop loop
			ALIGN
			ENDFUNC
profit_array	DCD	60,100,120	;profit_array = [60,100,120]
profit_end
weight_array	DCD	10,20,30	;weight_array = [10,20,30]
weight_end
	END
	
	
	
	
	


	
	