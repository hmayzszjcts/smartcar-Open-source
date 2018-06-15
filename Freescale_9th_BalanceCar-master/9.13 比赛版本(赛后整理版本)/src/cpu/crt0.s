/*
; * File:	crt0.s
; * Purpose:	Lowest level routines for Kinetis.
 *
 * Notes:	
 *
 */

;         AREA   Crt0, CODE, READONLY      ; name this block of code



  	SECTION .noinit : CODE
  	EXPORT  Reset_Handler
Reset_Handler                           ;本来是命名为 __startup 的，改为 Reset_Handler  需要修改 vectors.h

    CPSIE   i                       ; Unmask interrupts       关中断  
    MOV     r0,#0                   ; Initialize the GPRs     初始化寄存器
	MOV     r1,#0
	MOV     r2,#0
	MOV     r3,#0
	MOV     r4,#0
	MOV     r5,#0
	MOV     r6,#0
	MOV     r7,#0
	MOV     r8,#0
	MOV     r9,#0
	MOV     r10,#0
	MOV     r11,#0
	MOV     r12,#0
	
    import start
    BL      start               ; call the C code       调用 C语言 编写的 start 函数
__done
        B       __done


        END
