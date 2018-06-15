#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#include  "common.h"

/*
 * Include 用户自定义的头文件
 */

#include  "gpio.h"       //IO口操作
#include  "LED.H"       //流水灯
#include  "uart.h"      //串口
#include  "adc.h"       //ADC模块
#include  "FTM.h"       //FTM模块（FTM0：电机控制 / 通用 /PWM     FTM1、2：正交解码 / 通用 /PWM ）
#include  "PIT.h"       //周期中断计时器
#include  "i2c.h"
#include  "AT24C02.h"
#include  "lptmr.h"     //低功耗定时器(延时)
#include  "exti.h"       //EXTI外部GPIO中断

/***************** ucos 专用 *****************/
#if USOC_EN > 0u
#include  "ucos_ii.h"  		        //uC/OS-II系统函数头文件
#include  "BSP.h"			//与开发板相关的函数
#include  "app.h"			//用户任务函数


#endif  //if  USOC_EN > 0


/***************** 定时器 **********************
 *   1x8ch 电机控制 / 通用 /PWM 定时器 (FTM)
 *   1x2ch 正交解码器 / 通用 /PWM 定时器 (FTM)
 *   载波调制定时器 (CMT)
 *   可编程延迟模块 (PDB)
 *   1x4ch 可编程中断定时器 (PIT)
 *   低功耗定时器 (LPT)
**********************************************/

/*******************频率设置**********************
 *  在 Kinetis 芯片内部存在 3 种不同时钟：
 *  内核时钟    总线时钟    Flash 时钟
 *
 * 我们开发板上，在 k60_tower.h 处定义
 * #define CORE_CLK_MHZ      PLL100      //内核频率为100M
 * 在 pll_init 中：100M的选项：MCG=PLL, core = MCG, bus = MCG/2, FlexBus = MCG/2, Flash clock= MCG/4
 * 即 内核频率 为 100MHz 、总线时钟 为 50MHz 、 Flash 时钟为 25MHz
 * printf 输出端口也是 在 k60_tower.h 处定义
**********************************************/




#endif  //__INCLUDE_H__
