/*********************************************************************************************************************
 * 
 * @file       		misc.h
 *  		
 * @core			S9KEA128
 * @date       		2018
 ********************************************************************************************************************/

#ifndef _misc_h
#define _misc_h

#include "common.h"

void write_vtor (int);                                              

#define enable_irq(irq)                 NVIC_EnableIRQ(irq)         
#define disable_irq(irq)                NVIC_DisableIRQ(irq)        
#define set_irq_priority(irq,pri0)      NVIC_SetPriority(irq,pri0)  

#define EnableInterrupts                __enable_irq()              
#define DisableInterrupts               __disable_irq()             


#endif
