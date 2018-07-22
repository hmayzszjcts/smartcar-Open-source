
#ifndef  _LED_H
#define  _LED_H

#include "MK60DZ10.h"
#include "LDPY_GPIO.h"
#include "LDPY_SysTick.h"
	
#define LED3_ON   PEout(26)=0;
#define LED3_OFF  PEout(26)=1;

#define LED4_ON   PEout(27)=0;
#define LED4_OFF  PEout(27)=1;

#define LED5_ON   PEout(28)=0;
#define LED5_OFF  PEout(28)=1;

#define Bell_ON   PEout(25)=1;
#define Bell_OFF  PEout(25)=0;

void myLED_Init(void);
void myLED_LED_ON(void);

#endif

