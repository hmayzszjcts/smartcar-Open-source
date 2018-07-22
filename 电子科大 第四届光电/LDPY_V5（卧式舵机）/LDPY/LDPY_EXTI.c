

#include "LDPY_EXTI.h"
/*************************************************************************
*				零	度	偏	移
*  函数名称:  EXTI_Init外部引脚中断配置函数
*  功能说明:  
*  参数说明:	IRQ_number中断号在手册20页

*  函数返回:
*  修改时间:
*  备		 注:	只初始化PORTx,外部引脚中断）		
*************************************************************************/ 	

void EXTI_Init(PORT_Type * PORTx,uint8_t GPIO_Pin_x,EXTI_Trigger_mode  Trigger_mode )
{
	uint8_t IRQ_number=0x00,IRQ_Group,IRQ_Posit;
	
	PORTx->PCR[GPIO_Pin_x]&=(~0x000F0703);		//将[19:16][1:0]设置为缺省值
	PORTx->PCR[GPIO_Pin_x]|=PORT_PCR_MUX(1);  	//GPIO
	PORTx->PCR[GPIO_Pin_x]|=Trigger_mode;		//配置触发模式
	
	/*-----------配置管脚为输入模式------------------*/
	
	if((uint32_t)PORTx==PORTA_BASE)
	{
		GPIOA->PDDR&=~(1<<GPIO_Pin_x);  		//在相应管脚置0，则为输入模式
		IRQ_number=87;					// 中断请求号IRQ
	}
	else if((uint32_t)PORTx==PORTB_BASE)
	{
		GPIOB->PDDR&=~(1<<GPIO_Pin_x);
		IRQ_number=88;
	}
	else if((uint32_t)PORTx==PORTC_BASE)
	{
		GPIOC->PDDR&=~(1<<GPIO_Pin_x);
		IRQ_number=89;
	}
	else if((uint32_t)PORTx==PORTD_BASE)
	{
		GPIOD->PDDR&=~(1<<GPIO_Pin_x);
		IRQ_number=90;
	}
	else if((uint32_t)PORTx==PORTE_BASE)
	{
		GPIOE->PDDR&=~(1<<GPIO_Pin_x);
		IRQ_number=91;
	}
									
	IRQ_Group=IRQ_number/32;	//计算中断号在寄存器组里的偏移，组号[0~7]
	IRQ_Posit=IRQ_number%32;	//计算中断号在寄存器里的位置偏移，位号[0~31]
	NVIC->ICPR[IRQ_Group]|=(1<<IRQ_Posit);  // 先清中断挂起，保险起见
	NVIC->ISER[IRQ_Group]|=(1<<IRQ_Posit);  // 使能中断			
}

/*************************************************************************
*				零	度	偏	移    
*
*  函数名称:LDPY_Isr_SetIrqPriority
*  功能说明:设置中断分组
*  参数说明: NVIC_Group(0~4)
*			 组 号		AIRCR[10:8]		 IPR[7:0](x1:x2  x1位抢占优先级,x2位响应优先级)
			   0			111			   0:4
			   1			110			   1:3
			   2			101			   2:2
			   3			100			   3:1
			   4			011			   4:0
*  函数返回:
*  修改时间:2014-08-10
*  备    注:
*************************************************************************/
void LDPY_NVIC_PriorityGroupConfig(uint8_t NVIC_Group)
{
	uint32_t Group_temp = 0;
	uint32_t SCB_temp = 0;
	
	Group_temp = (~NVIC_Group) & 0x07;
	Group_temp <<= 8;
	
	SCB_temp = SCB->AIRCR;	//读取
	SCB_temp &= 0x0000F8FF; //清零
	SCB_temp |= 0x05FA0000; //写入密匙
	SCB_temp |= Group_temp; //配置组号
	
	SCB->AIRCR = SCB_temp;  //写入
}
/*************************************************************************
*				零	度	偏	移    
*
*  函数名称:LDPY_NVIC_Init
*  功能说明:设置中断优先级
*  参数说明:	irq 为中断向量号	
*			prio为优先级，0为最高优先级，然后1,2，3.....依次降低
*  函数返回:
*  修改时间:2014-05-14
*  备    注:
*************************************************************************/	
void LDPY_NVIC_Init(uint8_t NVIC_PreemptionPriority,uint8_t NVIC_SubPriority,uint8_t NVIC_Channel,uint8_t NVIC_Group) 
{
	uint8_t IPRADDR = 0, IPROFFSET = 0;
	uint8_t IPR_temp = 0;
	
	IPRADDR = NVIC_Channel / 4;		//组地址
	IPROFFSET = NVIC_Channel % 4;	//组内偏移
	IPROFFSET = IPROFFSET * 8 + 4;  //组内位置
	
	IPR_temp  = NVIC_PreemptionPriority << (4 - NVIC_Group);
	IPR_temp |= NVIC_SubPriority & (0x0F >> NVIC_Group);
	IPR_temp &= 0x0F;
	
	LDPY_NVIC_PriorityGroupConfig(NVIC_Group);
	
	if(NVIC_Channel<32)
	{
		NVIC->ISER[0]|=1<<NVIC_Channel;
	}
	else 			
	{		
		NVIC->ISER[1]|=1<<(NVIC_Channel-32); 
	}
	
	NVIC->IP[IPRADDR]|=IPR_temp<<IPROFFSET;		
}

/*************************************************************************
*				零	度	偏	移
*  函数名称:  ENABLE_IRQ
*  功能说明:  中断使能函数
*  参数说明:	IRQ_number中断号在手册20页
*            
*  函数返回:
*  修改时间:
*  备		 注:
*************************************************************************/ 	
void ENABLE_IRQ(uint8_t  IRQ_number)
{
	uint8_t  IRQ_Group,IRQ_Posit;
	
	IRQ_Group=IRQ_number/32;	//计算中断号在寄存器组里的偏移，组号[0~7]
	IRQ_Posit=IRQ_number%32;	//计算中断号在寄存器里的位置偏移，位号[0~31]
	NVIC->ICPR[IRQ_Group]|=(1<<IRQ_Posit);  // 先清中断挂起，保险起见
	NVIC->ISER[IRQ_Group]|=(1<<IRQ_Posit);  // 使能中断	

}
	
/*************************************************************************
*                             零度偏移
*  函数名称:  Un_ENABLE_IRQ
*  功能说明:  中断除能函数
*  参数说明:	
*            
*  函数返回:
*  修改时间:
*  备		 注:
*************************************************************************/
void Un_ENABLE_IRQ(uint8_t  IRQ_number)
{
	uint8_t  IRQ_Group,IRQ_Posit;
	
	IRQ_Group=IRQ_number/32;	//计算中断号在寄存器组里的偏移，组号[0~7]
	IRQ_Posit=IRQ_number%32;	//计算中断号在寄存器里的位置偏移，位号[0~31]
	NVIC->ICPR[IRQ_Group]|=(1<<IRQ_Posit);  // 先清中断挂起，保险起见
	NVIC->ICER[IRQ_Group]=(1<<IRQ_Posit);  // 
}
