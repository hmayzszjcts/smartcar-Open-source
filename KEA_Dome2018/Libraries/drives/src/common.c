/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		common
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.23
 * @Target core		S9KEA128AMLK
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-11-6
 ********************************************************************************************************************/

#include "common.h"
#include "system_SKEAZ1284.h"

#define EX_REF_CLK  16000 //(定义外部参考时钟为16000KHZ)
uint32 ics_clk_khz;
uint32 ftm_clk_khz;
uint32 core_clk_khz;
uint32 bus_clk_khz;



void get_clk(void)
{
    uint16 prescaler_factor[]={1,2,4,8,16,32,64,128,256,512,1024};
    
    if(2 == KEA128_CLOCK)   //内部时钟
    {
        ics_clk_khz = 48000;
        ftm_clk_khz = 48000;
        core_clk_khz = 48000;
        bus_clk_khz = 24000; 
    }
    else                    //外部时钟
    {
        ics_clk_khz  = EX_REF_CLK * 1280 ;
        ics_clk_khz = ics_clk_khz / prescaler_factor[((ICS->C1&ICS_C1_RDIV_MASK) >> ICS_C1_RDIV_SHIFT) + 5];    //计算FLL输出频率
        ics_clk_khz = ics_clk_khz / prescaler_factor[(ICS->C2&ICS_C2_BDIV_MASK) >> ICS_C2_BDIV_SHIFT];          //计算ICSCLKOUT
        
        core_clk_khz = ics_clk_khz  / (((SIM->CLKDIV&SIM_CLKDIV_OUTDIV1_MASK) >> SIM_CLKDIV_OUTDIV1_SHIFT) + 1);
        bus_clk_khz  = core_clk_khz / (((SIM->CLKDIV&SIM_CLKDIV_OUTDIV2_MASK) >> SIM_CLKDIV_OUTDIV2_SHIFT) + 1);
        ftm_clk_khz  = ics_clk_khz  / (((SIM->CLKDIV&SIM_CLKDIV_OUTDIV3_MASK) >> SIM_CLKDIV_OUTDIV3_SHIFT) + 1);
    }

    uart_init(DEBUG_PORT,DEBUG_BAUD);   //初始化调试串口，如果不使用printf可以屏蔽
}
