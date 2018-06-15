/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		KEA128_adc
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.23
 * @Target core		S9KEA128AMLK
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-11-6
 ********************************************************************************************************************/


#ifndef _KEA128_adc_h
#define _KEA128_adc_h

#include "misc.h"
#include "common.h"
#include "SKEAZ1284.h"
#include "KEA128_port_cfg.h"

typedef enum
{
    ADC0_SE0=0,  //A0
    ADC0_SE1,    //A1
    ADC0_SE2,    //A6
    ADC0_SE3,    //A7
    ADC0_SE4,    //B0
    ADC0_SE5,    //B1
    ADC0_SE6,    //B2
    ADC0_SE7,    //B3
    ADC0_SE8,    //C0
    ADC0_SE9,    //C1
    ADC0_SE10,   //C2
    ADC0_SE11,   //C3
    ADC0_SE12,   //F4
    ADC0_SE13,   //F5
    ADC0_SE14,   //F6
    ADC0_SE15    //F7
} ADCn_Ch;



//精度位数
typedef enum ADC_nbit
{
    ADC_8bit   = 0x00,
    ADC_10bit  = 0x01,
    ADC_12bit  = 0x02
} ADC_nbit;


void adc_init(ADCn_Ch adcn_ch);
uint16 adc_once(ADCn_Ch adcn_ch, ADC_nbit bit);
void adc_stop(void);
uint16 ad_ave(ADCn_Ch adcn_ch, ADC_nbit bit,uint8 N);



#endif
