/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MK60_ftm.c
 * @brief      TPM定时器函数库
 * @author     山外科技
 * @version    v5.0
 * @date       2013-08-22
 */

#ifndef _MK60_TPM_H_
#define _MK60_TPM_H_

//定义TPM模块号
typedef enum
{
    TPM0,
    TPM1,
    TPM2,

    TPM_MAX,
} TPMn_e;

//定义TPM 通道号
typedef enum
{

    TPM_CH0,
    TPM_CH1,
    TPM_CH2,
    TPM_CH3,
    TPM_CH4,
    TPM_CH5,
    TPM_CH6,
    TPM_CH7,

} TPM_CHn_e;

typedef enum
{
    TPM_CLKIN0,         // 相应的管脚请看 TPM_CLKIN0_PIN 的定义
    TPM_CLKIN1,         // 相应的管脚请看 TPM_CLKIN1_PIN 的定义
} TPM_CLKIN_e;

//分频系数
typedef enum
{
    TPM_PS_1,
    TPM_PS_2,
    TPM_PS_4,
    TPM_PS_8,
    TPM_PS_16,
    TPM_PS_32,
    TPM_PS_64,
    TPM_PS_128,

    TPM_PS_MAX,
}TPM_PS_e;

//用于开始计数 或者 重载计数 (暂时没用到)
typedef enum
{
    EXTRG_IN,       //外部触发
    CMP0_OUT,       //CMP0 输出

    TRG_CFG_RES0,   //保留
    TRG_CFG_RES1,   //保留

    PIT0_TRG,       //PIT0 触发
    PIT1_TRG,       //PIT0 触发

    TRG_CFG_RES2,   //保留
    TRG_CFG_RES3,   //保留

    TPM0_OVER,      //TPM0溢出
    TPM1_OVER,      //TPM1溢出
    TPM2_OVER,      //TPM2溢出

    TRG_CFG_RES4,   //保留

    RTC_ALARM,      //RTC报警
    RTC_SEC,        //RTC秒中断

    LPTMR_TRG,      //LPTMR 触发

    TRG_CFG_RES5,   //保留


}TPMx_TRG_CFG_e;



//TPM 输入捕捉配置
typedef enum
{
    TPM_Rising,               //上升沿捕捉
    TPM_Falling,              //下降沿捕捉
    TPM_Rising_or_Falling     //跳变沿捕捉
} TPM_Input_cfg;


extern TPM_MemMapPtr TPMN[TPM_MAX];

/*********************** PWM **************************/

#define TPM0_PRECISON 10000u     //定义占空比精度，100即精度为1%，1000u则精度为0.1%，用于占空比 duty 形参传入，即占空比为 duty/TPM_PRECISON
#define TPM1_PRECISON 100u     //定义占空比精度，100即精度为1%，1000u则精度为0.1%，用于占空比 duty 形参传入，即占空比为 duty/TPM_PRECISON
#define TPM2_PRECISON 100u     //定义占空比精度，100即精度为1%，1000u则精度为0.1%，用于占空比 duty 形参传入，即占空比为 duty/TPM_PRECISON

extern void  tpm_pwm_init(TPMn_e, TPM_CHn_e, uint32 freq, uint32 duty);  //初始化TPM的PWM功能并设置频率、占空比。设置通道输出占空比。同一个TPM，各通道的PWM频率是一样的，共3个TPM

extern void  tpm_pwm_duty(TPMn_e, TPM_CHn_e,              uint32 duty);  //设置通道占空比,占空比为 （duty * 精度） % ，如果 TPM_PRECISON 定义为 1000 ，duty = 100 ，则占空比 100*0.1%=10%
extern void  tpm_pwm_freq(TPMn_e,            uint32 freq);               //设置TPM的频率（改频率后，需要重新配置占空比）


#define TPM_IRQ_EN(TPMn,CHn)        TPM_CnSC_REG(TPMN[TPMn],CHn) |= TPM_CnSC_CHIE_MASK       //开启 TPMn_CHn 中断
#define TPM_IRQ_DIS(TPMn,CHn)       TPM_CnSC_REG(TPMN[TPMn],CHn) &= ~TPM_CnSC_CHIE_MASK      //关闭 TPMn_CHn 中断

extern void     tpm_pulse_init(TPMn_e tpmn,TPM_CLKIN_e clkin,TPM_PS_e ps);                  //脉冲计数初始化
extern uint16   tpm_pulse_get (TPMn_e tpmn);                                                 //获取脉冲数
extern void    tpm_pulse_clean (TPMn_e tpmn);                                              //清脉冲计数


#endif  //_MK60_TPM_H_





