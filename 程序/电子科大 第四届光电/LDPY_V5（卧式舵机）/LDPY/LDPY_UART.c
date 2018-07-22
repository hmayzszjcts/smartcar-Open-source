
#include "LDPY_UART.h"

/*************************************************************************
*                     	零	度	偏	移
*  函数名称:LDPY_UART5_Init
*  功能说明:初始化串口5，设置波特率115200
*  参数说明:传入参数为波特率
*            
*  函数返回:
*  修改时间:2014-05-10
*  备	注:UART0、1使用内核时钟，其它使用外设bus时钟
*************************************************************************/
void LDPY_UART5_Init(uint32_t UART_baud_rate)
{
	uint16_t sbr, brfa;
	uint8_t temp;
	uint32_t UART_module_clock = 100000 * 1000;//100MHz

	PORTD->PCR[9] = PORT_PCR_MUX(0x3);         //使能PTD9   UART5_TXD
      PORTD->PCR[8] = PORT_PCR_MUX(0x3);         //使能PTD8   UART5_RXD
      SIM->SCGC1 |= SIM_SCGC1_UART5_MASK;     	 //使能UART5时钟
	                    
	 //先禁止发送、接收
	UART5->C2 &= ~(UART_C2_TE_MASK  | UART_C2_RE_MASK );

	//8位模式、无奇偶校验
		
	UART5->C1 = 0;	
	 
	// UART_module_clock = 50000 * 1000;
	sbr = (uint16_t)(UART_module_clock / (UART_baud_rate << 4));
	 
	// Save off the current value of the UARTx_BDH except for the SBR field 
	temp = UART5->BDH & ~(UART_BDH_SBR(0x1F));

	UART5->BDH = temp |  UART_BDH_SBR(((sbr & 0x1F00) >> 8));
	UART5->BDL = (uint8_t)(sbr & UART_BDL_SBR_MASK);

	//brfa = (((sysclk*32)/(baud * 16)) - (sbr * 32));
	brfa = (((UART_module_clock << 5) / (UART_baud_rate << 4)) - (sbr << 5));

	// Save off the current value of the UARTx_C4 register except for the BRFA field 
	temp = UART5->C4 & ~(UART_C4_BRFA(0x1F));

	UART5->C4 = temp |  UART_C4_BRFA(brfa);//配置波特率微调值

	UART5->C2 |= UART_C2_RIE_MASK;//使能接收中断
	
	/*--------------DMA配置-----------------*/
	UART5->C2 |= UART_C2_RIE_MASK;//TDRE中断和DMA传输请求使能
	UART5->C2 |= UART_C2_TIE_MASK;//TC中断请求禁用
	UART5->C5 |= UART_C5_TDMAS_MASK;//配置为DMA请求

	//使能发送、接收
	UART5->C2 |= (UART_C2_TE_MASK | UART_C2_RE_MASK );
 
	Un_ENABLE_IRQ(55);//禁止UART5串口中断，55为串口5状态中断号
}
 
/*************************************************************************
*                     	零	度	偏	移
*  函数名称:LDPY_UART5_PutChar
*  功能说明:串口5发送一个字符
*  参数说明:
*            
*  函数返回:
*  修改时间:2014-05-10
*  备	注:
*************************************************************************/
void LDPY_UART5_PutChar(uint8_t ch)
{
	while(!(UART_X->S1 & UART_S1_TC_MASK));//当发送完成标志=1时跳出UART_S1_TC_MASK
	
	UART_X->D = ch;		
}

/*************************************************************************
*                     	零	度	偏	移
*  函数名称:LDPY_UART5_PutCharArr
*  功能说明:串口5接收一个字符
*  参数说明:
*            
*  函数返回:
*  修改时间:2014-05-10
*  备	注:
*************************************************************************/
void LDPY_UART5_PutCharArr(char *ch,int len)
{
  while(len--)
  {
    LDPY_UART5_PutChar(*(ch++));
  }
}

/*************************************************************************
*                     	零	度	偏	移
*  函数名称:LDPY_UART5_GetChar
*  功能说明:串口5接收一个字符
*  参数说明:
*            
*  函数返回:
*  修改时间:2014-05-10
*  备	注:
*************************************************************************/ 
uint8_t LDPY_UART5_GetChar(void)
{
	while(!(UART_X->S1 & UART_S1_RDRF_MASK));//等待缓冲区有值
	
	return UART_X->D;
}

/*************************************************************************
*                     	零	度	偏	移
*  函数名称:LDPY_UART5_ClearReceiveBuffer
*  功能说明:清串口缓冲区
*  参数说明:
*            
*  函数返回:
*  修改时间:2014-05-10
*  备	注:
*************************************************************************/ 
void LDPY_UART5_ClearReceiveBuffer(void)
{

  while ((UART_X->S1 & UART_S1_RDRF_MASK))//当缓冲区中有值的时候就读取
  {
    UART_X->D;
  }
}

/*------------------------end of LDPY_UART.c--------------------------------*/
	
	
