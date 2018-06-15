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


#include "KEA128_adc.h"
#include "system_SKEAZ1284.h"

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ADC初始化
//  @param      adcn_ch         选择ADC通道
//  @return     void
//  @since      v2.0
//  Sample usage:               adc_init(ADC0_SE8);
//-------------------------------------------------------------------------------------------------------------------
void ADC_Init(ADCn_Ch adcn_ch)
{
    SIM->SCGC |= SIM_SCGC_ADC_MASK;         //开启ADC时钟
    
    ADC->APCTL1 |= 1<<adcn_ch;              //使能ADC引脚
    
    ADC->SC2 = ADC_SC2_REFSEL(1);           //基准电压选择
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ADC采集
//  @param      adcn_ch         选择ADC通道
//  @param      bit             选择精度ADC_8bit
//  @return     void
//  @since      v2.0
//  Sample usage:               uint16 dat = adc_once(ADC0_SE8,ADC_12bit);//采集ADC0_SE8电压，精度12位
//-------------------------------------------------------------------------------------------------------------------
uint16 ADC_Once(ADCn_Ch adcn_ch, ADC_nbit bit)
{
    if(1 == KEA128_CLOCK)
    {
        //超频后，设置ADC的CLK为总线时钟/2
        ADC->SC3 = (0
                    | ADC_SC3_ADIV(1)           //分频系数
                    | ADC_SC3_MODE(bit)         //分辨率
                    | ADC_SC3_ADICLK(0)         //使用总线时钟做为ADC得时钟源
                    | ADC_SC3_ADLSMP_MASK       //1：长采样时间  0：短采样时间   注释为0 长采样时间采集更稳定
                    );
    }
    else
    {
        //未超频，设置ADC的CLK为总线时钟
        ADC->SC3 = (0
                    | ADC_SC3_ADIV(0)           //分频系数
                    | ADC_SC3_MODE(bit)         //分辨率
                    | ADC_SC3_ADICLK(0)         //使用总线时钟做为ADC得时钟源
                    | ADC_SC3_ADLSMP_MASK       //1：长采样时间  0：短采样时间   注释为0 长采样时间采集更稳定
                    );
    }
    
    ADC->SC1 = ADC_SC1_ADCH(adcn_ch);       //启动转换
    
    while(!(ADC->SC1 & ADC_SC1_COCO_MASK)); //等待转换完成
    return (ADC->R & ADC_R_ADR_MASK);       //返回结果
    
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ADC停止采集
//  @param      adcn_ch         选择ADC通道
//  @param      bit             选择精度ADC_8bit
//  @return     void
//  @since      v2.0
//  Sample usage:               adc_stop(adc0);
//-------------------------------------------------------------------------------------------------------------------
void ADC_Stop(void)
{
    ADC->SC1 = ADC_SC1_ADCH(0xFF); 
}

/**
*
*  函数名称：ad_ave
*  功能说明：多次采样，取平均值
*  参数说明：ADCx        模块号
*            ADC_Channel 通道号
*            ADC_nbit    精度（ ADC_8bit, ADC_10bitADC_12bit,）
*            N           均值滤波次数(范围:0~255)
*  函数返回：16位无符号结果值
**/
uint16 AD_Avg(ADCn_Ch adcn_ch, ADC_nbit bit,uint8 N) //均值滤波
{
    uint32 tmp = 0;
    uint8  i;
    uint8  j;
    for(i = 0; i < N; i++){
        tmp+= ADC_Once(adcn_ch, bit);
	j = 20;  
	while(j--);  //循环20次增加每次一次采集时间间隔  	
    }
    tmp = tmp / N;
    return (uint16)tmp;
}
