/*
 * File:	cw_crt0.s
 * Purpose:	Lowest level routines for Kinetis.
 *
 * Notes:	This is a CodeWarrior specific version of crt0.s
 *
 */

	.extern start
	
	.global startup
	.global __startup

	.text
	
startup:
__startup:
    MOV     r0,#0                   // Initialize the GPRs
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
	CPSIE   i                       // Unmask interrupts
    BL      start                  // call the C code
done:
        B       done

	.end
