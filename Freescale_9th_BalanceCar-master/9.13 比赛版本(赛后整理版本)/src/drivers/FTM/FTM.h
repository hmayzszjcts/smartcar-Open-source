/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名       ：FTM.h
 * 描述         ：包含PWM、输入捕捉函数配置头文件
 *
 * 实验平台     ：野火kinetis开发板
 * 库版本       ：
 * 嵌入系统     ：
 *
 * 作者         ：野火嵌入式开发工作室
 * 淘宝店       ：http://firestm32.taobao.com
 * 技术支持论坛 ：http://www.ourdev.cn/bbs/bbs_list.jsp?bbs_id=1008
**********************************************************************************/

#ifndef _FTM_H_
#define _FTM_H_

extern volatile struct FTM_MemMap *FTMx[3];

typedef enum FTMn
{
    FTM0,
    FTM1,
    FTM2
} FTMn;

typedef enum CHn
{
    //   --FTM0--  --FTM1--  --FTM2--
    CH0,   //     PTC1      PTA8      PTA10
    CH1,   //     PTC2      PTA9      PTA11
    CH2,   //     PTC3       ×         ×
    CH3,   //     PTC4       ×         ×
    CH4,   //     PTD4       ×         ×
    CH5,   //     PTD5       ×         ×
    CH6,   //     PTD6       ×         ×
    CH7    //     PTD7       ×         ×
    // ×表示不存在
} CHn;



/*********************** PWM **************************/

#define FTM_PRECISON 4000u     //定义占空比精度，100即精度为1%，1000u则精度为0.1%，用于占空比 duty 形参传入，即占空比为 duty/FTM_PRECISON

void  FTM_PWM_init(FTMn, CHn, u32 freq, u32 duty);  //初始化FTM的PWM功能并设置频率、占空比。设置通道输出占空比。同一个FTM，各通道的PWM频率是一样的，共3个FTM

void  FTM_PWM_Duty(FTMn, CHn,         u32 duty);    //设置通道占空比,占空比为 （duty * 精度） % ，如果 FTM_PRECISON 定义为 1000 ，duty = 100 ，则占空比 100*0.1%=10%
void  FTM_PWM_freq(FTMn,    u32 freq);              //设置FTM的频率


/*********************** 输入捕捉 **************************/

typedef enum Input_cfg
{
    Rising,               //上升沿捕捉
    Falling,              //下降沿捕捉
    Rising_or_Falling     //跳变沿捕捉
} Input_cfg;


void FTM_Input_init(FTMn, CHn, Input_cfg);

#define FTM_IRQ_EN(FTMn,CHn)        FTM_CnSC_REG(FTMx[FTMn],CHn) |= FTM_CnSC_CHIE_MASK       //开启 FTMn_CHn 中断
#define FTM_IRQ_DIS(FTMn,CHn)       FTM_CnSC_REG(FTMx[FTMn],CHn) &= ~FTM_CnSC_CHIE_MASK      //关闭 FTMn_CHn 中断








#endif  //_FTM_H_



