;/*****************************************************************************
; * @file:    startup_SKEAZ1284.s
; * @purpose: CMSIS Cortex-M0plus Core Device Startup File for the
; *           SKEAZ1284
; * @version: 1.2
; * @date:    2014-1-10
; *
; * Copyright: 1997 - 2014 Freescale Semiconductor, Inc. All Rights Reserved.
;*
; *------- <<< Use Configuration Wizard in Context Menu >>> ------------------
; *
; *****************************************************************************/


; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00000100

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000000

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp  ; Top of Stack
                DCD     Reset_Handler  ; Reset Handler
                DCD     NMI_Handler  ; NMI Handler
                DCD     HardFault_Handler  ; Hard Fault Handler
                DCD     0  ; Reserved
                DCD     0  ; Reserved
                DCD     0  ; Reserved
                DCD     0  ; Reserved
                DCD     0  ; Reserved
                DCD     0  ; Reserved
                DCD     0  ; Reserved
                DCD     SVC_Handler  ; SVCall Handler
                DCD     0  ; Reserved
                DCD     0  ; Reserved
                DCD     PendSV_Handler  ; PendSV Handler
                DCD     SysTick_Handler  ; SysTick Handler

                ; External Interrupts
                DCD     Reserved16_IRQHandler  ; Reserved interrupt
                DCD     Reserved17_IRQHandler  ; Reserved interrupt
                DCD     Reserved18_IRQHandler  ; Reserved interrupt
                DCD     Reserved19_IRQHandler  ; Reserved interrupt
                DCD     Reserved20_IRQHandler  ; Reserved interrupt
                DCD     FTMRE_IRQHandler  ; Command complete and read collision
                DCD     PMC_IRQHandler  ; Low-voltage detect, low-voltage warning
                DCD     IRQ_IRQHandler  ; External Interrupt
                DCD     I2C0_IRQHandler  ; I2C0 interrupt
                DCD     I2C1_IRQHandler  ; I2C1 interrupt
                DCD     SPI0_IRQHandler  ; SPI0 single interrupt vector for all sources
                DCD     SPI1_IRQHandler  ; SPI1 single interrupt vector for all sources
                DCD     UART0_IRQHandler  ; UART0 status and error
                DCD     UART1_IRQHandler  ; UART1 status and error
                DCD     UART2_IRQHandler  ; UART2 status and error
                DCD     ADC_IRQHandler  ; ADC interrupt
                DCD     ACMP0_IRQHandler  ; ACMP0 interrupt
                DCD     FTM0_IRQHandler  ; FTM0 single interrupt vector for all sources
                DCD     FTM1_IRQHandler  ; FTM1 single interrupt vector for all sources
                DCD     FTM2_IRQHandler  ; FTM2 single interrupt vector for all sources
                DCD     RTC_IRQHandler  ; RTC overflow
                DCD     ACMP1_IRQHandler  ; ACMP1 interrupt
                DCD     PIT_CH0_IRQHandler  ; PIT CH0 overflow
                DCD     PIT_CH1_IRQHandler  ; PIT CH1 overflow
                DCD     KBI0_IRQHandler  ; Keyboard interrupt0(32bit)
                DCD     KBI1_IRQHandler  ; Keyboard interrupt1(32bit)
                DCD     Reserved42_IRQHandler  ; Reserved interrupt
                DCD     ICS_IRQHandler  ; ICS interrupt
                DCD     WDOG_IRQHandler  ; Watchdog timeout
                DCD     PWT_IRQHandler  ; PWT interrupt
                DCD     MSCAN_RX_IRQHandler  ; MSCAN Rx Interrupt
                DCD     MSCAN_TX_IRQHandler  ; MSCAN Tx, Err and Wake-up interrupt
__Vectors_End

__Vectors_Size 	EQU     __Vectors_End - __Vectors

