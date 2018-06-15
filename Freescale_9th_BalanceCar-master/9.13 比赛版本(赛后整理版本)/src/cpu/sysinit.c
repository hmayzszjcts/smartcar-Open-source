/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名       ：sysinit.c
 * 描述         ：k60的初始化函数
 * 备注         ：在官方基础上修改
 *
 * 实验平台     ：野火kinetis开发板
 * 库版本       ：
 * 嵌入系统     ：
 *
 * 作者         ：
 * 淘宝店       ：http://firestm32.taobao.com
 * 技术支持论坛 ：http://www.ourdev.cn/bbs/bbs_list.jsp?bbs_id=1008
**********************************************************************************/


#include "common.h"
#include "sysinit.h"
#include "uart.h"


/********************************************************************/

/* Actual system clock frequency */
u32 core_clk_khz;       //内核时钟(KHz)
u32 core_clk_mhz;       //内核时钟(MHz)
u32 bus_clk_khz;        //外围总线时钟



/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：sysinit
*  功能说明：开发板初始化函数，设置锁相环，初始化时钟 和 printf串口
*  参数说明：无
*  函数返回：无
*  修改时间：2012-2-10
*  备    注：
*************************************************************************/
void sysinit (void)
{
    /* 使能所有端口PORT时钟   */
    SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK
                  | SIM_SCGC5_PORTB_MASK
                  | SIM_SCGC5_PORTC_MASK
                  | SIM_SCGC5_PORTD_MASK
                  | SIM_SCGC5_PORTE_MASK );

#if defined(NO_PLL_INIT)
    core_clk_mhz = 20;                      //如果定义不初始化锁相环，则进入 FEI mode
#else
    /* 增强系统频率   可以在这里设置超频  */
    core_clk_mhz = pll_init(MCG_CLK_MHZ);
#endif

    //通过pll_init函数的返回值来计算内核时钟和外设时钟，便于其他函数可查询时钟频率
    core_clk_khz = core_clk_mhz * 1000;
    bus_clk_khz = core_clk_khz / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> 24) + 1);

    //    trace_clk_init();                       //使能跟踪时钟，用于调试
    //    fb_clk_init();                          //FlexBus时钟初始化

    uart_init(FIRE_PORT, FIRE_BAUD);    //初始化 printf 函数所用到的串口
}



//-------------------------------------------------------------------------*
//函数名: trace_clk_init                                                   *
//功  能: 跟踪时钟初始化                                                   *
//参  数: 无							  	   *
//返  回: 无                                                               *
//说  明: 用于调试                                                         *
//-------------------------------------------------------------------------*
void trace_clk_init(void)
{
    /* Set the trace clock to the core clock frequency */
    SIM_SOPT2 |= SIM_SOPT2_TRACECLKSEL_MASK;

    /* Enable the TRACE_CLKOUT pin function on PTA6 (alt7 function) */
    PORTA_PCR6 = ( PORT_PCR_MUX(0x7));
}
/********************************************************************/

//-------------------------------------------------------------------------*
//函数名: fb_clk_init                                                      *
//功  能: FlexBus时钟初始化                                                *
//参  数: 无								   *
//返  回: 无                                                               *
//说  明:                                                                  *
//-------------------------------------------------------------------------*
void fb_clk_init(void)
{
    /* Enable the clock to the FlexBus module */
    SIM_SCGC7 |= SIM_SCGC7_FLEXBUS_MASK;

    /* Enable the FB_CLKOUT function on PTC3 (alt5 function) */
    PORTC_PCR3 = ( PORT_PCR_MUX(0x5));
}
/********************************************************************/
