/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名       ：main.c
 * 描述         ：DMA操作实验
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

#if 0

u8 BUFF[11];

/*************************************************************************
*                             野火嵌入式开发工作室
*                             DMA传输GPIO数据实验
*
*  实验说明：DMA传输GPIO数据实验，利用串口来发送命令，控制 PTA26 产生脉冲波，
*            而短接PTA26和PTA27，即PTA27读取到上升沿，从而触发DMA传输。
*
*  实验操作：短接PTA26和PTA27 ，在串口助手里随便发送一个字符就可以使PTA26产生
*            脉冲波，断点设置在调用uart_getchar 或者 DELAY_MS 函数那里，把
*            BUFF 放在watch里这样每次触发后，在watch里可以看到 BUFF 数组里元
*            素的值改变了。
*
*  实验效果：每次串口接收到一个字节，就会触发DMA中断，从而保存数据到 BUFF数组
*            里，在DEBUG界面的watch串口，可以看到BUFF数组的元素发送变化。
*
*  实验目的：学会使用 DMA_PORTx2BUFF_Init 函数来达到 DMA 传输目的
*
*  修改时间：2012-3-24     已测试
*
*  备    注：悬空输入管脚，因为 DMA_PORTx2BUFF_Init 设置源IO管脚内部下拉，
*            所以只会读取到 0
*************************************************************************/
void  main(void)
{
    char command;

    gpio_init  (PORTA, 26, GPO, LOW);         //初始化 PTA26 为输出低电平

    DMA_PORTx2BUFF_Init (DMA_CH4, (void *)&PTD_BYTE0_IN, BUFF, PTA27, DMA_BYTE1, 3, DMA_rising);
    //DMA通道4初始化，PTA27上升沿触发DMA传输，源地址为PTD_BYTE0_IN，目的地址为：BUFF ，每次传输1Byte，传输3次后停止传输，恢复目的地址

    while(1)
    {
        command = uart_getchar(FIRE_PORT);  //通过串口来控制PTA26的输出，即PTA27的输入
                                            //从而控制触发DMA传输

        if(command)                         //接收到数据就翻转PTA26两次，产生一个脉冲波，DMA就传输一次
        {
            PTA26_OUT = ~PTA26_OUT;         //取反
            PTA26_OUT = ~PTA26_OUT;         //取反
        }

        DELAY_MS(50);                       //延时一下，这步骤可以删掉
    }
}


#elif  0

void  main(void)
{
    char command;

    gpio_init  (PORTA, 26, GPO, LOW);         //初始化 PTA26 为输出低电平

    DMA_count_Init(DMA_CH4, PTA27 , 0x4, DMA_rising);

    while(1)
    {
        command = uart_getchar(FIRE_PORT);  //通过串口来控制PTA26的输出，即PTA27的输入
                                            //从而控制触发DMA传输

        if(command)                         //接收到数据就翻转PTA26两次，产生一个脉冲波，DMA就传输一次
        {
            PTA26_OUT = ~PTA26_OUT;         //取反
            PTA26_OUT = ~PTA26_OUT;         //取反
        }
        /*      //
                if(DMA_count_get(DMA_CH4)==2)
                {
                    DMA_count_reset(DMA_CH4);
                }
        */
        DELAY_MS(50);                       //延时一下，这步骤可以删掉
    }
}

#else
extern volatile u8  pit_flag ;
extern volatile u32 dma_int_count;
#define INT_COUNT  100
void main()
{
    u16 count;

    DisableInterrupts;                                                              //禁止总中断

    FTM_PWM_init(FTM1, CH0, 30000, 50);                                               //FTM模块产生PWM，用 FTM1_CH0 ，即 PTA8 ，频率为 200

    //lptmr_counter_init(LPT0_ALT2,INT_COUNT,2,LPT_Rising);                         //初始化脉冲计数器，用LPT0_ALT2，即PTC5输入，每隔INT_COUNT产生中断，延时2个时钟滤波，上升沿触发
    DMA_count_Init(DMA_CH4, PTC5 , 0x7FFF, DMA_rising);

    pit_init_ms(PIT0, 1000);                                                        //定时 1秒 中断

    EnableInterrupts;                                                               //开总中断

    while(1)
    {
        if( pit_flag > 0 )                                                          /*   1 秒中断了  */
        {
            //count=LPTMR0_CNR;                                                       //保存脉冲计数器计算值
            //lptmr_counter_clean();                                                  //清空脉冲计数器计算值（马上清空，这样才能保证计数值准确）
            count = DMA_count_get(DMA_CH4);
            DMA_count_reset(DMA_CH4);
            //printf("1秒钟LPT读取脉冲:%d个\n",LPT_INT_count*INT_COUNT +  count);     //读取间隔1秒的脉冲次数
            printf("1秒钟LPT读取脉冲:%d个\n", count);     //读取间隔1秒的脉冲次数
            //LPT_INT_count=0;                                                        //清空LPT中断次数
            dma_int_count = 0;
            pit_flag = 0;                                                           //清空pit中断标志位
        }
    }
}


#endif