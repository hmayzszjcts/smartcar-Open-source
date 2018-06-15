/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		KEA128_pit
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.23
 * @Target core		S9KEA128AMLK
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-11-6
 ********************************************************************************************************************/

#include "KEA128_pit.h"

//-------------------------------------------------------------------------------------------------------------------
//  @brief      pit初始化
//  @param      pitn            选择pit模块
//  @param      cnt             定时中断的时间
//  @return     void
//  @since      v2.0
//  Sample usage:               pit_init(pit0,1000);								//定时1000个bus时钟后中断
//								set_irq_priority(PIT_CH0_IRQn,1);					//设置优先级,根据自己的需求设置 可设置范围为 0 - 3  越小优先级越高
//								enable_irq(PIT_CH0_IRQn);							//打开pit0的中断开关
//								EnableInterrupts;									//打开总的中断开关
//-------------------------------------------------------------------------------------------------------------------
void pit_init(PITn pitn, uint32 cnt)
{
	SIM->SCGC       |= SIM_SCGC_PIT_MASK;        			//使能PIT时钟
	PIT->MCR         = 0;									//使能PIT定时器时钟 ，调试模式下继续运行
	PIT->CHANNEL[pitn].LDVAL  = cnt;              			//设置溢出中断时间
	PIT_FLAG_CLR(pitn);										//清除中断标志位
	PIT->CHANNEL[pitn].TCTRL &= ~ PIT_TCTRL_TEN_MASK;       //禁止PITn定时器（用于清空计数值）
    PIT->CHANNEL[pitn].TCTRL  = ( 0
								| PIT_TCTRL_TEN_MASK        //使能 PITn定时器
								| PIT_TCTRL_TIE_MASK        //开PITn中断
								);
	//enable_irq( (IRQn_Type)((int)pitn + PIT_CH0_IRQn) );       //开中断
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      pit初始化
//  @param      pitn            选择pit模块
//  @param      cnt             定时延时的时间
//  @return     void
//  @since      v2.0
//  Sample usage:               pit_init(pit0,1000);		//延时1000个bus时钟
//-------------------------------------------------------------------------------------------------------------------
void pit_delay(PITn pitn, uint32 cnt)
{
	SIM->SCGC       |= SIM_SCGC_PIT_MASK;        			//使能PIT时钟
	PIT->MCR         = 0;									//使能PIT定时器时钟 ，调试模式下继续运行
	PIT->CHANNEL[pitn].LDVAL  = cnt;              			//设置溢出中断时间
	PIT_FLAG_CLR(pitn);										//清除中断标志位
	PIT->CHANNEL[pitn].TCTRL &= ~ PIT_TCTRL_TEN_MASK;       //禁止PITn定时器（用于清空计数值）
    PIT->CHANNEL[pitn].TCTRL  = ( 0
								| PIT_TCTRL_TEN_MASK        //使能 PITn定时器
								//| PIT_TCTRL_TIE_MASK        //开PITn中断
								);
	while( !((PIT->CHANNEL[pitn].TFLG) & PIT_TFLG_TIF_MASK) ){}//等待时间到

    PIT_FLAG_CLR(pitn);                                     //清中断标志位
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      pit初始化
//  @param      pitn            选择pit模块
//  @return     void
//  @since      v2.0
//  Sample usage:               pit_time_start(pit0);		//pit0开始计时
//-------------------------------------------------------------------------------------------------------------------
void pit_time_start(PITn pitn)
{
	SIM->SCGC       |= SIM_SCGC_PIT_MASK;        			//使能PIT时钟
	PIT->MCR         = 0;									//使能PIT定时器时钟 ，调试模式下继续运行
	PIT->CHANNEL[pitn].LDVAL  = ~0;              			//设置溢出中断时间
	PIT_FLAG_CLR(pitn);										//清除中断标志位
	PIT->CHANNEL[pitn].TCTRL &= ~ PIT_TCTRL_TEN_MASK;       //禁止PITn定时器（用于清空计数值）
    PIT->CHANNEL[pitn].TCTRL  = ( 0
								| PIT_TCTRL_TEN_MASK        //使能 PITn定时器
								//| PIT_TCTRL_TIE_MASK        //开PITn中断
								);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      pit初始化
//  @param      pitn            选择pit模块
//  @return     void
//  @since      v2.0
//  Sample usage:               pit_time_get(pit0);			//获取当前pit0计时时间
//-------------------------------------------------------------------------------------------------------------------
uint32 pit_time_get(PITn pitn)
{
	uint32 val;
	val = (uint32)(~0) - PIT->CHANNEL[pitn].CVAL;
	if(PIT->CHANNEL[pitn].TFLG &  PIT_TFLG_TIF_MASK)		//判断时间是否超时
	{
		PIT_FLAG_CLR(pitn);									//清除中断标志位
		PIT->CHANNEL[pitn].TCTRL &= ~ PIT_TCTRL_TEN_MASK;   //禁止PITn定时器（用于清空计数值）
		return 0XFFFFFFFF;									//超时返回最大值
	}
	return val;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      pit初始化
//  @param      pitn            选择pit模块
//  @return     void
//  @since      v2.0
//  Sample usage:               pit_close(pit0);			//pit0停止计时
//-------------------------------------------------------------------------------------------------------------------
void pit_close(PITn pitn)
{
	PIT_FLAG_CLR(pitn);										//清除中断标志位
	PIT->CHANNEL[pitn].TCTRL &= ~ PIT_TCTRL_TEN_MASK;       //禁止PITn定时器（用于清空计数值）
}


















