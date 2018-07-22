/*!
 * @file       MK60_SysTick.h
 * @brief      SysTick 驱动函数，包括延时
 */

//SysTick 计数时间太短，不宜用于 计时，因而不加入 计时功能

#define     SYSTICK_CLK_KHZ                 core_clk_khz


/*          用于延时         */
extern void    systick_delay(uint32 time);
extern void    systick_delay_ms(uint32 ms);

#define     systick_delay_us(us)          systick_delay(us * SYSTICK_CLK_KHZ/1000);   //延时 us
#define     systick_delay_ns(ns)          systick_delay(ns * SYSTICK_CLK_KHZ/1000000);//延时 ns

/*       用于定时           */
extern void systick_timing(uint32 time);
#define systick_timing_ms(ms)               systick_timing(ms * SYSTICK_CLK_KHZ);        //延时 ms
#define systick_timing_us(us)               systick_timing(us * SYSTICK_CLK_KHZ/1000);   //延时 us
#define systick_timing_ns(ns)               systick_timing(ns * SYSTICK_CLK_KHZ/1000000);//延时 ns


