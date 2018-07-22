/*!
 * @file       system_MK60DZ10.h
 * @brief      系统启动相关函数
 */

#ifndef __SYSTEM_MK60_H__
#define __SYSTEM_MK60_H__

extern int core_clk_khz;
extern int core_clk_mhz;
extern int bus_clk_khz;


void start(void);           //kinetis 启动函数
void sysinit (void);        //系统初始化，设置系统频率，初始化printf端口
void trace_clk_init(void);  //trace 时钟输出 初始化
void fb_clk_init(void);     //fb 时钟输出 初始化

void common_startup(void);  //复制中断向量表 和 必要的数据到 RAM里

#endif  //__SYSTEM_MK60_H__
