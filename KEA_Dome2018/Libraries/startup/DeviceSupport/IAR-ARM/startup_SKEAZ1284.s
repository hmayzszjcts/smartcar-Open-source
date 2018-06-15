/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		启动文件
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.23
 * @Target core		S9KEA128AMLK
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-11-6
 ********************************************************************************************************************/


;
; The modules in this file are included in the libraries, and may be replaced
; by any user-defined modules that define the PUBLIC symbol _program_start or
; a user defined start symbol.
; To override the cstartup defined in the library, simply add your modified
; version to the workbench project.
;
; The vector table is normally located at address 0.
; When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
; The name "__vector_table" has special meaning for C-SPY:
; it is where the SP start value is found, and the NVIC vector
; table register (VTOR) is initialized to this address if != 0.
;
; Cortex-M version
;

        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:ROOT(2)

        EXTERN  __iar_program_start
        EXTERN  Start        
        PUBLIC  __vector_table
        PUBLIC  __Vectors
        PUBLIC  __Vectors_End
        PUBLIC  __Vectors_Size

        DATA
__vector_table
        DCD     sfe(CSTACK)               ; Top of Stack
        DCD     Reset_Handler             ; Reset Handler
        DCD     NMI_Handler               ; NMI Handler
        DCD     HardFault_Handler         ; Hard Fault Handler
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     SVC_Handler               ; SVCall Handler
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     PendSV_Handler            ; PendSV Handler
        DCD     SysTick_Handler           ; SysTick Handler
        ; External Interrupts
        DCD     Reserved16_IRQHandler     ; 0:  Reserved interrupt 16/0
        DCD     Reserved17_IRQHandler     ; 1:  Reserved interrupt 17/1
        DCD     Reserved18_IRQHandler     ; 2:  Reserved interrupt 18/2
        DCD     Reserved19_IRQHandler     ; 3:  Reserved interrupt 19/3
        DCD     Reserved20_IRQHandler     ; 4:  Reserved interrupt 20/4
        DCD     FTMRE_IRQHandler          ; 5:  Command complete and error interrupt
        DCD     PMC_IRQHandler            ; 6:  Low-voltage detect, low-voltage warning
        DCD     IRQ_IRQHandler            ; 7:  External interrupt
        DCD     I2C0_IRQHandler           ; 8:  Single interrupt vector for all sources
        DCD     I2C1_IRQHandler           ; 9:  Single interrupt vector for all sources
        DCD     SPI0_IRQHandler           ;10:  Single interrupt vector for all sources
        DCD     SPI1_IRQHandler           ;11:  Single interrupt vector for all sources
        DCD     UART0_IRQHandler          ;12:  UART0 Status and Error interrupt
        DCD     UART1_IRQHandler          ;13:  UART1 Status and Error interrupt
        DCD     UART2_IRQHandler          ;14:  UART2 Status and Error interrupt
        DCD     ADC0_IRQHandler           ;15:  ADC conversion complete interrupt
        DCD     ACMP0_IRQHandler          ;16:  Analog comparator 0 interrupt
        DCD     FTM0_IRQHandler           ;17:  Single interrupt vector for all sources
        DCD     FTM1_IRQHandler           ;18:  Single interrupt vector for all sources
        DCD     FTM2_IRQHandler           ;19:  Single interrupt vector for all sources
        DCD     RTC_IRQHandler            ;20:  RTC overflow
        DCD     ACMP1_IRQHandler          ;21:  Analog comparator 1 interrupt
        DCD     PIT_CH0_IRQHandler        ;22:  PIT CH0 overflow
        DCD     PIT_CH1_IRQHandler        ;23:  PIT CH1 overflow
        DCD     KBI0_IRQHandler           ;24:  Keyboard interrupt0
        DCD     KBI1_IRQHandler           ;25:  Keyboard interrupt1
        DCD     Reserved26_IRQHandler     ;26:  Reserved interrupt 42/26
        DCD     ICS_IRQHandler            ;27:  Clock loss of lock Interrupt
        DCD     WDG_IRQHandler            ;28:  WDG timeout interrupt
        DCD     PWT_IRQHandler            ;29:  Single interrupt vector for all sources
        DCD     MSCAN_Rx_IRQHandler       ;30:  MSCAN Rx Interrupt
        DCD     MSCAN_Tx_IRQHandler       ;31:  MSCAN Tx, Err and Wake-up interrupt
