/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名       ：main.c
 * 描述         ：UART串口中断接收实验
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
*                             串口中断接收实验测试
*
*  实验说明：野火串口中断接收实验
*            野火串口默认为： UART1，TX接PTC4，RX接PTC3
*            k60_fire.h里定义了 printf 函数的输出设置：
*                #define FIRE_PORT           UART1
*                #define FIRE_BAUD           19200
*            这里的串口接收实验也使用 UART1 ，这样就可以使用 printf 函数。
*            这里用到串口1的中断服务函数 USART1_IRQHandler();
*            我们需要在 isr.c 里编写 USART1_IRQHandler 中断服务函数
*            在 isr.h 里面重新宏定义中断号，重映射中断向量表里的中断函数地址
*
*  实验操作：打开串口助手 ，设置波特率为 19200 。
*            串口线（经过MAX3232电平转换）：TX接PTC4，RX接PTC3 。
*            运行程序后，要在串口助手里，根据提示发送数据，才能接收到数据。
*
*
*  实验效果：可以看到流水灯正常运行着。
*            在串口助手里，可以看到输出如下信息：
*
*
*                     野火kinetis核心板测试程序
*       内核频率：200MHz	总线频率 ：66MHz
*       flex频率：66MHz 	flash频率：28MHz
*
*       Software Reset
*
*       K60-144pin      Silicon rev 1.2
*       Flash parameter version 0.0.7.0
*       Flash version ID 3.2.7.0
*       512 kBytes of P-flash	P-flash only
*       128 kBytes of RAM
*
*       串口中断接收实验
*
*       		——野火kinetis开发板
*
*       请发送数据：
*
*       你发送的数据为：f
*       你发送的数据为：i
*       你发送的数据为：r
*       你发送的数据为：e
*
*
*  实验目的：测试串口中断接收
*
*  修改时间：2012-2-29     已测试
*
*  备    注：
*************************************************************************/



/*
* 这里用到串口1的中断服务函数 UART1_ISR();
* 接收到数据后就发送出去
*/
void  main(void)
{

    LED_INIT();                                   //LED初始化

    UART_IRQ_DIS(UART1);                          //串口1   关接收中断

    uart_init(UART1, 19200);                      //初始化串口1

    printf("串口中断接收实验\n\n");
    printf("\t\t——野火kinetis开发板\n\n");
    printf("请发送数据：\n");

    UART_IRQ_EN(UART1);                           //串口1   开接收中断

    while(1)
    {
        water_lights();                           //流水灯,用来指示系统正在运行
    }
}