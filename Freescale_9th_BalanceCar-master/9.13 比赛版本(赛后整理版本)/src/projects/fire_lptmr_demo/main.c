/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名       ：main.c
 * 描述         ：lptmr操作实验
 *
 * 实验平台     ：野火kinetis开发板
 * 库版本       ：
 * 嵌入系统     ：
 *
 * 作者         ：野火嵌入式开发工作室
 * 淘宝店       ：http://firestm32.taobao.com
 * 技术支持论坛 ：http://www.ourdev.cn/bbs/bbs_list.jsp?bbs_id=1008
**********************************************************************************/



#include "common.h"
#include "include.h"


extern volatile u32 LPT_INT_count;
extern volatile u8  pit_flag;
#if 0
/*************************************************************************
*                             野火嵌入式开发工作室
*                   LPT脉冲计数中断实验（利用FTM产生PWM脉冲波）
*
*  实验说明：利用 FTM 产生PWM脉冲，LPT脉冲计数
*
*  实验操作：短接 PTC5 和 PTA8
*
*  实验效果：串口输出计数值，效果现象：
*                野火kinetis核心板测试程序
*        内核频率：200MHz	总线频率 ：100MHz
*        flex频率：100MHz 	flash频率：28MHz
*
*        Core Lockup Event Reset
*
*        野火Kinetis开发板启动方式：flash启动
*
*        K60-144pin      Silicon rev 1.2
*        Flash parameter version 0.0.7.0
*        Flash version ID 3.2.7.0
*        512 kBytes of P-flash	P-flash only
*        128 kBytes of RAM
*
*        LPT产生一次中断啦:100
*        LPT产生一次中断啦:100
*        ……
*
*  实验目的：明白如何用脉冲计数函数
*
*  修改时间：2012-3-16     已测试
*
*  备    注：可以修改FTM频率来发现频率越快，LPT产生中断越快
*************************************************************************/
#define INT_COUNT  100          //LPT 产生中断的计数次数
void  main(void)
{
    u16 count;

    DisableInterrupts;                                      //禁止总中断

    FTM_PWM_init(FTM1, CH0, 100, 50);                       //FTM模块产生PWM，用 FTM1_CH0 ，即 PTA8 ，频率为 100

    lptmr_counter_init(LPT0_ALT2, INT_COUNT, 2, LPT_Rising); //初始化脉冲计数器，用LPT0_ALT2，即PTC5输入，每隔INT_COUNT产生中断，延时2个时钟滤波，上升沿触发

    EnableInterrupts;                                       //开总中断

    while(1)
    {
        if( LPT_INT_count > 0 )
        {
            count           =   LPTMR0_CNR;                         //保存脉冲计数器计算值
            lptmr_counter_clean();                                  //清空脉冲计数器计算值（马上清空，这样才能保证计数值准确）
            printf("LPT产生一次中断啦:%d\n", LPT_INT_count * INT_COUNT +  count); //打印计数值
            LPT_INT_count   =   0;                                  //清空LPT中断次数
        }
    }
}

#else
/*************************************************************************
*                             野火嵌入式开发工作室
*                   PIT定时读取LPT脉冲计数实验（利用FTM产生PWM脉冲波）
*
*  实验说明：利用 FTM 产生PWM脉冲，LPT脉冲计数，PIT定时中断读取计数
*
*  实验操作：短接 PTC5 和 PTA8
*
*  实验效果：串口输出计数值，效果现象：
*                野火kinetis核心板测试程序
*        内核频率：200MHz	总线频率 ：100MHz
*        flex频率：100MHz 	flash频率：28MHz
*
*        Core Lockup Event Reset
*
*        野火Kinetis开发板启动方式：flash启动
*
*        K60-144pin      Silicon rev 1.2
*        Flash parameter version 0.0.7.0
*        Flash version ID 3.2.7.0
*        512 kBytes of P-flash	P-flash only
*        128 kBytes of RAM
*
*        1秒钟LPT读取脉冲:81个                 第一个不稳定
*        1秒钟LPT读取脉冲:199个
*        1秒钟LPT读取脉冲:199个
*        1秒钟LPT读取脉冲:199个
*        ……
*
*  实验目的：明白如何定时统计脉冲计数
*
*  修改时间：2012-3-16     已测试
*
*  备    注：可以修改FTM频率，1秒时间的脉冲计数与频率非常相近（有误差）
*************************************************************************/
#define INT_COUNT  100
void main()
{
    u16 count;

    DisableInterrupts;                                                              //禁止总中断

    FTM_PWM_init(FTM1, CH0, 200, 50);                                               //FTM模块产生PWM，用 FTM1_CH0 ，即 PTA8 ，频率为 200

    lptmr_counter_init(LPT0_ALT2, INT_COUNT, 2, LPT_Rising);                        //初始化脉冲计数器，用LPT0_ALT2，即PTC5输入，每隔INT_COUNT产生中断，延时2个时钟滤波，上升沿触发

    pit_init_ms(PIT0, 1000);                                                        //定时 1秒 中断

    EnableInterrupts;                                                               //开总中断

    while(1)
    {
        if( pit_flag > 0 )                                                          /*   1 秒中断了  */
        {
            count = LPTMR0_CNR;                                                     //保存脉冲计数器计算值
            lptmr_counter_clean();                                                  //清空脉冲计数器计算值（马上清空，这样才能保证计数值准确）
            printf("1秒钟LPT读取脉冲:%d个\n", LPT_INT_count * INT_COUNT +  count);  //读取间隔1秒的脉冲次数
            LPT_INT_count = 0;                                                      //清空LPT中断次数
            pit_flag = 0;                                                           //清空pit中断标志位
        }
    }
}

#endif

