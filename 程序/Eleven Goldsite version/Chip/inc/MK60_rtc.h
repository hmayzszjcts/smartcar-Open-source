/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MK60_rtc.h
 * @brief      rtc模块函数
 * @author     山外科技
 * @version    v5.1
 * @date       2014-04-25
 */

#ifndef _MK60_RTC_H_
#define _MK60_RTC_H_


#define rtc_alarm_irq_en()      RTC_IER |= RTC_IER_TAIE_MASK      //使能闹钟中断
#define rtc_alarm_irq_dis()     RTC_IER &= ~RTC_IER_TAIE_MASK     //禁止闹钟中断


#define rtc_overflow_irq_en()   RTC_IER |= RTC_IER_TOIE_MASK      //使能溢出中断
#define rtc_overflow_irq_dis()  RTC_IER &= ~RTC_IER_TOIE_MASK     //禁止溢出中断

#define rtc_invalid_irq_en()    RTC_IER |= RTC_IER_TIIE_MASK      //使能无效设置中断
#define rtc_invalid_irq_dis()   RTC_IER &= ~RTC_IER_TIIE_MASK     //禁止无效设置中断



extern void     rtc_init(void);                         //初始化

extern void     rtc_set_time(uint32 seconds);           //设置时间
extern uint32   rtc_get_time(void);                     //获取时间

extern uint8    rtc_set_alarm(uint32 alarm);            //设置闹钟
extern void     rtc_close_alarm(void);                  //关闭闹钟

extern void     rtc_test_handler(void);                 //可供参考的 RTC中断服务例程


#endif