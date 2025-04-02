;		TAHA TEMIZ 150210055 31.12.2024
      	AREA    Timing_Code, CODE, READONLY
        ALIGN
        THUMB
        EXPORT  Systick_Start_asm
        EXPORT  Systick_Stop_asm
		EXPORT	SysTick_Handler
		EXTERN	ticks
		EXTERN SystemCoreClock	;externing systemcoreclock to calculate reload value

SysTick_Handler FUNCTION
		PUSH	{LR}
		LDR R0, =ticks ;Loading the address of ticks to R0
		LDR R1,[R0]		;Loading the current ticks value to R1
		ADDS R1,R1,#1	;Incrementing current ticks value
		STR R1,[R0]		;Storing incremented ticks value to address of ticks
		POP		{PC}
		ENDFUNC

Systick_Start_asm FUNCTION
		PUSH	{LR}
		LDR R0, =ticks	;Loading the address of ticks to R0
		MOVS R1, #0		;R1 = 0
		STR R1,[R0]		;ticks = 0

		LDR R3,=100000				;R5=100000
		MOVS R2,#0					;R2=0, R2 will keep the dividing result
		LDR R1, =SystemCoreClock	;Loading the address of systemcoreclock into R1
		LDR R1,[R1]					;R1 = SystemCoreClock
loop	CMP R1,#0					;Checking if R1 is less than 0(Because we are dividing R1 by 100000.We are basically substracting 100000 from SystemCoreClock until it gets zero)
		BEQ enddiv					;If it is zero then dividing is completed
		SUBS R1,R1,R3				;Substracting 100000 from R1
		ADDS R2,#1					;Keeping the count of substracting to get the dividing result
		B loop						;Go back to the loop
enddiv	SUBS R2,#1					;If the dividing operation is ended then R2 = (SystemCoreClock / 100000) - 1 = Reload Value
		LDR	 R0, =0xE000E014		;Loading the SYST_RVR address to R0 
		STR  R2, [R0]				;Storing the reload value(R2) in reload value register
		

		LDR R0, =0xE000E018					;Loading SYST_CVR address to R0
		MOVS R1,#0							;R1 =0
		STR R1,[R0]							;Clear the current value register

		LDR	 R0, =0xE000E010				; Loading the SYST_CSR address to R0
		LDR  R1, [R0]						; Loading Control and Status Register to R1.
	    MOVS R2, #7							; R2 = 7 (111)
		ORRS R1, R1, R2						; Setting CLKSOURCE, TICKINT, and ENABLE flags.	
		STR  R1, [R0]						; Setting the Reload Value Register to enable interrupt and timer.

        POP		{PC}
		ENDFUNC

Systick_Stop_asm FUNCTION
		PUSH	{LR}						
		LDR	 R0, =0xE000E010				; Loading SYST_CSR Address.
		LDR  R1, [R0]						; Loading Control and Status Register to R1.
		LDR  R2, =0xFFFFFFFC				; MOVS Mask value to R2.
		ANDS R1, R1, R2						; Clear TICKINT and ENABLE flags.
		STR  R1, [R0]						; Store the new register value.	
		LDR R0, =ticks						;Loading the address of ticks to R0
		LDR R1,[R0]							;Keeping the current ticks value in R1
		MOVS R2,#0							;r2 =0
		STR R2,[R0]							;Clearing the ticks
		MOVS R0,R1							;Storing saved ticks value in R0 to return
		POP		{PC}
		ENDFUNC
		END
		
