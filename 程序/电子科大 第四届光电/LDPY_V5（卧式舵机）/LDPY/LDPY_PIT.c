
#include "LDPY_PIT.h"
	
/*************************************************************************
*				零	度	偏	移                             
*
*  函数名称:LDPY_PIT0_Init(void)
*  功能说明:PIT0 初始化
*  参数说明:
*  函数返回:
*  修改时间:2014-05-09
*  备    注: Bus clock = 100MHz
*************************************************************************/		
void LDPY_PIT0_Init(void)
{
	uint32_t IRQ_Group,IRQ_Posit,IRQ_number;
	IRQ_number=68;
	
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK ;//使能定时器时钟
	
	PIT->MCR &=~ (PIT_MCR_MDIS_MASK | PIT_MCR_FRZ_MASK );//使能定时器时钟，调试模式继续运行

	IRQ_Group=IRQ_number/32;	//计算中断号在寄存器组里的偏移，组号[0~7],每个组能操作32个中断
	IRQ_Posit=IRQ_number%32;	//计算中断号在寄存器里的位置偏移，位号[0~31]
	NVIC->ICPR[IRQ_Group]|=(1<<IRQ_Posit);  // 先清中断挂起，保险起见
	NVIC->ISER[IRQ_Group]|=(1<<IRQ_Posit);  // 使能中断		
}	
	
/*************************************************************************
*				零	度	偏	移                             
*
*  函数名称:LDPY_PIT0_ms(uint32_t ms_Load)
*  功能说明:PIT0 ms定时
*  参数说明:
*  函数返回:
*  修改时间:2014-05-09
*  备    注: 最大定时429ms
*************************************************************************/			
void LDPY_PIT0_ms(uint32_t ms_Load)
{
	
	PIT->CHANNEL[0].LDVAL= ms_Load*100000;//加载时间
	PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;//清除中断标志位

	PIT->CHANNEL[0].TCTRL |= (PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK);//定时器使能，中断使能		
}	

/*************************************************************************
*				零	度	偏	移                             
*
*  函数名称:LDPY_PIT0_us(uint32_t ms_Load)
*  功能说明:PIT0 us定时
*  参数说明:
*  函数返回:
*  修改时间:2014-05-09
*  备    注: 
*************************************************************************/
void LDPY_PIT0_us(uint32_t us_Load)
{	
	PIT->CHANNEL[0].LDVAL= us_Load*100;//加载时间
	PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;//清除中断标志位
	//
	PIT->CHANNEL[0].TCTRL |= (PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK);//定时器使能，中断使能

}
	
/*************************************************************************
*				零	度	偏	移                             
*
*  函数名称:LDPY_PIT1_Init(void)
*  功能说明:PIT1 初始化
*  参数说明:
*  函数返回:
*  修改时间:2014-05-09
*  备    注:
*************************************************************************/
void LDPY_PIT1_Init(void)
{
	uint32_t IRQ_Group,IRQ_Posit,IRQ_number;
	IRQ_number=69;
	
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK ;//使能定时器时钟
	
	PIT->MCR &=~ (PIT_MCR_MDIS_MASK | PIT_MCR_FRZ_MASK );//使能定时器时钟，调试模式继续运行

	IRQ_Group=IRQ_number/32;	//计算中断号在寄存器组里的偏移，组号[0~7],每个组能操作32个中断
	IRQ_Posit=IRQ_number%32;	//计算中断号在寄存器里的位置偏移，位号[0~31]
	NVIC->ICPR[IRQ_Group]|=(1<<IRQ_Posit);  // 先清中断挂起，保险起见
	NVIC->ISER[IRQ_Group]|=(1<<IRQ_Posit);  // 使能中断		
}
/*************************************************************************
*				零	度	偏	移                             
*
*  函数名称:LDPY_PIT0_10ns(uint32_t ns_Load)
*  功能说明:PIT0 ns定时
*  参数说明:
*  函数返回:
*  修改时间:2014-05-09
*  备    注: 最大定时 429 496 728 ns
*************************************************************************/			
void LDPY_PIT1_10ns(uint32_t ns_Load)
{		
	PIT->CHANNEL[1].LDVAL= ns_Load;//加载时间
	PIT->CHANNEL[1].TFLG = PIT_TFLG_TIF_MASK;//清除中断标志位
	//
	PIT->CHANNEL[1].TCTRL |= (PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK);//定时器使能，中断使能	
}

/*************************************************************************
*				零	度	偏	移                             
*
*  函数名称:LDPY_PIT1_us(uint32_t us_Load)
*  功能说明:PIT1 us定时
*  参数说明:
*  函数返回:
*  修改时间:2014-05-09
*  备    注: 最大定时 429 495 us
*************************************************************************/			
void LDPY_PIT1_us(uint32_t us_Load)
{	
	PIT->CHANNEL[1].LDVAL= us_Load*100;//加载时间
	PIT->CHANNEL[1].TFLG = PIT_TFLG_TIF_MASK;//清除中断标志位

	PIT->CHANNEL[1].TCTRL |= (PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK);//定时器使能，中断使能

}
/*************************************************************************
*				零	度	偏	移                             
*
*  函数名称:LDPY_PIT1_ms(uint32_t ms_Load)
*  功能说明:PIT1 ms定时
*  参数说明:
*  函数返回:
*  修改时间:2014-05-09
*  备    注: 最大定时 428 ms
*************************************************************************/			
void LDPY_PIT1_ms(uint32_t ms_Load)
{

	PIT->CHANNEL[1].LDVAL= ms_Load*100000;//加载时间
	PIT->CHANNEL[1].TFLG = PIT_TFLG_TIF_MASK;//清除中断标志位

	PIT->CHANNEL[1].TCTRL |= (PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK);//定时器使能，中断使能		
}	
	
	
/*------------------------end of myPIT.c----------------------------------*/

