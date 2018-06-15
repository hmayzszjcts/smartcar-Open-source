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
*************************************************************************/
void  main()
{
    u32 DeviceID, FlashID;

    w25x16_init();

    while(1)
    {
        DeviceID = w25x16_ReadDeviceID();
        printf("\nw25x16的DeviceID为：0x%x", DeviceID);

        //FlashID=w25x16_ReadFlashID();
        //printf("\nw25x16的FlashID为：0x%x",FlashID);

        //time_delay_ms(1000);                //延时1s
    }
}
