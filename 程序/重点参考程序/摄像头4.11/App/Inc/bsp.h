#ifndef __BSP_H__
#define __BSP_H__


extern unsigned int pitCnt ;
extern unsigned int catchCnt;
extern int distance_bsp;

extern void PIT1_IRQHandler();
extern void BSP_init();
extern void PORTC_IRQHandler();
#endif