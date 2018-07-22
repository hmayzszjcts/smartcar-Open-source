

#ifndef __LDPY_UART_H__
#define __LDPY_UART_H__

#include "MK60DZ10.h"
#include "LDPY_EXTI.h"

typedef enum 
{
	lukuang = 255,
	renzi1  = 253,
	renzi2  = 252,

	sudu1   = 250,

}uart5;

#define 	UART_X	(UART5)
#define 	LDPY_UART5_DisableIrq()		Un_ENABLE_IRQ(55)	//关闭UART5串口中断
#define		LDPY_UART5_EnableIrq()		ENABLE_IRQ(55)	//打开UART5串口中断

void LDPY_UART5_Init(uint32_t UART_baud_rate);

uint8_t LDPY_UART5_GetChar(void);

void LDPY_UART5_PutChar(uint8_t ch);
void LDPY_UART5_PutCharArr(char *ch,int len);

void LDPY_UART5_ClearReceiveBuffer(void);
 
#endif

/*---------------------------end of LDPY_UART.h------------------------------*/

