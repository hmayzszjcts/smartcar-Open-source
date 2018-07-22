 /*!
  *     COPYRIGHT NOTICE
  *     Copyright (c) 2013,山外科技
  *     All rights reserved.
  *     技术讨论：山外论坛 http://www.vcan123.com
  *
  *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
  *     修改内容时必须保留山外科技的版权声明。
  *
  * @file       VCAN_RTC_count.c
  * @brief      rtc数据计算函数实现(以1970-01-01 为基准，到2099-12-31)
  * @author     山外科技
  * @version    v5.1
  * @date       2014-01-12
  */


/*
 * 包含头文件
 */
#include "common.h"
#include "VCAN_RTC_count.h"



const uint8     mon_t[12]   ={ 31,28,31,30,31,30,31,31,30,31,30,31};                //平年的月份
const uint8     mon2_t[12]  ={ 31,29,31,30,31,30,31,31,30,31,30,31};

const uint16    momoff_t[]  ={ 0,31,59,90,120,151,181,212,243,273,304,334,365 };    //平年的每月1日的累加天数
const uint16    momoff2_t[] ={ 0,31,60,91,121,152,182,213,244,274,305,335,366 };    //闰年的每月1日的累加天数


/*!
 *  @brief      检测是否为闰年
 *  @param      year    年份
 *  @return     是否为闰年(0表示平年，1表示闰年)
 *  @since      v5.0
 *  Sample usage:       if( Is_LeapYear(2013) )         //检测 2013 是否为闰年
                        {
                            printf("\n闰年!");          //
                        }else
                        {
                            printf("\n不是闰年！");
                        }
 */
uint8 Is_LeapYear(uint32 year)
{
    if(year%4 == 0)
    {
        if((year%100 != 0) || (year%400 == 0))
        {
            //闰年
            return 1;
        }
    }

    //不是闰年
    return 0;
}

/*!
 *  @brief      求年份的天数
 *  @param      year    年份
 *  @return     指定年份的天数
 *  @since      v5.0
 *  Sample usage:       uint32 days = year2day(2013);
                        printf("2013年共%d天!",days);
 */
uint32 year2day(uint32 year)
{
    return (365 + Is_LeapYear(year));
}

/*!
 *  @brief      从1970-01-01 0:0:0 至 指定时间的年月日时分秒格式转为总秒数
 *  @param      time    时间结构体
 *  @return     从1970-01-01 0:0:0 至 指定时间的总秒数
 *  @since      v5.0
 *  Sample usage:       time_s time = {2013,9,1,0,0,0};  //时间为 2013-09-01 0:0:0
                        printf("共%d秒!",time2sec(time));
 */
uint32 time2sec(time_s time)
{
    //参考 linux内核代码 include/linux/time.h 里的函数 mktime()
    //根据高斯算法将以year/mon/day/hour/min/sec（如1980－12－31 23：59：59）
    //格式表示的时间转换为相对于1970－01－01 00：00：00
    //这个UNIX时间基准以来的相对秒数

    //注意一点，闰年的二月份是29天，非闰年则为28天
    //四年一闰，百年不闰，四百年再闰

    //检测时间是否有效
    ASSERT(time.year >= 1970);     //断言，以 1970 为基准，年份最小为 1970
    ASSERT(time.year < 2100);      //断言，32位的秒计数器可保存136年的时间，我们此处限定最大年份为2099，实际上是可计时到 2105年12月31日
    ASSERT(time.mon  <= 12);       //断言，月份最多为12月
    ASSERT(     ((Is_LeapYear(time.year) == 0) &&                    (time.day <= mon_t[time.mon-1])) //平年
           ||   ((Is_LeapYear(time.year) == 1) && (time.mon != 2) && (time.day <= mon_t[time.mon-1]))               //闰年的非2月
           ||   ((Is_LeapYear(time.year) == 1) && (time.mon == 2) && (time.day <= 29))
           );


               //ASSERT((time.mon != 2) || ((time.mon == 2) && (time.day  <= 29)));   //二月份天数不超过 29天。
    //ASSERT((time.mon == 2) && (time.year%4 ！= 0)); //年份非4的倍数，则是普通年份


    if (0 >= (int8) (time.mon -= 2)) /* 1..12 -> 11,12,1..10 */
    {
        time.mon += 12; /* Puts Feb last since it has leap day */
        time.year -= 1;
    }

    return (((
            (uint32) (time.year/4 - time.year/100 + time.year/400 + 367*(time.mon)/12 + time.day) + time.year*365 - 719499 /* 现有的总日数 */
            )*24 + time.hour /* 现有的总小时数 */
            )*60 + time.min  /* 现有的总分钟数 */
            )*60 + time.sec; /* 最终的总秒数   */
}

/*!
 *  @brief      以1970-01-01 为基准的总秒数 转为年月日时分秒格式
 *  @param      sec     总秒数
 *  @param      time    时间结构体
 *  @since      v5.0
 *  Sample usage:       time_s time;
                        sec2time(1000,&time);   //求1000秒对应的时间
                        printf("\n时间是：%d-%02d-%02d %02d:%02d:%02d",
                                                time.year,time.mon,time.day,
                                                time.hour,time.min,time.sec);
 */
void sec2time(uint32 sec, time_s * time)
{
#define DAYSEC  (60*60*24)      //定义每天的总秒数
    //uint32 i;
    uint32  hms                //一天剩下的秒数
            ,day                //秒数共有的天数
            ,tmp;

    uint16 const* dayoff ;
    uint8  const* monoff;

    if(sec > 0xf48656ff)    //时间超过 2099-12-31 23:59:59后，会从 1970-1-1 0:0:0 开始计时
    {
        sec -= 0xf48656ff;
    }

    day = sec / DAYSEC;
    hms = sec % DAYSEC;

    //先求时分秒
    time->hour = (uint8) (hms / (60*60));         //时
    hms = hms % (60*60);
    time->min = (uint8) (hms / 60);  //分
    time->sec = (uint8) (hms % 60);   //秒

    /*算出当前年份，起始的计数年份为1970年*/
    time->year  =  (uint16)(day/366 );          //粗略求出 年份(还没加 1970)

    day = day
            -  365*(time->year)    //天数
            - ((time->year +1)/4)  //4年一闰年
            //+ (time->year/(2101-1970) )      //2100不是闰年，由于最大年份是 2099，不考虑2100，因而此处注释
            ;

    time->year += 1970;
    tmp = 365 + Is_LeapYear(time->year);        //指定年份的 天数
    if(day >= tmp)
    {
        time->year++;
        day -=  tmp;
    }

    time->mon = (uint8)(day/31 +1);                 //粗略算月份，可能需要加1

    tmp = Is_LeapYear(time->year);
    dayoff  = tmp ? momoff2_t : momoff_t;
    monoff  = tmp ? mon2_t : mon_t;

    time->day = (uint8)(day +1 - dayoff[time->mon -1]);

    if(time->day > monoff[time->mon-1 ] )
    {
        time->mon++;
        time->day = (uint8)(day +1 - dayoff[time->mon -1]);
    }
}
