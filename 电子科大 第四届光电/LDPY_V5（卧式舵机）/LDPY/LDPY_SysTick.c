
#include "LDPY_SysTick.h"

/*************************************************************************
*				零	度	偏	移  
*
*  函数名称：LDPY_SysTickInit
*  功能说明：时钟初始化
*  参数说明：
*  函数返回：无
*  修改时间：2014-05-09
*  备    注：
*************************************************************************/
void LDPY_SysTickInit(void)
{
	SysTick->CTRL=0x04;//清比较标志，时钟源：core clock,中断禁止，Disable
}

/*************************************************************************
*				零	度	偏	移  
*
*  函数名称：LDPY_Delay_ms
*  功能说明:ms级延时函数
*  参数说明：
*  函数返回：无
*  修改时间：2014-05-09
*  备    注：最大延时167ms
*************************************************************************/
void LDPY_Delay_ms(uint32_t ms_count)
{
	uint32_t temp;
	SysTick->LOAD=ms_count*100000;		//因为内核时钟100M，所以计一次是0.01us，毫秒延时要乘上因子
	SysTick->VAL=0x00;					//清零计数器
	SysTick->CTRL|=0x01;				//ENABLE
	do
	{
			temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));		//temp&0x01判断是否使能;(temp&(1<<16)判断计数完成
	
	SysTick->CTRL=0x04;				//DISABLE
	SysTick->VAL=0x00; 				//clear
}
/*************************************************************************
*				零	度	偏	移  
*
*  函数名称：LDPY_Delay_us
*  功能说明:us级延时函数
*  参数说明：
*  函数返回：无
*  修改时间：2014-05-09
*  备    注：最大延时 167777us
******************************************************************/	
void LDPY_Delay_us(uint32_t us_count)
{
	uint32_t temp;
	SysTick->LOAD=us_count*100;
	SysTick->VAL=0x00;
	SysTick->CTRL|=0x01;
	do
	{
			temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));
	
	SysTick->CTRL=0x04;
	SysTick->VAL=0x00;
}
	
/*************************************************************************
*				零	度	偏	移  
*
*  函数名称：LDPY_Delay_10ns
*  功能说明:ns级延时函数
*  参数说明：
*  函数返回：无
*  修改时间：2014-05-09
*  备    注：
******************************************************************/	
void LDPY_Delay_10ns(uint32_t ns_count)
{
	uint32_t temp;
	SysTick->LOAD=ns_count;
	SysTick->VAL=0x00;
	SysTick->CTRL|=0x01;
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));
	
	SysTick->CTRL=0x04;
	SysTick->VAL=0x00;
}

/*************************************************************************
*				零	度	偏	移  
*
*  函数名称：LDPY_Delay_s
*  功能说明:s级延时函数
*  参数说明：
*  函数返回：无
*  修改时间：2014-05-09
*  备    注：
*************************************************************************/
void LDPY_Delay_s(uint8_t s_count)
{
  uint8_t i;
  for(i=0; i<10*s_count; i++)
	 {
	   LDPY_Delay_ms(100);
	 }
}
	
