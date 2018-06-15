/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名       ：port.c
 * 描述         ：PORT外部GPIO中断例程
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
#include "gpio.h"
#include "port.h"



/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：port_init
*  功能说明：PORT外部GPIO中断初始化
*  参数说明：PTxn        端口
*            cfg         端口属性配置，如触发选项和上拉下拉选项
*  函数返回：无
*  修改时间：2013-1-19
*  备    注：
*************************************************************************/
void  port_init(PTxn ptxn, u32 cfg )
{
    
    u8 portx = ptxn>>5;
    u8 n = ptxn & 0x1f;
    SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK << portx);    //开启PORTx端口
    
    PORT_PCR_REG(PORTX[portx], n) = PORT_PCR_MUX(1) | cfg; // 复用GPIO , 确定触发模式 ,开启上拉或下拉电阻
    GPIO_PDDR_REG(GPIOx[portx]) &= ~(1 << n);       //输入模式
    //enable_irq(portx + 87);                         //使能PORT中断，PORTA的ISR中断号为87
}