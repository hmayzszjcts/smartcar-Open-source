
#include "myLED.h"

/*************************************************************************
*				零	度	偏	移   
*  函数名称:  myLED_LEDInit
*  功能说明:  LED初始化函数
*  参数说明:	
*            
*  函数返回:
*  修改时间:2014-05-09
*  备注:  
*************************************************************************/ 
void myLED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//LED3
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_26;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_PU;
	GPIO_InitStructure.GPIO_Direct=GPIO_Direct_Out;
	LDPY_GPIO_Init(PORTE,&GPIO_InitStructure);
	PEout(26)=1;
	
	//LED4
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_27;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_PU;
	GPIO_InitStructure.GPIO_Direct=GPIO_Direct_Out;
	LDPY_GPIO_Init(PORTE,&GPIO_InitStructure);
	PEout(27)=1;
	
	//LED5
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_28;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_PU;
	GPIO_InitStructure.GPIO_Direct=GPIO_Direct_Out;
	LDPY_GPIO_Init(PORTE,&GPIO_InitStructure);
	PEout(28)=1;
	
	//蜂鸣器
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_25;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_PU;
	GPIO_InitStructure.GPIO_Direct=GPIO_Direct_Out;
	LDPY_GPIO_Init(PORTE,&GPIO_InitStructure);
	PEout(25)=0;
	
}

/*************************************************************************
*				零	度	偏	移   
*  函数名称:  myLED_LED_ON
*  功能说明:  LED闪烁函数
*  参数说明:	
*            
*  函数返回:
*  修改时间:2014-05-09
*  备注: 
*************************************************************************/ 
void myLED_LED_ON(void)
{	  
	 LED3_ON;    
	 LDPY_Delay_ms(100);
	 LED3_OFF;
	 LDPY_Delay_ms(100);
	
	 LED4_ON;    
	 LDPY_Delay_ms(100);
	 LED4_OFF;
	 LDPY_Delay_ms(100);
	
	 LED5_ON;   
	 LDPY_Delay_ms(100);
	 LED5_OFF;
	 LDPY_Delay_ms(100);
}
	
/*----------------------------end of myLED.c-------------------------------*/
	
	
