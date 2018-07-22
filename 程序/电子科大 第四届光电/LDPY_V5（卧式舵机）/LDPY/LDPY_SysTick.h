

#ifndef  __LDPY_SYSTICK_H__
#define  __LDPY_SYSTICK_H__

#include "MK60DZ10.h"

void LDPY_SysTickInit(void);
void LDPY_Delay_ms(uint32_t ms_count);
void LDPY_Delay_us(uint32_t us_count);
void LDPY_Delay_10ns(uint32_t ns_count);
void LDPY_Delay_s(uint8_t s_count);
	
#endif

/*------------------------end of LDPY_SysTick.h------------------------*/



