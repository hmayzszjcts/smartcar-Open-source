/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		KEA128_systick
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.23
 * @Target core		S9KEA128AMLK
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-11-6
 ********************************************************************************************************************/

#include "KEA128_systick.h"

//-------------------------------------------------------------------------------------------------------------------
//  @brief      systick延时函数
//  @param      time            需要延时的时间
//  @return     void
//  @since      v2.0
//  Sample usage:               systick_delay(1000);   //延时1000个内核时钟周期
//-------------------------------------------------------------------------------------------------------------------
void systick_delay(uint32 time)
{
    if(time == 0)   return;
    SysTick->CTRL = 0x00;//先关了 systick ,清标志位
    SysTick->LOAD = time;//设置延时时间
    SysTick->VAL = 0x00;//清空计数器
    SysTick->CTRL = ( 0 | SysTick_CTRL_ENABLE_Msk     //使能 systick
                  //| SysTick_CTRL_TICKINT_Msk        //使能中断 (注释了表示关闭中断)
                    | SysTick_CTRL_CLKSOURCE_Msk      //时钟源选择 (core clk)
                );
    while( !(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));//等待时间到
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      毫秒级systick延时函数
//  @param      ms              延时多少毫秒
//  @return     void
//  @since      v2.0
//  Sample usage:               systick_delay_ms(1000);   //延时1000毫秒
//-------------------------------------------------------------------------------------------------------------------
void systick_delay_ms(uint32 ms)
{
    //get_clk();//获取内核时钟便于后面设置
	while(ms--) systick_delay(core_clk_khz);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      systick定时器
//  @param      time            定时时间(0-0x00ffffff)
//  @return     void
//  @since      v2.0
//  Sample usage:               systick_timing(1000);   //定时1000个内核时钟周期
//-------------------------------------------------------------------------------------------------------------------
void systick_timing(uint32 time)
{
    time &= 0x00ffffff;     	//数据强制变换为24位 
    SysTick->LOAD = time;       //设置延时时间
    SysTick->VAL = 0x00;       	//清空计数器
    SysTick->CTRL = ( 0 
					| SysTick_CTRL_ENABLE_Msk       //使能 systick
                    | SysTick_CTRL_TICKINT_Msk      //使能中断
                    | SysTick_CTRL_CLKSOURCE_Msk    //时钟源选择 (core clk) 
                );
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      获得当前System tick timer的值
//  @return     返回当前System tick timer的值
//  @since      v2.0
//  Sample usage:               uint32 tim = systick_getval();   
//-------------------------------------------------------------------------------------------------------------------
uint32 systick_getval(void)
{
    return SysTick->VAL;
}

