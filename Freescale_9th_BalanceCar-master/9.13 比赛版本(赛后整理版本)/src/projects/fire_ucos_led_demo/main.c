/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名       ：main.c
 * 描述         ：这是个用户模版，已经添加好所有的库，用户自己调用库里函数即可
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
*                               LED实验综合测试
*  实验说明：野火 ucos LED 实验
*            Task_Start 里面不断闪烁 LED0
*
*  实验操作：无
*
*  实验效果：LED0 闪烁
*
*  实验目的：测试 ucos 系统是否运行起来
*
*  修改时间：2012-2-29     已测试
*
*  备    注：野火Kinetis开发板的 LED0~3 ，分别接PTD15~PTD12 ，低电平点亮
*************************************************************************/
OS_STK startup_task_stk[STARTUP_TASK_STK_SIZE];		    //定义任务Task_Start的栈
void main()
{

    OSInit();                                               //初始化uC/OS
    OSTaskCreate(Task_Start, (void *)0,
                 &startup_task_stk[STARTUP_TASK_STK_SIZE-1], STARTUP_TASK_PRIO);    //创建任务

    OSStart();                                              //启动多任务管理
}

