/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		KEA128_irq
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.23
 * @Target core		S9KEA128AMLK
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-11-6
 ********************************************************************************************************************/



#include "KEA128_irq.h"


//-------------------------------------------------------------------------------------------------------------------
//  @brief      外部中断
//  @param      void            
//  @return     void
//  @since      v2.0
//  Sample usage:               irq_init();
//								set_irq_priority(IRQ_IRQn,1);					//设置优先级,根据自己的需求设置 可设置范围为 0 - 3
//								enable_irq(IRQ_IRQn);							//打开pit0的中断开关
//								EnableInterrupts;								//打开总的中断开关  
//  @note                       在KEA128_port_cfg文件切换引脚，irq同一时刻只能对一个引脚进行检测
//                              进入中断函数后务必使用CLEAR_IRQ_FLAG宏定义清除中断标志位
//-------------------------------------------------------------------------------------------------------------------

void irq_init(void)
{
    uint8 ptn;
    uint32 temp;

    if(A5 == IRQ_PIN)
    {
        temp = SIM->PINSEL;
        temp &= ~((uint32)SIM_PINSEL_IRQPS_MASK);
        SIM->PINSEL = temp | SIM_PINSEL_IRQPS(0);
    }
    else
    {
        ptn = PTn(IRQ_PIN);//获取引脚号
        
        temp = SIM->PINSEL;
        temp &= ~((uint32)SIM_PINSEL_IRQPS_MASK);
        SIM->PINSEL = temp | SIM_PINSEL_IRQPS(ptn+1);
    }
    
    SIM->SCGC |= SIM_SCGC_IRQ_MASK;     //开启IRQ时钟
    temp = (0
            //| IRQ_SC_IRQPDD_MASK      //1：禁用上拉电阻 0：使能上拉电阻  注释代表0
            //| IRQ_SC_IRQEDG_MASK        //1：上升沿或者高电平触发 0：下降沿或者低电平触发   注释代表0
            | IRQ_SC_IRQPE_MASK         //使能IRQ
            | IRQ_SC_IRQIE_MASK         //开启中断
            //| IRQ_SC_IRQMOD_MASK      //1：检测边沿与电平触发  0：只检测边沿触发  注释代表0
            );
    IRQ->SC = temp;
    
}





