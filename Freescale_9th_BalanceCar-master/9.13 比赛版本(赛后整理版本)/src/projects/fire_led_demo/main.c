/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名       ：main.c
 * 描述         ：LED操作实验
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


#if 0     //这里选择 0 或 1 来切换编译两个不同的main函数
/*************************************************************************
*                             野火嵌入式开发工作室
*                               LED实验简单测试
*  实验说明：野火LED实验，控制 LED 亮灭
*
*  实验操作：无
*
*  实验效果：LED0每隔500ms
*
*  实验目的：明白如何亮灭LED
*
*  修改时间：2012-2-28     已测试
*
*  备    注：野火Kinetis开发板的 LED0~3 ，分别接PTD15~PTD12 ，低电平点亮
*************************************************************************/
void  main(void)
{
    LED_init();                            //初始化 4 个LED

    while(1)
    {
        led(LED0, LED_ON);                 //LED0 亮

        time_delay_ms(500);                //延时500ms

        led(LED0, LED_OFF);                //LED0 灭

        time_delay_ms(500);                //延时500ms
    }
}

#elif  0
/*************************************************************************
*                             野火嵌入式开发工作室
*                               LED实验综合测试
*  实验说明：野火LED实验
*
*  实验操作：无
*
*  实验效果：LED0和LED1同时间隔500ms亮灭
*
*  实验目的：测试 led_init、led、led_turn 函数
*
*  修改时间：2012-2-28     已测试
*
*  备    注：野火Kinetis开发板的 LED0~3 ，分别接PTD15~PTD12 ，低电平点亮
*************************************************************************/
void  main(void)
{
    LED_init();                            //初始化 4 个LED

    while(1)
    {
        led(LED0, LED_ON);                 //LED0 亮
        LED_turn(LED1);                    //LED1翻转

        time_delay_ms(500);                //延时500ms

        led(LED0, LED_OFF);                //LED0 灭
        LED_turn(LED1);                    //LED1翻转

        time_delay_ms(500);                //延时500ms
    }
}

#else

void  main(void)
{
    LED_init();                            //初始化 4 个LED

    while(1)
    {
        water_lights();
    }
}

#endif




