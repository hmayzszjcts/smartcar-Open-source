
#include "LDPY_GPIO.h"
	
/*************************************************************************
*				零	度	偏	移   
*  函数名称:  LDPY_GPIO_Init
*  功能说明:  IO口初始化函数
*  参数说明:	
*            
*  函数返回:
*  修改时间:2014-05-11
*  备		 注:  
*************************************************************************/ 
void LDPY_GPIO_Init(PORT_Type * PORTx, GPIO_InitTypeDef *GPIO_InitStruct)
{
	uint32_t PI,clear;
		
	//----------------------配置功能，上下拉-------------	
	PORTx->PCR[GPIO_InitStruct->GPIO_Pin]&=(~0x00000703);
		
	PORTx->PCR[GPIO_InitStruct->GPIO_Pin]|=GPIO_InitStruct->GPIO_Mode;
	
	clear=(1<<GPIO_InitStruct->GPIO_Pin);
	PI=(GPIO_InitStruct->GPIO_Direct <<GPIO_InitStruct->GPIO_Pin);
	
	if((uint32_t)PORTx==PORTA_BASE)  		//因为控制寄存器和配置寄存器不同，所以很傻的判断了一下
	{
		GPIOA->PDDR&=(~clear);			//清零相应管脚
		GPIOA->PDDR|=PI;  			//如果是输入，PI=0;如果是输出,PI=1
	}
	else if((uint32_t)PORTx==PORTB_BASE)
	{
		GPIOB->PDDR&=(~clear);
		GPIOB->PDDR|=PI;
	}
	else if((uint32_t)PORTx==PORTC_BASE)
	{
		GPIOC->PDDR&=(~clear);
		GPIOC->PDDR|=PI;
	}
	else if((uint32_t)PORTx==PORTD_BASE)
	{
		GPIOD->PDDR&=(~clear);
		GPIOD->PDDR|=PI;
	}
	else if((uint32_t)PORTx==PORTE_BASE)
	{
		GPIOE->PDDR&=(~clear);
		GPIOE->PDDR|=PI;
	}
		
}


/*************************************************************************
*				零	度	偏	移   
*  函数名称:  LDPY_GPIO_SetBits
*  功能说明:  设置IO口的电平
*  参数说明:	
*            
*  函数返回:
*  修改时间:2014-05-11
*  备		 注:  
*************************************************************************/ 
void LDPY_GPIO_SetBits(GPIO_Type * GPIOx,uint32_t GPIO_Pin)
{
	GPIO_Pin=1<<GPIO_Pin;
	GPIOx->PSOR=GPIO_Pin;	
}

	
/*************************************************************************
*				零	度	偏	移   
*  函数名称:  LDPY_GPIO_ResetBits
*  功能说明:  对一个端口进行设置
*  参数说明:	
*            
*  函数返回:
*  修改时间:2014-05-11
*  备		 注:  
*************************************************************************/ 

void LDPY_GPIO_ResetBits(GPIO_Type * GPIOx,uint32_t GPIO_Pin)
{
	GPIO_Pin=1<<GPIO_Pin;		
	GPIOx->PCOR=GPIO_Pin;	
}
	
	
/*************************************************************************
*				零	度	偏	移   
*  函数名称:  LDPY_GPIO_WtiteBit
*  功能说明:  对一个端口进行设置
*  参数说明:	
*            
*  函数返回:
*  修改时间:2014-05-11
*  备		 注:  
*************************************************************************/
void GPIO_WriteBit(GPIO_Type* GPIOx, uint32_t GPIO_Pin, BitAction BitVal)
{
	GPIO_Pin=1<<GPIO_Pin;
	
	if (BitVal != Bit_RESET)
	{
		GPIOx->PSOR=GPIO_Pin;
	}
	else
	{
		GPIOx->PCOR=GPIO_Pin;
	}
}
	
		
/*************************************************************************
*				零	度	偏	移   
*  函数名称:  LDPY_GPIO_Write
*  功能说明:  对一个端口进行设置
*  参数说明:	
*            
*  函数返回:
*  修改时间:2014-05-11
*  备		 注:  对一组端口写数据
**************************************************************************/
void LDPY_GPIO_Write(GPIO_Type* GPIOx, uint32_t PortVal)
{
	GPIOx->PDOR=PortVal;
}
	
	
	
/*************************************************************************
*				零	度	偏	移   
*  函数名称:  LDPY_GPIO_ReadInputDataBit
*  功能说明:  读取一个端口的数据
*  参数说明:	
*            
*  函数返回:
*  修改时间:2014-05-11
*  备		 注:  
*************************************************************************/

uint8_t LDPY_GPIO_ReadInputDataBit(GPIO_Type* GPIOx, uint32_t GPIO_Pin)
{
	uint8_t bitstatus = 0x00;
	GPIO_Pin=1<<GPIO_Pin;
	
	if ((GPIOx->PDIR & GPIO_Pin) != (uint32_t)Bit_RESET)	//如果端口Pin=1
	{
		bitstatus = (uint8_t)Bit_SET;  			//则返回1
	}
	else
	{
		bitstatus = (uint8_t)Bit_RESET;
	}
	
	
	return bitstatus;
}
	
	
/*************************************************************************
*				零	度	偏	移   
*  函数名称:  LDPY_GPIO_ReadInputData
*  功能说明:  读一组端口的数据
*  参数说明:	
*            
*  函数返回:
*  修改时间:2014-05-11
*  备		 注:  
*************************************************************************/
uint32_t LDPY_GPIO_ReadInputData(GPIO_Type* GPIOx)
{
  return (GPIOx->PDIR);
}