; <h> Flash Configuration
;   <i> 16-byte flash configuration field that stores default protection settings (loaded on reset)
;   <i> and security information that allows the MCU to restrict acces to the FTFL module.
;   <h> Backdoor Comparison Key
;     <o0>  Backdoor Key 0  <0x0-0xFF:2>
;     <o1>  Backdoor Key 1  <0x0-0xFF:2>
;     <o2>  Backdoor Key 2  <0x0-0xFF:2>
;     <o3>  Backdoor Key 3  <0x0-0xFF:2>
;     <o4>  Backdoor Key 4  <0x0-0xFF:2>
;     <o5>  Backdoor Key 5  <0x0-0xFF:2>
;     <o6>  Backdoor Key 6  <0x0-0xFF:2>
;     <o7>  Backdoor Key 7  <0x0-0xFF:2>
BackDoorK0      EQU     0xFF
BackDoorK1      EQU     0xFF
BackDoorK2      EQU     0xFF
BackDoorK3      EQU     0xFF
BackDoorK4      EQU     0xFF
BackDoorK5      EQU     0xFF
BackDoorK6      EQU     0xFF
BackDoorK7      EQU     0xFF
;   </h>
;     <h> FPROT
;       <i> P-Flash Protection Register
;       <o.7> FPOPEN
;         <0=> FPHDIS and FPLDIS bits define unprotected address ranges as specified by the corresponding FPHS and FPLS bits FPROT1.1
;         <1=> FPHDIS and FPLDIS bits enable protection for the address range specified by the corresponding FPHS and FPLS bits
;       <o.5> FPHDIS
;         <0=> Protection/Unprotection enabled
;         <1=> Protection/Unprotection disabled
;       <o.3..4> FPHS
;         <0=> Address range: 0x00_7C00-0x00_7FFF; protected size: 1 KB
;         <1=> Address range: 0x00_7800-0x00_7FFF; protected size: 2 KB
;         <2=> Address range: 0x00_7000-0x00_7FFF; protected size: 4 KB
;         <3=> Address range: 0x00_6000-0x00_7FFF; protected size: 8 KB
;       <o.5> FPLDIS
;         <0=> Protection/Unprotection enabled
;         <1=> Protection/Unprotection disabled
;       <o.3..4> FPLS
;         <0=> Address range: 0x00_0000-0x00_07FF; protected size: 2 KB
;         <1=> Address range: 0x00_0000-0x00_0FFF; protected size: 4 KB
;         <2=> Address range: 0x00_0000-0x00_1FFF; protected size: 8 KB
;         <3=> Address range: 0x00_0000-0x00_3FFF; protected size: 16 KB
FPROT         EQU     0xFF
;     </h>
;   </h>
;   <h> Flash security byte (FSEC)
;     <i> WARNING: If SEC field is configured as "MCU security status is secure" and MEEN field is configured as "Mass erase is disabled",
;     <i> MCU's security status cannot be set back to unsecure state since Mass erase via the debugger is blocked !!!
;     <o.0..1> SEC
;       <2=> MCU security status is unsecure
;       <3=> MCU security status is secure
;         <i> Flash Security
;         <i> This bits define the security state of the MCU.
;     <o.6..7> KEYEN
;       <2=> Backdoor key access enabled
;       <3=> Backdoor key access disabled
;         <i> Backdoor key Security Enable
;         <i> These bits enable and disable backdoor key access to the FTFL module.
FSEC            EQU     0xFE
;   </h>
;   <h> Flash Option Register (FOPT)
FOPT            EQU     0xFE
;   </h>
                IF      :LNOT::DEF:RAM_TARGET
                AREA    |.ARM.__at_0x400|, CODE, READONLY
                DCB     BackDoorK0, BackDoorK1, BackDoorK2, BackDoorK3
                DCB     BackDoorK4, BackDoorK5, BackDoorK6, BackDoorK7
                DCB     0xFF,       0xFF,       0xFF,       0xFF
                DCB     0xFF,       FPROT,      FSEC,       FOPT
                ENDIF

                AREA    |.text|, CODE, READONLY