__Vectors_End
__FlashConfig
      	DCD	0xFFFFFFFF
      	DCD	0xFFFFFFFF
      	DCD	0xFFFFFFFF
      	DCD	0xFFFFFFFE
__FlashConfig_End

__Vectors       EQU   __vector_table
__Vectors_Size 	EQU 	__Vectors_End - __Vectors


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB

        PUBWEAK Reset_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reset_Handler
        LDR     R0, =Start
        BLX     R0
        LDR     R0, =__iar_program_start
        BX      R0

		PUBWEAK NMI_Handler
		SECTION .text:CODE:REORDER:NOROOT(1)
NMI_Handler
        B NMI_Handler
		
		PUBWEAK HardFault_Handler
		SECTION .text:CODE:REORDER:NOROOT(1)
HardFault_Handler
		B HardFault_Handler
		
		PUBWEAK MemManage_Handler
		SECTION .text:CODE:REORDER:NOROOT(1)
MemManage_Handler
		B MemManage_Handler
		
		PUBWEAK BusFault_Handler
		SECTION .text:CODE:REORDER:NOROOT(1)
BusFault_Handler
		B BusFault_Handler
		
		PUBWEAK UsageFault_Handler
		SECTION .text:CODE:REORDER:NOROOT(1)
UsageFault_Handler
		B UsageFault_Handler
		
		PUBWEAK SVC_Handler
		SECTION .text:CODE:REORDER:NOROOT(1)
SVC_Handler
		B SVC_Handler
		
		PUBWEAK DebugMon_Handler
		SECTION .text:CODE:REORDER:NOROOT(1)
DebugMon_Handler
		B DebugMon_Handler
		
		PUBWEAK PendSV_Handler
		SECTION .text:CODE:REORDER:NOROOT(1)
PendSV_Handler
		B PendSV_Handler
		
		PUBWEAK SysTick_Handler
		SECTION .text:CODE:REORDER:NOROOT(1)
SysTick_Handler
		B SysTick_Handler
		
        PUBWEAK Reserved16_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
Reserved16_IRQHandler
        B Reserved16_IRQHandler
        
        PUBWEAK Reserved17_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
Reserved17_IRQHandler
        B Reserved17_IRQHandler
        
        PUBWEAK Reserved18_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
Reserved18_IRQHandler
        B Reserved18_IRQHandler
        
        PUBWEAK Reserved19_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
Reserved19_IRQHandler
        B Reserved19_IRQHandler
        
        PUBWEAK Reserved20_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
Reserved20_IRQHandler
        B Reserved20_IRQHandler
        
		PUBWEAK FTMRE_IRQHandler
		SECTION .text:CODE:REORDER:NOROOT(1)
FTMRE_IRQHandler
		B FTMRE_IRQHandler
		
		PUBWEAK PMC_IRQHandler
		SECTION .text:CODE:REORDER:NOROOT(1)
PMC_IRQHandler
		B PMC_IRQHandler
		
		PUBWEAK IRQ_IRQHandler
		SECTION .text:CODE:REORDER:NOROOT(1)
IRQ_IRQHandler
		B IRQ_IRQHandler
		
		PUBWEAK I2C0_IRQHandler
		SECTION .text:CODE:REORDER:NOROOT(1)
I2C0_IRQHandler
		B I2C0_IRQHandler
		
		PUBWEAK I2C1_IRQHandler
		SECTION .text:CODE:REORDER:NOROOT(1)
I2C1_IRQHandler
		B I2C1_IRQHandler
		
		PUBWEAK SPI0_IRQHandler
		SECTION .text:CODE:REORDER:NOROOT(1)
SPI0_IRQHandler
		B SPI0_IRQHandler
		
		PUBWEAK SPI1_IRQHandler
		SECTION .text:CODE:REORDER:NOROOT(1)
