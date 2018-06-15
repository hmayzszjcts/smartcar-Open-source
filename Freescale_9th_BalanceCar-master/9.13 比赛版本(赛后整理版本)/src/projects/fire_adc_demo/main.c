/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名       ：main.c
 * 描述         ：ADC模数转换实验
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
*                              ADC模数转换实验测试
*
*  实验说明：野火ADC模数转换实验，用串口发送转换后结果。
*            野火串口默认为： UART1，TX接PTC4，RX接PTC3
*            k60_fire.h里定义了 printf 函数的输出设置：
*                #define FIRE_PORT           UART1
*                #define FIRE_BAUD           19200
*
*  实验操作：这里用 ADC1_SE4a ，所以 ADC1_SE4a 还要接一个0~3.3V的可调电路
*            ADC1_SE4a 就是 PTE0 , 在 adc.h 文件里有表格
*            打开串口助手，设置波特率为 19200 。
*            串口线（经过MAX3232电平转换）：TX接PTC4，RX接PTC3
*
*  实验效果：在串口助手里，可以看到输出如下信息：
*
*                    野火kinetis核心板测试程序
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
*       AD转换一次的结果为:36983
*       AD转换三次的中值结果为:37143
*       AD转换十次的平均值结果为:36912
*
*
*
*  实验目的：测试 ADC 转换的各个结果
*
*  修改时间：2012-2-29     已测试
*
*  备    注：adc.h 有 各个ADC通道所对应管脚的表格，方便查看
*            如果悬空管脚，那测出的值会变化比较大
*************************************************************************/
void  main()
{
    u16 ADresult;                           //保存ADC转换结果

    uart_init(UART1, 19200);                //初始化串口，用来发送转换数据

    adc_init(ADC1, SE4a);                   //初始化ADC1_SE4a ,从adc.h里的表格就可以看到ADC1_SE4a对应为PTE0

    while(1)
    {
        /****** 读取一次 ******/
        ADresult    =   ad_once(ADC1, SE4a, ADC_16bit);         //读取 ADC1_SE4a ，16位精度
        printf("AD转换一次的结果为:%d\n\n", ADresult);

        //time_delay_ms(500);                //延时500ms

        /****** 读取三次，取中值 ******/
        ADresult    =   ad_mid(ADC1, SE4a, ADC_16bit);          //读取 ADC1_SE4a ，16位精度
        printf("AD转换三次的中值结果为:%d\n\n", ADresult);

        //time_delay_ms(500);                //延时500ms

        /****** 读取十次次，取平均值 ******/
        ADresult    =   ad_ave(ADC1, SE4a, ADC_16bit, 10);      //读取 ADC1_SE4a ，16位精度
        printf("AD转换十次的平均值结果为:%d\n\n", ADresult);

        ADresult    =   ad_flt(ADC1, SE4a, ADC_16bit);
        printf("AD转换与前几次采样均值滤波的平均值结果为:%d\n\n", ADresult);

        time_delay_ms(500);                //延时500ms
    }
}