;init_data_bss
;     User defined function for data and bs memory segment initialization.
;      Weak definition to be replaced by user code.

init_data_bss   PROC
                EXPORT  init_data_bss             [WEAK]
                BX      R14
                ENDP

; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  Start
                IMPORT  __main
                LDR     R0, =Start
                BLX     R0
                LDR     R0, =init_data_bss
                BLX     R0
                LDR     R0, =__main
                BX      R0
                ENDP


; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler           [WEAK]
                B       .
                ENDP

Default_Handler PROC
                EXPORT  Reserved16_IRQHandler     [WEAK]
                EXPORT  Reserved17_IRQHandler     [WEAK]
                EXPORT  Reserved18_IRQHandler     [WEAK]
                EXPORT  Reserved19_IRQHandler     [WEAK]
                EXPORT  Reserved20_IRQHandler     [WEAK]
                EXPORT  FTMRE_IRQHandler     [WEAK]
                EXPORT  PMC_IRQHandler     [WEAK]
                EXPORT  IRQ_IRQHandler     [WEAK]
                EXPORT  I2C0_IRQHandler     [WEAK]
                EXPORT  I2C1_IRQHandler     [WEAK]
                EXPORT  SPI0_IRQHandler     [WEAK]
                EXPORT  SPI1_IRQHandler     [WEAK]
                EXPORT  UART0_IRQHandler     [WEAK]
                EXPORT  UART1_IRQHandler     [WEAK]
                EXPORT  UART2_IRQHandler     [WEAK]
                EXPORT  ADC_IRQHandler     [WEAK]
                EXPORT  ACMP0_IRQHandler     [WEAK]
                EXPORT  FTM0_IRQHandler     [WEAK]
                EXPORT  FTM1_IRQHandler     [WEAK]
                EXPORT  FTM2_IRQHandler     [WEAK]
                EXPORT  RTC_IRQHandler     [WEAK]
                EXPORT  ACMP1_IRQHandler     [WEAK]
                EXPORT  PIT_CH0_IRQHandler     [WEAK]
                EXPORT  PIT_CH1_IRQHandler     [WEAK]
                EXPORT  KBI0_IRQHandler     [WEAK]
                EXPORT  KBI1_IRQHandler     [WEAK]
                EXPORT  Reserved42_IRQHandler     [WEAK]
                EXPORT  ICS_IRQHandler     [WEAK]
                EXPORT  WDOG_IRQHandler     [WEAK]
                EXPORT  PWT_IRQHandler     [WEAK]
                EXPORT  MSCAN_RX_IRQHandler     [WEAK]
                EXPORT  MSCAN_TX_IRQHandler     [WEAK]
                EXPORT  DefaultISR                      [WEAK]

Reserved16_IRQHandler
Reserved17_IRQHandler
Reserved18_IRQHandler
Reserved19_IRQHandler
Reserved20_IRQHandler
FTMRE_IRQHandler
PMC_IRQHandler
IRQ_IRQHandler
I2C0_IRQHandler
I2C1_IRQHandler
SPI0_IRQHandler
SPI1_IRQHandler
UART0_IRQHandler
UART1_IRQHandler
UART2_IRQHandler
ADC_IRQHandler
ACMP0_IRQHandler
FTM0_IRQHandler
FTM1_IRQHandler
FTM2_IRQHandler
RTC_IRQHandler
ACMP1_IRQHandler
PIT_CH0_IRQHandler
PIT_CH1_IRQHandler
KBI0_IRQHandler
KBI1_IRQHandler
Reserved42_IRQHandler
ICS_IRQHandler
WDOG_IRQHandler
PWT_IRQHandler
MSCAN_RX_IRQHandler
MSCAN_TX_IRQHandler
DefaultISR

                B       .

                ENDP


                ALIGN


; User Initial Stack & Heap

                IF      :DEF:__MICROLIB

                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit

                ELSE

                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
__user_initial_stackheap

                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR

                ALIGN

                ENDIF


                END
