/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       VCAN_MK60_conf.h
 * @brief      山外K60 平台配置文件
 * @author     山外科技
 * @version    v5.0
 * @date       2013-06-26
 */

#ifndef __VCAN_MK60_CONF_H__
#define __VCAN_MK60_CONF_H__

#include    "MKL_mcg.h"


/*
 * 选择是否输出调试信息，不输出则注释下面的宏定义
 */
//#define DEBUG_PRINT

/*
 * 定义晶振时钟，单位为MHz
 */
#define EXTAL_IN_MHz            (8)

/*
 * 定义 PLL 超频 频率
 */
#define PLL_CLK                PLL200      // 从 PLL_e 里选择 已提供的 配置方案
                                          // core/bus 频率 都是 pll 整数倍分频而来

#define MAX_CORE_CLK            120        // core     (bus        >= pll/16  )
#define MAX_BUS_CLK             40         // bus      (bus        >= core/16  )



/*********************   自定义 时钟频率 分频因子   ********************/
//如果 CORE_CLK 为 PLLUSR ，则为自定义模式 ，如下配置生效
#define PRDIV             10        // MCG_CLK_MHZ = EXTAL_IN_MHz/(PRDIV+1)*(VDIV+24)
#define VDIV              29
#define CORE_DIV          0         //  core = mcg/ ( CORE_DIV  + 1 )
#define BUS_DIV           1         //  bus  = mcg/ ( BUS_DIV   + 1 )



/*
 * 定义 printf函数 的 串口输出端口 和 串口信息
 */
#define VCAN_PORT           UART0
#define VCAN_BAUD           115200

/*
 * 配置延时函数
 */
#if  0
#include "MKL_lptmr.h"
#define     DELAY()         lptmr_delay_ms(500)
#define     DELAY_MS(ms)    lptmr_delay_ms(ms)
#define     DELAY_US(us)    lptmr_delay_us(us)
#elif   1
#include "MKL_pit.h"
#define DELAY()         pit_delay_ms(PIT1,500)
#define DELAY_MS(ms)    pit_delay_ms(PIT1,ms)
#define DELAY_US(us)    pit_delay_us(PIT1,us)
#else
#include "MKL_SysTick.h"
#define DELAY()         systick_delay_ms(500)
#define DELAY_MS(ms)    systick_delay_ms(ms)
#define DELAY_US(us)    systick_delay_us(us)
#endif


/*
 * 配置断言和其实现函数
 */
void assert_failed(char *, int);

#if ( defined( DEBUG ) && defined( DEBUG_PRINT ))
#define ASSERT(expr) \
    if (!(expr)) \
        assert_failed(__FILE__, __LINE__)
#else
#define ASSERT(expr)
#endif

/*
 * 配置调试输出函数
 */
#if( defined(DEBUG) && defined(DEBUG_PRINT))
#define DEBUG_PRINTF(FORMAT,...)        do{printf(FORMAT,##__VA_ARGS__);}while(0)	/*无需打印调试信息时，请将宏内容注释掉*/
#else
#define DEBUG_PRINTF(FORMAT,...)
#endif

/*
 * 启动前检测，防止 复用下载口
 */
void start_check();
#ifdef DEBUG
#define SRART_CHECK()       start_check()
#else
#define SRART_CHECK()
#endif


#endif /* __VCAN_MK60_CONF_H__  */
