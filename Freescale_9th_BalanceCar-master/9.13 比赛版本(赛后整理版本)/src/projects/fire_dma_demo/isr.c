/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名       ：isr.c
 * 描述         ：中断处理例程
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

volatile u8  pit_flag = 0;
volatile u32 dma_int_count = 0;


/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：DMA_CH4_Handler
*  功能说明：DMA通道4的中断服务函数
*  参数说明：无
*  函数返回：无
*  修改时间：2012-3-18    已测试
*  备    注：
*************************************************************************/
void DMA_CH4_Handler(void)
{
    //DMA通道4
    DMA_IRQ_CLEAN(DMA_CH4);                             //清除通道传输中断标志位    (这样才能再次进入中断)
    DMA_EN(DMA_CH4);                                    //使能通道CHn 硬件请求      (这样才能继续触发DMA传输)

    dma_int_count++;
}

void PIT0_IRQHandler()
{
    printf("PIT0_IRQHandler\n");
    pit_flag = 1;               //标记进入PIT中断
    PIT_Flag_Clear(PIT0);       //清中断标志位
    PIT_MCR = 0;  // add for correcting PIT bug
}

void DMA_CH0_Handler(void)
{

    //DMA通道0
    printf("DMA_CH0_Handler\n");
    DMA_IRQ_CLEAN(DMA_CH0);                             //清除通道传输中断标志位    (这样才能再次进入中断)
    DMA_EN(DMA_CH0);                                    //使能通道CHn 硬件请求      (这样才能继续触发DMA传输)

}

extern u8 BUFF[];
void PIT1_IRQHandler()
{
    u8 i;
    printf("PIT1_IRQHandler\n");
    printf("\nBUFF[10]={");
    for(i = 0; i < 9; i++)
        printf("%d,", BUFF[i]);
    printf("%d}\n", BUFF[10]);
    //printf("\n")

    PIT_Flag_Clear(PIT1);       //清中断标志位
    PIT_MCR = 0;  // add for correcting PIT bug
}