

#ifndef __LDPY_ISR_H__
#define __LDPY_ISR_H__

#include "MK60DZ10.h"
#include "LDPY_SysTick.h"
#include "LDPY_GPIO.h"
#include "LDPY_PIT.h"

#include "myStruct.h"
#include "myLED.h"
#include "myOLED.h"
#include "myCCD.h"
#include "myKey.h"
#include "myLabVIEW.h"

#define 	ARM_INTERRUPT_LEVEL_BITS	(4)
#define 	PIT0_IRQ_NUM	(0)
#define 	PIT1_IRQ_NUM	(1)
#define 	PIT2_IRQ_NUM	(2)
#define 	PIT3_IRQ_NUM	(3)

#define 	StartEndLine_TIME (0.15)	//检测起跑线红外管间允许间隔的最大时间,150ms

/***********************************************************************/
//  /*!< Macro to enable all interrupts. */
//#define EnableInterrupts asm(" CPSIE i");

//  /*!< Macro to disable all interrupts. */
//#define DisableInterrupts asm(" CPSID i");
/***********************************************************************/

extern void NMI_Handler(void);
extern void HardFault_Handler(void);
extern void MemManage_Handler(void);
extern void BusFault_Handler(void);
extern void UsageFault_Handler(void);
extern void SVC_Handler(void);
extern void DebugMon_Handler(void);
extern void PendSV_Handler(void);
extern void SysTick_Handler(void);

extern void PORTB_IRQHandler(void);
extern void PORTC_IRQHandler(void);

extern void PIT1_IRQHandler(void);
extern void PIT0_IRQHandler(void);

void LDPY_Isr_SetIrqPriority (int irq, int prio);

#endif

/*--------------------------end of LDPY_Isr.h----------------------------*/
