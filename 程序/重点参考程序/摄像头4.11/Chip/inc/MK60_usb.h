
/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MK60_usb.c
 * @brief      USB 代码库(目前 仅 有 虚拟 串口 )
 * @author     山外科技
 * @version    v5.0
 * @date       2013-09-26
 * @note       本例程 移植 飞思卡尔公司 提供的 K60 参考代码
 */


#ifndef _MK60_USB_H_
#define _MK60_USB_H_


#include "USB.h"


// USB  模块时钟必须 配置 为 48M

// USB 模块时钟源 选择
#define USB_CLOCK_PLL       //PLL　作为时钟源
//#define USB_CLOCK_FLL     //FLL  作为时钟源
//#define USB_CLOCK_CLKIN   //USB_CLKIN (即 PTE26) 作为时钟源

//USB clk = 时钟源频率 * (1 + USB_CLK_FRAC) / (USB_CLK_DIV + 1)
//USB_CLK_FRAC = 0 或 1
//USB_CLK_DIV 0 ~ 7
#if defined(MK60DZ10)
//PLL  = 96M
#define USB_CLK_DIV         1       // 时钟源分频因子 。 
#define USB_CLK_FRAC        0

#elif defined(MK60F15)
//PLL  = 120M
#define USB_CLK_DIV         4       // 时钟源分频因子 。 
#define USB_CLK_FRAC        1


#endif




void    usb_init(void);                         // USB 模块初始化

void    usb_com_init(void);                     // USB 虚拟串口 初始化
void    usb_enum_wait(void);                    // USB 等待枚举
uint8   usb_com_rx(uint8 *rx_buf);              // USB 虚拟串口 接收
void    usb_com_tx(uint8 *tx_buf, uint8 len);   // USB 虚拟串口 发送


#endif  //_MK60_USB_H_
