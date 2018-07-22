
#ifndef  __LDPY_PIT_H__
#define  __LDPY_PIT_H__

#include "MK60DZ10.h"

void LDPY_PIT0_Init(void);
void LDPY_PIT0_ms(uint32_t ms_Load);	
void LDPY_PIT0_us(uint32_t us_Load);

void LDPY_PIT1_Init(void);
void LDPY_PIT1_10ns(uint32_t ns_Load);
void LDPY_PIT1_us(uint32_t us_Load);
void LDPY_PIT1_ms(uint32_t ms_Load);

#endif

/*------------------end of LDPY_PIT.h-----------------------*/