SPI1_IRQHandler
		B SPI1_IRQHandler
		
		PUBWEAK UART0_IRQHandler
		SECTION .text:CODE:REORDER:NOROOT(1)
UART0_IRQHandler
		B UART0_IRQHandler
		
		PUBWEAK UART1_IRQHandler
		SECTION .text:CODE:REORDER:NOROOT(1)
UART1_IRQHandler
		B UART1_IRQHandler
		
		PUBWEAK UART2_IRQHandler
		SECTION .text:CODE:REORDER:NOROOT(1)
UART2_IRQHandler
		B UART2_IRQHandler
		
		PUBWEAK ADC0_IRQHandler
		SECTION .text:CODE:REORDER:NOROOT(1)
ADC0_IRQHandler
		B ADC0_IRQHandler
		
		PUBWEAK ACMP0_IRQHandler
		SECTION .text:CODE:REORDER:NOROOT(1)
ACMP0_IRQHandler
		B ACMP0_IRQHandler
		
		PUBWEAK FTM0_IRQHandler
		SECTION .text:CODE:REORDER:NOROOT(1)
FTM0_IRQHandler
		B FTM0_IRQHandler
		
		PUBWEAK FTM1_IRQHandler
		SECTION .text:CODE:REORDER:NOROOT(1)
FTM1_IRQHandler
		B FTM1_IRQHandler
		
		PUBWEAK FTM2_IRQHandler
		SECTION .text:CODE:REORDER:NOROOT(1)
FTM2_IRQHandler
		B FTM2_IRQHandler
		
		PUBWEAK RTC_IRQHandler
		SECTION .text:CODE:REORDER:NOROOT(1)
RTC_IRQHandler
		B RTC_IRQHandler
		
		PUBWEAK ACMP1_IRQHandler
		SECTION .text:CODE:REORDER:NOROOT(1)
ACMP1_IRQHandler
		B ACMP1_IRQHandler
		
		PUBWEAK PIT_CH0_IRQHandler
		SECTION .text:CODE:REORDER:NOROOT(1)
PIT_CH0_IRQHandler
		B PIT_CH0_IRQHandler
		
		PUBWEAK PIT_CH1_IRQHandler
		SECTION .text:CODE:REORDER:NOROOT(1)
PIT_CH1_IRQHandler
		B PIT_CH1_IRQHandler
		
		PUBWEAK KBI0_IRQHandler
		SECTION .text:CODE:REORDER:NOROOT(1)
KBI0_IRQHandler
		B KBI0_IRQHandler
		
		PUBWEAK KBI1_IRQHandler
		SECTION .text:CODE:REORDER:NOROOT(1)
KBI1_IRQHandler
		B KBI1_IRQHandler
		
        PUBWEAK Reserved26_IRQHandler
		SECTION .text:CODE:REORDER:NOROOT(1)
Reserved26_IRQHandler
		B Reserved26_IRQHandler
        
		PUBWEAK ICS_IRQHandler
		SECTION .text:CODE:REORDER:NOROOT(1)
ICS_IRQHandler
		B ICS_IRQHandler
		
		PUBWEAK WDG_IRQHandler
		SECTION .text:CODE:REORDER:NOROOT(1)
WDG_IRQHandler
		B WDG_IRQHandler
		
		PUBWEAK PWT_IRQHandler
		SECTION .text:CODE:REORDER:NOROOT(1)
PWT_IRQHandler
		B PWT_IRQHandler
		
		PUBWEAK MSCAN_Rx_IRQHandler
		SECTION .text:CODE:REORDER:NOROOT(1)
MSCAN_Rx_IRQHandler
		B MSCAN_Rx_IRQHandler
		
		PUBWEAK MSCAN_Tx_IRQHandler
		SECTION .text:CODE:REORDER:NOROOT(1)
MSCAN_Tx_IRQHandler
		B MSCAN_Tx_IRQHandler

		PUBWEAK DefaultISR
        SECTION .text:CODE:REORDER:NOROOT(1)       
DefaultISR
        B DefaultISR
		
        END
