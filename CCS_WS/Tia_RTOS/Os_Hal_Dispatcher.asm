
 ;                                           <Os_Dispatcher>
 ; In : None
 ; Out: None
 ; return: None
 ; description: If a higher-priority task is ready, the internal state will be prepared to 
 ;              perform the context switch or the task state will be changed and linked to a task
 ;              parent.
 ; 
 ; Usage: This function will be triggered as a sys Os interrupt from the application level (SetEvent)
 ;        or from the Os tick timer. 
 ;          (The context switch will take place at the end of this function if needed).

	.global Os_GlobalTaskCtxInfo
	.global Os_Dispatcher
	.global Os_SVC_Handler

.rodata
	.align 2
ARM_M7_VECTOR_TABLE_OFFSET: .word 0xE000ED08
OS_GLOBAL_TASK_CTX_PTR:	.word Os_GlobalTaskCtxInfo 	;Os Main var.

.text

Os_Dispatcher:

    CPSID         I  			 ;Disable the global interrupt.
	
	LDR		R0, OS_GLOBAL_TASK_CTX_PTR ;Load OS MAIN STRUCT to R0

	LDR		R1, [R0]
	LDR		R2, [R0, #4]
	CMP     R1, R2
    BEQ     ret				  	 ; if (CurrentTask == NextTask)RETURN.

	LDRB  R3, [R1, #4]			 ;Current task state
	CMP	  R3, #0xAA
	BNE	  ctx_switch			 ;if the task is not running 'Suspended / Waiting !!'

	MOV   R3, #0XAC 			 ;Change the current task state to ready.
	STRB  R3, [R1, #4]

ctx_switch:
; Save the current context information..
	MRS    R12, PSP				;R12=PSP
	STMDB  R12!, {r4-r11}		;PUSH   {r4-r11}
	STR    R12, [R1]			;CURRENT_TASK_SP=R12

; Restore the new task context information..
	LDR		R12, [R2]			;R12=TASK_SP
	LDMIA	R12!, {r4-r11}		;POP 	{r4-r11}
	MSR 	PSP, R12			;PSP=R12

; Current task ptr = Next Task ptr.
	STR 	R2, [R0]

; Change next task state to running..
	MOV	  R3, #0XAA
	STRB  R3, [R2, #4]			 

ret:
	movs       r0, #6

	CPSIE  I  					 ; Enable the global interrupt.
	BX     LR 					 ; return using the link register..


; Supervisor call :
;					Used when the application needs something from the Os.
; For the First call: 'StartOs'
;		* Use the vector table offset register to get the stack. 
;		* Clear the old MSP "As this function shall not return to anywhere
;		* Activate/Run the first task. "Idle_Task|Init_Task"

; Extract the SVC service number
;TST	LR, #4
;MRSEQ	R1, MSP
;MRSNE	R1, PSP
;LDR	R1, [R1, #24]
;LDRB	R1, [R1, #-2]
Os_SVC_Handler:
;Clear the MSP 'Set the MSP to the vector table offset'.
	LDR.W R0,  ARM_M7_VECTOR_TABLE_OFFSET
	LDR R0, [R0]
	LDR R0, [R0]
	MSR MSP, R0 

	LDR		R0, OS_GLOBAL_TASK_CTX_PTR ;Load OS MAIN STRUCT to R0

	LDR		R2, [R0, #4]	; NEXT_TASK_PTR

; Restore the new task context information..
	LDR		R12, [R2]			;R12=TASK_SP
	LDMIA	R12!, {r4-r11}		;POP 	{r4-r11}
	MSR 	PSP, R12			;PSP=R12

; Current task ptr = Next Task ptr.
	STR 	R2, [R0]

; Change next task state to running..
	MOV	  R3, #0XAA
	STRB  R3, [R2, #4]			 

	ORR   LR, LR, #13 			 ;Return to Thread mode.

	BX     LR 					 ; return using the link register..
