/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		KEA128_uart
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.23
 * @Target core		S9KEA128AMLK
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-11-6
 ********************************************************************************************************************/

#include "KEA128_uart.h"

UART_Type * uart[3] = UART_BASES;

void uart_mux(UARTn uartn)
{
    switch(uartn)
    {
        case uart0:
        {
            SIM->SCGC |= SIM_SCGC_UART0_MASK;               //使能串口0时钟
            if(B0 == UART0_RX_PIN)
            {
                SIM->PINSEL &= ~SIM_PINSEL_UART0PS_MASK;
            }
            else
            {
                SIM->PINSEL |= SIM_PINSEL_UART0PS_MASK;
            }
        }break;
        
        case uart1:
        {
            SIM->SCGC |= SIM_SCGC_UART1_MASK;               //使能串口0时钟
            if(C6 == UART1_RX_PIN) 
            {
                SIM->PINSEL1 &= ~SIM_PINSEL1_UART1PS_MASK;
            }
            else
            {
                SIM->PINSEL1 |= SIM_PINSEL1_UART1PS_MASK;
            }
        }break;
        
        case uart2:
        {
            SIM->SCGC |= SIM_SCGC_UART2_MASK;               //使能串口0时钟
            if(D6 == UART2_RX_PIN) 
            {
                SIM->PINSEL1 &= ~SIM_PINSEL1_UART2PS_MASK;
            }
            else
            {
                SIM->PINSEL1 |= SIM_PINSEL1_UART2PS_MASK;
            }
        }break;
        
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      uart 初始化
//  @param      uartn           选择串口(uart0,uart1,uart2)
//  @param      baud            串口波特率
//  @return     uint32          实际设置得波特率
//  @since      v1.0
//  Sample usage:               uart_init(uart1,9600);   //初始化串口1为1位起始位、8位数据位、1位停止位、波特率9600
//  @note                       由于KEA128没有波特率微调寄存器，因此在设置得波特率较高得情况下误差比较大，因此返回实际波特率用来校验
//-------------------------------------------------------------------------------------------------------------------
uint32 uart_init(UARTn uartn, uint32 baud)
{
    vuint32 uart_input_clk;
    uint32 sbr;
    
    uart_mux(uartn);
    
    //设置的时候，应该禁止发送和接收
    uart[uartn]->C2 &= ~(0
                         | UART_C2_TE_MASK
                         | UART_C2_RE_MASK);
    
    //配置成8位无校验模式
    //设置 UART 数据格式、校验方式和停止位位数。通过设置 UART 模块控制寄存器 C1 实现；
    uart[uartn]->C1 |= (0
                        //| UART_C1_M_MASK                      //9 位或 8 位模式选择 : 0 为 8位 ，1 为 9位（注释了表示0，即8位） （如果是9位，位8在UARTx_C3里）
                        //| UART_C1_PE_MASK                     //奇偶校验使能（注释了表示禁用）
                        //| UART_C1_PT_MASK                     //校验位类型 : 0 为 偶校验 ，1 为 奇校验
                       );
    
    //计算波特率
    uart_input_clk = bus_clk_khz * 1000;   //bus时钟

    //UART 波特率 = UART 模块时钟 / (16 × SBR[12:0])
    //SBR = UART 模块时钟 / (16 * UART 波特率)
    sbr = ((uart_input_clk>>4)*10 / baud + 5)/10;               //四舍五入
    if(sbr > 0x1FFF)sbr = 0x1FFF;                               //SBR 是 13bit，最大为 0x1FFF

    //写 SBR
    uart[uartn]->BDH &= ~UART_BDH_SBR_MASK;                     //清除原来波特率
    uart[uartn]->BDH |= UART_BDH_SBR(sbr>>8);                   //先写入SBR高位
    uart[uartn]->BDL  = UART_BDL_SBR((uint8)sbr);               //再写入SBR低位

    // 允许发送和接收
    uart[uartn]->C2 |= (0
                        | UART_C2_TE_MASK                       //发送使能
                        | UART_C2_RE_MASK                       //接收使能
                        //| UART_C2_TIE_MASK                    //发送中断或DMA传输请求使能（注释了表示禁用）
                        //| UART_C2_TCIE_MASK                   //发送完成中断使能（注释了表示禁用）
                        //| UART_C2_RIE_MASK                    //接收满中断或DMA传输请求使能（注释了表示禁用）
                        );
    //由于KEA128没有波特率微调寄存器，因此在设置得波特率较高得情况下误差比较大，因此返回实际波特率用来校验
    return ((uart_input_clk>>4)/sbr);                           //返回实际波特率
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      串口发送一个字节
//  @param      uartn           选择串口
//  @param      ch              要发送的字符
//  @return     void
//  @since      v1.0
//  Sample usage:               uart_putchar(uart3,0x5a);   
//-------------------------------------------------------------------------------------------------------------------
void uart_putchar(UARTn uartn, uint8 ch)
{
    while( !((uart[uartn]->S1) & UART_S1_TDRE_MASK) );  //等待发送缓冲区空
    uart[uartn]->D = (uint8)ch;                         //发送数据
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      串口发送数组
//  @param      uartn           选择串口
//  @param      *buff           要发送的数组地址
//  @param      len             发送长度
//  @return     void
//  @since      v1.0
//  Sample usage:               uart_putbuff(uart3,&a[0],sizeof(a));  
//-------------------------------------------------------------------------------------------------------------------
void uart_putbuff (UARTn uartn, uint8 *buff, uint32 len)
{
    while(len--)
    {
        uart_putchar(uartn, *buff);
        buff++;
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      串口发送字符串
//  @param      uartn           选择串口
//  @param      *str            要发送的字符串地址
//  @return     void
//  @since      v1.0
//  Sample usage:               uart_putstr(uart3,"i lvoe you"); 
//-------------------------------------------------------------------------------------------------------------------
void uart_putstr (UARTn uartn, const uint8 *str)
{
    while(*str)
    {
        uart_putchar(uartn, *str++);
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      串口等待接收1个字符
//  @param      uartn           选择串口
//  @param      *str            接收的地址
//  @return     void
//  @since      v1.0
//  Sample usage:               uart_getchar(uart3,&dat);
//-------------------------------------------------------------------------------------------------------------------
void uart_getchar (UARTn uartn, uint8 *ch) 
{
    while(!(uart[uartn]->S1 & UART_S1_RDRF_MASK));  //等待接收满了   
    *ch =  uart[uartn]->D;                          // 获取接收到的8位数据
    // 获取 9位数据时ch 应该是uint16 *类型的否则数据会溢出：
    // *ch =   ((( UARTx_C3_REG(UARTN[uartn]) & UART_C3_R8_MASK ) >> UART_C3_R8_SHIFT ) << 8)   |   UART_D_REG(UARTN[uartn]);  //返回9bit
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      开启接收中断
//  @param      uartn           选择串口
//  @return     void
//  @since      v1.0
//  Sample usage:               uart_rx_irq_en(uart3);
//-------------------------------------------------------------------------------------------------------------------
void uart_rx_irq_en(UARTn uartn)
{
    uart[uartn]->C2 |= UART_C2_RIE_MASK;                                        //使能UART接收中断
    enable_irq((IRQn_Type)((uint8)uartn + UART0_IRQn));                                //使能IRQ中断
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      开启发送完成中断
//  @param      uartn           选择串口
//  @return     void
//  @since      v1.0
//  Sample usage:               uart_txc_irq_en(uart3);
//-------------------------------------------------------------------------------------------------------------------
void uart_txc_irq_en(UARTn uartn)
{
    uart[uartn]->C2 |= UART_C2_TCIE_MASK;                                       //使能UART发送完成中断
    enable_irq((IRQn_Type)((uint8)uartn + UART0_IRQn));                                //使能IRQ中断
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      禁止接收中断
//  @param      uartn           选择串口
//  @return     void
//  @since      v1.0
//  Sample usage:               uart_rx_irq_dis(uart3);
//-------------------------------------------------------------------------------------------------------------------
void uart_rx_irq_dis(UARTn uartn)
{
    uart[uartn]->C2 &= ~UART_C2_RIE_MASK;                                       //禁止UART接收中断

    //如果发送中断还没有关，则不关闭IRQ
    if(!(uart[uartn]->C2 & (UART_C2_TIE_MASK | UART_C2_TCIE_MASK)) )
    {
        disable_irq((IRQn_Type)((uint8)uartn + UART0_IRQn));                           //关IRQ中断
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      禁止发送完成中断
//  @param      uartn           选择串口
//  @return     void
//  @since      v1.0
//  Sample usage:               uart_txc_irq_dis(uart3);
//-------------------------------------------------------------------------------------------------------------------
void uart_txc_irq_dis(UARTn uartn)
{
    uart[uartn]->C2 &= ~UART_C2_TCIE_MASK;                                      //禁止UART发送完成中断

    //如果接收中断还没有关，则不关闭IRQ
    if(!(uart[uartn]->C2 & UART_C2_RIE_MASK) )
    {
        disable_irq((IRQn_Type)((uint8)uartn + UART0_IRQn));              //关IRQ中断
    }
}

//串口中断函数请到isr.c文件参考



//-------------------------------------------------------------------------------------------------------------------
//  @brief      重定义printf 到串口
//  @param      ch      需要打印的字节
//  @param      stream  数据流
//  @since      v1.0
//  @note       此函数由编译器自带库里的printf所调用
//-------------------------------------------------------------------------------------------------------------------
int fputc(int ch, FILE *stream)
{
    uart_putchar(DEBUG_PORT, (char)ch);
    return(ch);
}












