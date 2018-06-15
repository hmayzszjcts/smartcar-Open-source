/*********************************************************************************************************************
 * 
 * @file       		isr.c
 *  				中断服务函数
 * @core			S9KEA128
 * @date       		2018
 ********************************************************************************************************************/
#include "includefile.h"


/****************全局变量定义******************/
uint8 Flag_Stop = OFF;			//=OFF停车
uint8 Flag_Speed = ON;			//=ON增加速度控制
uint8 Flag_Direction = ON;		//=ON增加方向控制
uint8 Flag_Debuge = OFF;		//=ON进入调参界面
uint8 Flag_Out = 2;				//虚拟示波器显示标志


void PIT_CH0_IRQHandler(void)
{
    PIT_FLAG_CLR(PIT0);
    
	//标志变量定义
	static uint16 i = 0;
	static uint16 j = 0;
	
	//0.5sLED闪烁
	i++;
	if(i >= 100)  
    {    
    	i = 0;
    	gpio_turn(LED);
	}
	//20ms一次速度控制
	j++;
	if(j >= 4)  
    {    
    	j = 0;
		SpeedControl();
	}	
	DirectionControl();	//方向控制
	PWMOut();			//最终PWM输出
	
	//以下为虚拟示波器部分
	switch(Flag_Out)
	{		
		case 0 : 	OutData[0] = (int)g_fRealSpeed;		//真实速度
		  			OutData[1] = (int)g_fSpeedError;	//速度偏差
					OutData[2] = (int)g_fSpeedFilter;	//速度滤波
					OutData[3] = (int)g_fExpectSpeed;	//期望速度
					break;
					
		case 1 :	OutData[0] = (int)g_ValueOfAD[0];	//水平左电感
		  			OutData[1] = (int)g_ValueOfAD[1];	//水平右电感
					OutData[2] = (int)(1000*g_fDirectionError[0]);		//方向偏差	  （放大1000倍）
					OutData[3] = (int)(1000*g_fDirectionError_dot[0]);	//方向偏差微分（放大1000倍）
					break;
					
		case 2 :	OutData[0] = (int)g_ValueOfAD[0];//水平左电感
		  			OutData[1] = (int)g_ValueOfAD[1];//垂直右电感
					OutData[2] = (int)g_ValueOfAD[2];//水平左电感
					OutData[3] = (int)g_ValueOfAD[3];//垂直右电感
					break;	
					
		case 3 :	OutData[0] = (int)g_fSpeedControlOut;		//速度控制输出	（红）
		  			OutData[1] = (int)g_fDirectionControlOut;	//方向输出		（黄）
					OutData[2] = (int)g_PWMOut;					//总PWM			（蓝）
					OutData[3] = 0;
					break;
		default :break;
	}
	OutPut_Data();
}


void PIT_CH1_IRQHandler(void)
{
    PIT_FLAG_CLR(PIT1); 
    
}

void IRQ_IRQHandler(void)
{  
    CLEAR_IRQ_FLAG; 
    gpio_set(BUZZ,1);		//蜂鸣器开
    ir_decode();			//红外解码和处理    
    gpio_set(BUZZ,0); 		//蜂鸣器关
}

void KBI0_IRQHandler(void)
{
    CLEAN_KBI0_FLAG;

}

/*
记得进入中断后清除标志位
FTMRE_IRQHandler      
PMC_IRQHandler        
IRQ_IRQHandler        
I2C0_IRQHandler       
I2C1_IRQHandler       
SPI0_IRQHandler       
SPI1_IRQHandler       
UART0_IRQHandler 
UART1_IRQHandler 
UART2_IRQHandler 
ADC0_IRQHandler       
ACMP0_IRQHandler      
FTM0_IRQHandler       
FTM1_IRQHandler       
FTM2_IRQHandler       
RTC_IRQHandler        
ACMP1_IRQHandler      
PIT_CH0_IRQHandler    
PIT_CH1_IRQHandler    
KBI0_IRQHandler       
KBI1_IRQHandler       
Reserved26_IRQHandler 
ICS_IRQHandler        
WDG_IRQHandler        
PWT_IRQHandler        
MSCAN_Rx_IRQHandler   
MSCAN_Tx_IRQHandler   
*/



