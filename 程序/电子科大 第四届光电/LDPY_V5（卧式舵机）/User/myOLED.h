
#ifndef __MYOLED_H__
#define __MYOLED_H__

#include "MK60DZ10.h"
#include "LDPY_GPIO.h"
#include "LDPY_SysTick.h"
#include "LDPY_TypeChange.h"
#include "myStruct.h"

#include "string.h"

//#include "ASCIIConfig.h"

#define 	OLED_DA   	PEout(11)
#define 	OLED_CK 	PEout(10)
#define 	OLED_DC		PEout(9)
#define 	OLED_RST	PEout(8)

void myOLED_GPIO_Init(void);
void myOLED_Init(void);
void myOLED_Write_REG(uint8_t COM);
void myOLED_Write_DAT(uint8_t DAT);
void myOLED_REST(void);
void myOLED_Clear(void);
void myOLED_RefreshGRAM(void);

void myOLED_String(uint8_t x,uint8_t y,const char *P);
void myOLED_Char(uint8_t ch);
void myOLED_Set_XY(uint8_t x,uint8_t y);
void myOLED_Dec(uint8_t x,uint8_t y,uint16_t DEC);
void myOLED_Dec4(uint8_t x,uint8_t y, int16_t DEC);
void myOLED_ADC_value(uint16 * value);
void myOLED_XY_point(uint8_t x,uint8_t y,uint8_t ch);

void myOLED_Decimals(uint8_t x,uint8_t y, float als);

void myOLED_CCDwave(CCD_Info * CCD1_info,CCD_Info * CCD2_info, CCD_Info * CCD3_info);

#endif 

/*-------------------------------end of myOLED.h---------------------------------*/

