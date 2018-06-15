/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名       ：main.c
 * 描述         ：EXTI外部中断实验
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
*                           EXTI外部中断实验综合测试
*
*  实验说明：野火EXTI外部中断实验
*            利用PTA27产生方波，触发 PTA26 外部中断，中断服务函数闪烁LED0。
*
*            PTA26 是 PORTA 端口 ，外部触发中断是PORTA中断：PORTA_IRQHandler
*            我们需要在 isr.c 里编写 PORTA_IRQHandler 中断服务函数
*            在 isr.h 里面重新宏定义中断号，重映射中断向量表里的中断函数地址
*
*  实验操作：用跳线短接 PTA27 和 PTA26
*
*  实验效果：LED0闪烁（中断服务函数里控制）
*
*  实验目的：测试 exti_init 函数
*
*  修改时间：2012-2-28     已测试
*
*  备    注：野火Kinetis开发板的 LED0~3 ，分别接PTD15~PTD12 ，低电平点亮
*************************************************************************/
void main()
{
    LED_INIT();                             //初始化LED，PORTA_IRQHandler中断用到LED

    gpio_init(PORTA, 27, GPO, 1);           //初始化gpio , PTA27设为输出，以便产生方波

    exti_init(PORTA, 26, rising_down);      //PORTA26 端口外部中断初始化 ，上升沿触发中断，内部下拉

    while(1)
    {
        GPIO_TURN(PORTA, 27);               //翻转PTA27，即产生方波

        time_delay_ms(500);                 //延时0.5s
    }
}





