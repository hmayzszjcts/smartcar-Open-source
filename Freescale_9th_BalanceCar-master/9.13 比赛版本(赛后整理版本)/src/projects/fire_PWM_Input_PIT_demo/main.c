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
*                        PWM、输入捕捉、PIT中断综合测试
*
*  实验说明：野火 PWM、输入捕捉、PIT中断综合测试。
*            利用 FTM2_CH0 产生PWM脉冲波，
*            FTM1_CH0 输入捕捉 FTM2_CH0 产生的PWM脉冲波，每次捕捉到就计数。
*            PIT每隔1s产生一次中断，用串口发送脉冲的计数，并清空计数。
*
*  实验操作：短接 PTA8 和 PTA10 ，即 FTM2_CH0 和 FTM1_CH0 短接。
*            打开串口助手，设置波特率为19200
*
*  实验效果：串口输出脉冲计数：
*
*  		野火kinetis核心板测试程序
*       内核频率：100MHz	总线频率 ：50MHz
*       flex频率：50MHz 	flash频率：25MHz
*
*       Software Reset
*
*       K60-144pin      Silicon rev 1.2
*       Flash parameter version 0.0.7.0
*       Flash version ID 3.2.7.0
*       512 kBytes of P-flash	P-flash only
*       128 kBytes of RAM
*
*
*       接收到 100 个脉冲
*       接收到 100 个脉冲
*       接收到 100 个脉冲
*       ......
*
*
*  实验目的：综合测试 PWM、输入捕捉、PIT中断
*
*  修改时间：2012-2-29     已测试
*
*  备    注：FTM.h 里有各个FTM通道所对应管脚的表格，方便查看
*            这里用到了中断，在 isr.h 和 isr.c 可以看到中断服务函数
*************************************************************************/
volatile u32 pwmtest = 0;                        //用来计数
void main()
{
    DisableInterrupts;                           //禁止总中断

    FTM_Input_init(FTM1, CH0, Rising);           //初始化FTM输入捕捉模式，上升沿触发

    FTM_PWM_init(FTM2, CH0, 100, 50);            //PTA10       100Hz

    pit_init_ms(PIT0, 1000);                     //1s产生一次中断

    EnableInterrupts;	                         //开总中断

    while(1)
    {
    }
}
