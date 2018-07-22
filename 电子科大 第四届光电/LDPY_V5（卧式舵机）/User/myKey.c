
#include "myKey.h"

/*************************************************************************
*				零	度	偏	移                             
*
*  函数名称:myKey_Init(void)
*  功能说明:按键初始化
*  参数说明:
*  函数返回:
*  修改时间:2014-05-09
*  备    注: 
*************************************************************************/
void myKey_Init(void)
{
	EXTI_Init(PORTE, GPIO_Pin_2,Trigger_Falling );//按键 1
	EXTI_Init(PORTE, GPIO_Pin_3,Trigger_Falling );//按键 2
	EXTI_Init(PORTE, GPIO_Pin_4,Trigger_Falling );//按键 3

	EXTI_Init(PORTE, GPIO_Pin_5,Trigger_Falling );//按键 4
	EXTI_Init(PORTE, GPIO_Pin_6,Trigger_Falling );//按键 5
	EXTI_Init(PORTE, GPIO_Pin_7,Trigger_Falling );//按键 6
	
	EXTI_Init(PORTD, GPIO_Pin_11,Trigger_Rising );//遥控器
}

/*************************************************************************
*				零	度	偏	移                             
*
*  函数名称:myKey_SatrtEndLineInit(void)
*  功能说明:起跑线及尾灯初始化
*  参数说明:
*  函数返回:
*  修改时间:2014-05-09
*  备    注: 
*************************************************************************/
void myKey_StartEndLineInit(void)
{
	EXTI_Init(PORTD, GPIO_Pin_12,Trigger_Falling );//  红外  前中左
	EXTI_Init(PORTD, GPIO_Pin_13,Trigger_Falling );//  红外  前左
	EXTI_Init(PORTD, GPIO_Pin_14,Trigger_Falling );//  红外  前右
	EXTI_Init(PORTD, GPIO_Pin_15,Trigger_Falling );//  红外  前中右
	
	NVIC_SetPriorityGrouping(1);
	NVIC_SetPriority(PORTD_IRQn,1);
	
	NVIC_SetPriorityGrouping(1);
	NVIC_SetPriority(PIT0_IRQn,2);
}

/*-------------------------end of myKey.c-------------------------*/
