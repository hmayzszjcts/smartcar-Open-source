
#ifndef  __LDPY_ADC_H__
#define  __LDPY_ADC_H__

#include "MK60DZ10.h"
#include "LDPY_GPIO.h"
#include "LDPY_SysTick.h"
#include "myOLED.h"
#include "myStruct.h"
#include "myLED.h"

void LDPY_ADC0_Init(void);
void LDPY_ADC1_Init(void);

uint16_t LDPY_ADC0_GetValue(uint8_t Channel);
uint16_t LDPY_ADC1_GetValue(uint8_t Channel);
uint16_t LDPY_ADC1_ENC(void);

#endif

/*------------------------end of LDPY_ADC.h------------------------------------*/
