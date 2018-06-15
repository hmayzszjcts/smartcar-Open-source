/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       VCAN_TSL1401.h
 * @brief      线阵CCD函数库
 * @author     山外科技
 * @version    v5.0
 * @date       2014-01-04
 */

#ifndef _TSL1401_H_
#define _TSL1401_H_ 1



#define TSL1401_SIZE    128                         //定义CCD每次采集的次数

#define TSL1401_INT_TIME(time)  pit_init_ms(PIT0,time)     //定义CCD 定时中断的中断时间设置函数


typedef enum
{
    TSL1401_L,      //左
    TSL1401_M,      //中
    TSL1401_R,      //右

    TSL1401_MAX,
} TSL1401_e;


typedef enum
{
    tsl1401_noint,      //没初始化
    tsl1401_start,      //开始采集
    //tsl1401_ing,        //采集中
    tsl1401_finish,     //采集结束
} tsl1401_status_e;


extern void tsl1401_init        (uint32 time);                              //tsl1401,线性CCD初始化，time为曝光时间，单位与 TSL1401_INT_TIME 的设置一致，初始化后必须调用tsl1401_set_addrs 来设置存储地址（或者先设置存储图像地址）
extern void tsl1401_set_addrs   (TSL1401_e num, uint8 *addr, ... );         //tsl1401,线性CCD设置采集数据的地址
extern void tsl1401_get_img     (void);                                     //tsl1401,线性CCD采集图像

extern void tsl1401_set_time    (uint32 time);                              //设置 tsl1401,线性CCD 曝光时间
extern uint32 tsl1401_get_time     (void);                                  //获取 tsl1401,线性CCD 曝光时间

extern void tsl1401_led_en      (TSL1401_e num);                            //开启补光灯
extern void tsl1401_led_dis     (TSL1401_e num);                            //关闭补光灯


extern void tsl1401_time_isr    ();                                         //定时曝光中断服务函数

#endif  // _TSL1401_H_