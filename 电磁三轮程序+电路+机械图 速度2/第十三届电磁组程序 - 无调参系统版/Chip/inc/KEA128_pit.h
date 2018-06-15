/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		KEA128_pit
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.23
 * @Target core		S9KEA128AMLK
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-11-6
 ********************************************************************************************************************/

#ifndef _KEA128_pit_h
#define _KEA128_pit_h

#include "misc.h"
#include "common.h"
#include "SKEAZ1284.h"
#include "KEA128_port_cfg.h"

//定义PIT模块号
typedef enum
{
    PIT0,
    PIT1,
} PITn;

#define     PIT_FLAG_CLR(PITn)          PIT->CHANNEL[PITn].TFLG |= PIT_TFLG_TIF_MASK        //清中断标志（写1 清空标志位）

void    pit_init(PITn pitn, uint32 cnt);
void    pit_delay(PITn pitn, uint32 cnt);

//------------------------------------以下代码用于PIT计时------------------------------------
void    pit_time_start(PITn pitn);
uint32  pit_time_get(PITn pitn);
void    pit_close(PITn pitn);
#define pit_time_get_ms(PITn_e)   (pit_time_get(PITn_e)/bus_clk_khz)                    //获取计时时间（单位为 ms）
#define pit_time_get_us(PITn_e)   (pit_time_get(PITn_e)/(bus_clk_khz/1000))             //获取计时时间（单位为 us）

//------------------------------------以下代码用于PIT中断------------------------------------
#define     pit_init_ms(PITn_e,ms)          pit_init(PITn_e,ms * bus_clk_khz);                  //初始化PITn，并设置定时时间(单位为 ms)
#define     pit_init_us(PITn_e,us)          pit_init(PITn_e,us * bus_clk_khz/1000);             //初始化PITn，并设置定时时间(单位为 us)
#define     pit_init_ns(PITn_e,ns)          pit_init(PITn_e,ns * bus_clk_khz/1000000);          //初始化PITn，并设置定时时间(单位为 ns)


//------------------------------------以下代码用于PIT延时------------------------------------
#define     pit_delay_ms(PITn_e,ms)          pit_delay(PITn_e,ms * bus_clk_khz);                //PIT延时 ms
#define     pit_delay_us(PITn_e,us)          pit_delay(PITn_e,us * bus_clk_khz/1000);           //PIT延时 us
#define     pit_delay_ns(PITn_e,ns)          pit_delay(PITn_e,ns * bus_clk_khz/1000000);        //PIT延时 ns(40M总线频率时最小单位为25ns)


#endif
