/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名       ：main.c
 * 描述         ：GPIO操作实验
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

#define    USR_VREF     VDDA      //这个宏定义是用来切换DAC参考电压的， VDDA 或 VREF_OUT


#if 0

/*************************************************************************
*                             野火嵌入式开发工作室
*                                DAC实验LED测试
*
*  实验说明：野火DAC实验LED测试，利用LED亮暗来指示输出电压的变化
*
*  实验操作：短接 DAC0 和 PTD15
*
*  实验效果：LED3由亮变暗，然后突然变亮
*
*  实验目的：说明DAC输出电压发生变化。
*
*  修改时间：2012-3-25     已测试
*
*  备    注：野火Kinetis开发板的 LED0~3 ，分别接PTD12~PTD15 ，低电平点亮
*************************************************************************/
void  main(void)
{
    u16 i = 0;

    dac_once_init(DAC0, USR_VREF);              //初始化DAC0

    while(1)
    {
        for(i = 0; i < 0xF00; i += 100)
        {
            dac_once_convert(DAC0, i);
            DELAY_MS(200);
        }
    }
}

#elif   0

/*************************************************************************
*                             野火嵌入式开发工作室
*                                DAC实验万用表测试
*
*  实验说明：野火DAC实验万用表测试，利用串口控制输出电压，用万用表测与理论值的差距。
*
*  实验操作：串口里发送 '+' 或 '-' ，然后用万用表测电压
*
*  实验效果：LED3由亮变暗，然后突然变亮
*
*  实验目的：测试实际测量值与理论值的差距。
*
*  修改时间：2012-3-25     已测试
*
*  备    注：
*************************************************************************/
void  main(void)
{
    u16 i = 0;
    u8  com = 0;

    dac_once_init(DAC0, USR_VREF);              //初始化DAC0

    while(1)
    {
        switch(com)                             //通过串口来控制
        {
        case '+':
            i += 500;
            break;
        case '-':
            i -= 500;
            break;
        default:
            break;
        }

        if(i >= 0x1000) i = 0;                  //12bit

        dac_once_convert(DAC0, i);              //DAC转换

        if(USR_VREF == VDDA)                    //两种参考电源选择方式
            printf("\n输出电压：%d mV\n", (u32)(3300 * (1 + i)) >> 12); //串口打印转换值
        else
            printf("\n输出电压：%d mV\n", (u32)(1200 * (1 + i)) >> 12); //串口打印转换值

        printf("\n请发送 '+' 或 '-' 来控制电压增减");
        com = uart_getchar(FIRE_PORT);          //等待串口命令
    }
}


#elif 1

/*************************************************************************
*                             野火嵌入式开发工作室
*                                DAC实验ADC测试
*
*  实验说明：野火DAC实验万用表测试，利用串口控制输出电压，用ADC读取比较。
*
*  实验操作：短接 ADC0 和 PTC0
*
*  实验效果：串口里可以看到：
*
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
*
*        请发送 '+' 或 '-' 来控制电压增减。发送其他则保存不变
*        DAC转换的值为：500
*        读取到ADC值为：444
*
*        请发送 '+' 或 '-' 来控制电压增减。发送其他则保存不变
*        DAC转换的值为：1000
*        读取到ADC值为：947
*
*        请发送 '+' 或 '-' 来控制电压增减。发送其他则保存不变
*        ……
*
*
*  实验目的：测试实际测量值与理论值的差距。
*
*  修改时间：2012-3-25     已测试
*
*  备    注：为什么会约50的误差？
*************************************************************************/
void  main(void)
{
    u16 i = 0;
    u16 ADresult;
    u8  com = 0;

    dac_once_init(DAC0, VDDA);              //初始化DAC0

    adc_init(ADC0, SE14);                       //初始化ADC0_SE14 ,从adc.h里的表格就可以看到ADC0_SE14对应为PTC0

    while(1)
    {

        switch(com)                             //通过串口来控制
        {
        case '+':
            i += 500;
            break;
        case '-':
            i -= 500;
            break;
        default:
            break;
        }

        if(i >= 0x1000) i = 0;                  //12bit

        dac_once_convert(DAC0, i);                              //DAC转换
        printf("\nDAC转换的值为：%d", i);                       //串口打印转换值

        ADresult = ad_once(ADC0, SE14, ADC_12bit);              //读取 ADC0_SE14 ，12位精度
        printf("\n读取到ADC值为：%d \n", ADresult);             //串口打印转换值

        printf("\n请发送 '+' 或 '-' 来控制电压增减。发送其他则保存不变");
        com = uart_getchar(FIRE_PORT);          //等待串口命令

    }
}


#endif

