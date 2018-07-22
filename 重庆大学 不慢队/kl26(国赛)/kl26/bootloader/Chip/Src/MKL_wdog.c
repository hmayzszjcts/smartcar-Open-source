/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MK60_wdog.c
 * @brief      看门狗驱动函数
 * @author     山外科技
 * @version    v5.0
 * @date       2013-07-02
 */

#include "common.h"
#include "MKL_wdog.h"


static  uint32 wdog_cfg = 0;

/*!
 *  @brief      初始化看门狗，设置喂狗时间
 *  @param      cfg     喂狗时间
 *  @since      v5.0
 */
void wdog_init(wdog_cfg_e cfg)
{

    wdog_cfg = cfg;
    SIM_COPC = wdog_cfg;
}


/*!
 *  @brief      启用看门狗
 *  @since      v5.0
 */
void wdog_enable(void)
{
    SIM_COPC = wdog_cfg;
}


/*!
 *  @brief      禁用看门狗
 *  @since      v5.0
 */
void wdog_disable(void)
{
    wdog_cfg =  SIM_COPC;

    SIM_COPC &= ~SIM_COPC_COPT_MASK;
}

/*!
 *  @brief      喂狗
 *  @since      v5.0
 */
void wdog_feed(void)
{
    SIM_SRVCOP = 0x55UL;
    SIM_SRVCOP = 0xAAUL;
}
