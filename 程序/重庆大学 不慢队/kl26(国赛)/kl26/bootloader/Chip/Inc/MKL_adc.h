/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MKL_adc.h
 * @brief      ADC函数
 * @author     山外科技
 * @version    v5.0
 * @date       2013-08-28
 */


#ifndef __MKL_ADC_H__
#define __MKL_ADC_H__ 1

typedef enum
{
    // ---------------------------------ADC0-------------------------
    ADC0_DP0 = 0,   // PTE20
    ADC0_SE0 = 0,

    ADC0_DP1 = 1,   // PTE16
    ADC0_SE1 = 1,

    ADC0_DP2 = 2,   // PTE18
    ADC0_SE2 = 2,

    ADC0_DP3 = 3,   // PTE22
    ADC0_SE3 = 3,

    ADC0_DM0 = 4,   // PTE21
    ADC0_SE4a= 4,

    ADC0_DM1 = 5,   // PTE17
    ADC0_SE5a= 5,

    ADC0_DM2 = 6,   // PTE19
    ADC0_SE6a= 6,

    ADC0_DM3 = 7,   // PTE23
    ADC0_SE7a= 7,

    ADC0_SE4b= 4,   // PTE29     不支持软件ADC，传递进软件触发ADC，会当作 a通道处理

    ADC0_SE5b= 5,   // PTD1      不支持软件ADC，传递进软件触发ADC，会当作 a通道处理

    ADC0_SE6b= 6,   // PTD5      不支持软件ADC，传递进软件触发ADC，会当作 a通道处理

    ADC0_SE7b= 7,   // PTD6      不支持软件ADC，传递进软件触发ADC，会当作 a通道处理

    ADC0_SE8,       // PTB0

    ADC0_SE9,       // PTB1

    ADC0_RES0,      // 保留

    ADC0_SE11,      // PTC2

    ADC0_SE12,      // PTB2

    ADC0_SE13,      // PTB3

    ADC0_SE14,      // PTC0

    ADC0_SE15,      // PTC1

    ADC0_RES1,      // 保留

    ADC0_RES2,      // 保留

    ADC0_RES3,      // 保留

    ADC0_RES4,      // 保留

    ADC0_RES5,      // 保留

    ADC0_RES6,      // 保留

    ADC0_RES7,      // 保留

    ADC0_SE23,      // PTE30

    DAC0_OUT = ADC0_SE23, // PTE30  DAC0输出 ,传入 ADC函数会当作 ADC0_SE23  处理

    ADC0_RES8,      // 保留

    ADC0_RES9,      // 保留

    Temp0_Sensor,   // Temperature Sensor,内部温度测量，可用ADC函数
    Bandgap0,       // 温度补偿结构带隙基准源   不支持ADC
    ADC0_RES10,     // 保留
    VREFH0,         // 参考高电压,可用ADC函数 ,结果恒为 2^n-1
    VREFL0,         // 参考低电压,可用ADC函数 ,结果恒为 0
    Module0_Dis,    // 不支持 ADC



} ADCn_Ch_e;

typedef enum  //ADC模块
{
    ADC0,

    ADC_MAX

} ADCn_e;

//精度位数
typedef enum ADC_nbit
{
    ADC_8bit   = 0x00,
    ADC_10bit  = 0x02,
    ADC_12bit  = 0x01,
    ADC_16bit  = 0x03
} ADC_nbit;


//外部函数接口声明
extern void     adc_init    (ADCn_Ch_e);                //ADC初始化
extern uint16   adc_once    (ADCn_Ch_e, ADC_nbit);      //采集一次一路模拟量的AD值


extern void     adc_stop    (ADCn_e);                   //停止ADC转换


#endif /* __MKL_ADC_H__ */
