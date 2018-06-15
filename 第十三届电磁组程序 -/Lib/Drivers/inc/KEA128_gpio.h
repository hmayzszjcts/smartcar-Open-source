/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		KEA128_gpio
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.23
 * @Target core		S9KEA128AMLK
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-11-6
 ********************************************************************************************************************/


#ifndef _KEA128_gpio_h
#define _KEA128_gpio_h



#include "misc.h"
#include "common.h"
#include "SKEAZ1284.h"
#include "KEA128_port_cfg.h"




void    gpio_init(PTX_n ptx_n, GPIO_MOD ddr, uint8 dat);//初始化gpio
void    gpio_ddr(PTX_n ptx_n, GPIO_MOD ddr);            //设置引脚方向
uint8   gpio_get(PTX_n ptx_n);                          //获取引脚状态
void    gpio_set(PTX_n ptx_n, uint8 dat);               //设置引脚状态
void    gpio_turn(PTX_n ptx_n);                         //翻转引脚状态


#endif
