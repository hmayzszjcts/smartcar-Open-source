/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名       ：main.c
 * 描述         ：PWM实验
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



/*************************************************************************
*                             野火嵌入式开发工作室
*                             FTM 输入捕捉中断测试
*
*  实验说明：野火 FTM 输入捕捉中断实验，用LED显示是否进入了中断。
*
*  实验操作：这里用 FTM1_CH0 输入捕捉 ，即 PTA8 管脚。
*            PTA9 产生方波 ，把 PTA9 和 PTA8 短接 ，
*            即PTA9产生的上升沿来触发 FTM1_CH0 中断
*
*  实验效果：LED0闪烁
*
*  实验目的：测试  FTM1_CH0 输入捕捉功能
*
*  修改时间：2012-2-29     已测试
*
*  备    注：FTM.h 里有各个FTM通道所对应管脚的表格，方便查看
*
*************************************************************************/
void main()
{
    DisableInterrupts;            //禁止总中断

    gpio_init(PORTA, 9, GPO, HIGH);               //初始化PTA9，输出高电平
    LED_INIT();                                  //初始化LED, FTM1_IRQHandler 中断函数里闪烁LED0
    FTM_Input_init(FTM1, CH0, Rising);           //初始化FTM输入捕捉模式，上升沿触发

    EnableInterrupts;	                         //开总中断

    while(1)
    {
        gpio_set(PORTA, 9, LOW);                //PTA9产生低电平

        time_delay_ms(250);                     //延时一下

        gpio_set(PORTA, 9, HIGH);               //PTA9产生高电平

        time_delay_ms(250);                     //延时一下
    }
}
