/*!
 * @file       MK60_wdog.h
 * @brief      看门狗驱动函数
 */

#ifndef __MK60_WDOG_H__
#define __MK60_WDOG_H__

/********************************************************************/

//wdog 代码，采用 LDO 时钟，不分频。

extern void wdog_init_ms(uint32 ms);   //初始化看门狗，设置喂狗时间 ms
extern void wdog_feed(void);           //喂狗


extern void wdog_disable(void);        //禁用看门狗
extern void wdog_enable(void);         //启用看门狗

/********************************************************************/
#endif /* __MK60_WDOG_H__ */
