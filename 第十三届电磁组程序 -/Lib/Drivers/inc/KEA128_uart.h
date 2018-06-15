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


#ifndef _KEA128_uart_h
#define _KEA128_uart_h


#include "misc.h"
#include "common.h"
#include "SKEAZ1284.h"
#include "KEA128_port_cfg.h"
#include "stdio.h"




typedef enum
{
    uart0,
    uart1,
    uart2,
} UARTn;


#define DEBUG_PORT  uart1
#define DEBUG_BAUD  115200



uint32 uart_init(UARTn uartn, uint32 baud);
void uart_putchar (UARTn uartn, uint8 ch);
void uart_putbuff (UARTn uartn, uint8 *buff, uint32 len);
void uart_putstr (UARTn uartn, const uint8 *str);
void uart_getchar (UARTn uartn, uint8 *ch);
void uart_rx_irq_en(UARTn uartn);
void uart_txc_irq_en(UARTn uartn);
void uart_rx_irq_dis(UARTn uartn);
void uart_txc_irq_dis(UARTn uartn);


#endif
