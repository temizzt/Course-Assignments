; Function: ft_lstsort_asm
; Parameters:
;   R0 - Pointer to the list (address of t_list *)
;   R1 - Pointer to comparison function (address of int (*f_comp)(int, int))
;
;	TAHA TEMIZ 150210055 31.12.2024
;
        AREA    Sorting_Code, CODE, READONLY
        ALIGN
        THUMB
        EXPORT  ft_lstsort_asm
		EXTERN	ft_lstlast_asm

ft_lstsort_asm FUNCTION
		PUSH	{R0-R1, LR}

		LDR R2,[SP]			;R2 = address of the start of the list			
		LDR R0,[R2]			;R2 = address of the first element
		PUSH {R0}			;Input for finding the last element
		BL ft_lstlast_asm	;To get the last element
		POP {R2}			;To keep the stack clean organised
		SUBS R3,R0,R2		;r3=last address-first address		
		ASRS R3,#2			;to get the arr.size dividing by 4
		MOVS R4,#0			;bool swapped = false
		MOVS R5,#0			;int i =0

loop1	CMP R5,R3			;checking if i < n
		BEQ endloop1		;if not then end loop1
		MOVS R4,#0			;swapped = false
		MOVS R6,#0			;j=0
		SUBS R7,R3,R5		;R7 = i - n
loop2	CMP R6,R7			;checking if j < n - i 
		BEQ endloop2		;if not then end loop2
		LSLS R6,#2			;offseting R6
		LDR R2,[SP]			;R2 = pointer to the list
		LDR R2,[R2]			;R2 = address of the first element
		LDR R0,[R2,R6]		; j st element is loaded to R0
		ADDS R6,#8			;to get the next element
		LDR R1,[R2,R6]		;j + 1 st element is loaded to R1
		PUSH{R3}			;To save the current R3 value
		LDR R3,[SP,#8]		;R3 = cmp function address

		PUSH{R0}			;argument 1 = arr[j]
		PUSH{R1}			;argument 2 = arr[j+1]
		SUBS R6,#8			;converting R6 
		ASRS R6,#2			;R6 = j

		BLX R3				;call cmp function

		CMP R0,#0			;result is in R0 so check r0
		BEQ swap			;if j[i] > j[i+1] then swap
		POP{R0}				;to keep the stack organized
		POP{R0}				;to keep the stack organized
		POP{R3}				;R3 = R3 value before the compare function
		
		ADDS R6,#2			;j +=1
		B loop2				;repeat loop2
swap	POP{R0}				;to keep stack organized
		POP{R0}				;to keep stack organized
		POP{R3}				;R3 = R3 value before the compare function
		LDR R0,[SP]			;R0 = pointer to the list
		LDR R0,[R0]			;R0= address of the first element
		LSLS R6,#2			;offsetting R6 to get the value
		LDR R1,[R0,R6]		;R1 = j st element
		ADDS R6,#8			;R6 + 8 to get the data of the next node
		LDR R2,[R0,R6]		;R2 = j+1 st element
		STR R1,[R0,R6]		;j+1 st element = R1
		SUBS R6,#8			;R6-8 to keep the R6 value the same
		STR R2,[R0,R6]		;j st element = R2. So they are swapped
		ASRS R6,#2			;converting r6 to index form
		MOVS R4,#1			;swapped = true
		ADDS R6,#2			;j +=1
		B loop2				;repeat loop2
endloop2	CMP R4,#0		;if no element swapped
			BEQ endloop1	;then end the function
			ADDS R5,#2		;if there is swap then i = i+1
			B loop1			;go back to loop1
endloop1	NOP				;do nothing.I got some errors so I added this part.
			
		POP		{R0-R1, PC}
		ENDFUNC
