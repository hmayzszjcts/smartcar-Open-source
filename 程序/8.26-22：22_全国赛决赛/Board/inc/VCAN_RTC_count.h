/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       VCAN_RTC_count.h
 * @brief      rtc数据计算函数实现(以1970-01-01 为基准，到2099-12-31)
 * @author     山外科技
 * @version    v5.0
 * @date       2013-08-27
 */


#ifndef _VCAN_RTC_COUNT_H_
#define _VCAN_RTC_COUNT_H_

typedef struct
{
    uint16  year;   //年
    uint8   mon;    //月
    uint8   day;    //日

    uint8   hour;   //时
    uint8   min;    //分
    uint8   sec;    //秒

    uint8   invalid;//时间无效检查（0表示时间有效，1表示时间无效）
} time_s;


extern uint8   Is_LeapYear(uint32 year);               //判断是否为闰年（是则返回1，否则返回0）
extern uint32  year2day(uint32 year);                  //求指定年份的天数
extern uint32  time2sec(time_s time);                  //年月日时分秒格式时间转为以 1970 -01-01为起点的秒数
extern void    sec2time(uint32 sec, time_s *time);     //以 1970-01-01为起点的秒数 转为 年月日时分秒格式时间

#endif
