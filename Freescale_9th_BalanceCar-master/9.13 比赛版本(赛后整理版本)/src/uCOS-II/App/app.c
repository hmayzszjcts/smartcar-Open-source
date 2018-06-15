/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名       ：app.c
 * 描述         ：用户任务函数，目前只有一个启动任务
 *
 * 实验平台     ：野火kinetis开发板
 * 库版本       ：
 * 嵌入系统     ：uC/OS-II V2.92
 *
 * 作者         ：野火嵌入式开发工作室
 * 淘宝店       ：http://firestm32.taobao.com
 * 技术支持论坛 ：http://www.ourdev.cn/bbs/bbs_list.jsp?bbs_id=1008
**********************************************************************************/

#include <app.h>
#include  "ucos_ii.h"  		//uC/OS-II系统函数头文件
#include  "LED.h"
#include  "sysinit.h"          //系统配置
#include <BSP.h>


/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：Task_Start
*  功能说明：启动任务,初始化滴答时钟
*  参数说明：p_arg       没用
*  函数返回：无
*  修改时间：2012-2-10
*  备    注：
*************************************************************************/
void Task_Start(void *p_arg)
{
    /**************** 初始化 系统滴答时钟 ：用于定时任务调度****************/
    u32  cnts;
    cnts         = core_clk_mhz * ( (u32)1000000u / (u32)OS_TICKS_PER_SEC );       /* Determine nbr SysTick increments.                    */
    OS_CPU_SysTickInit(cnts);                                                     /* Init uC/OS periodic time src (SysTick).              */


#if (OS_TASK_STAT_EN > 0)
    /*       开启统计任务           */
    OSStatInit();
#endif


    (void)p_arg;                				// 'p_arg' 并没有用到，防止编译器提示警告

    BSP_Init();                                                 //初始化开发板

    /******************* 用户任务区 ************************/

    while (1)
    {
        LED_turn(LED0);                                         //LED0闪烁
        OSTimeDlyHMSM(0, 0, 0, 100);
    }
}