/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       VCAN_LCD_ST7735R.h
 * @brief      LCD 函数库
 * @author     山外科技
 * @version    v5.0
 * @date       2013-09-07
 */

#ifndef _VCAN_LCD_ST7735R_H_
#define _VCAN_LCD_ST7735R_H_

#include "MK60_gpio.h"
#include "VCAN_LCD.h"

//用户配置
#define ST7735R_DIR_DEFAULT     0       //定义默认方向 (0~3)

#if (USE_LCD == LCD_ST7735R)

/*********************** API接口 ******************************/
//提供API接口给LCD调用

#define LCD_H                   ST7735R_get_h()                 //高
#define LCD_W                   ST7735R_get_w()                 //宽

#define LCD_INIT()              LCD_ST7735R_init()              //初始化
#define LCD_PTLON(site,size)    LCD_ST7735R_ptlon(site,size)    //开窗
#define LCD_RAMWR()             LCD_WR_CMD(0x2C)                //写模式
#define LCD_WR_DATA(data)       do{LCD_ST7735R_WR_8DATA((uint8)((data)>>8) );LCD_ST7735R_WR_8DATA( (uint8)(data) );}while(0)       //写数据
#define LCD_WR_CMD(cmd)         LCD_ST7735R_WR_8CMD(cmd)        //命令
#define LCD_SET_DIR(opt)        LCD_ST7735R_dir(opt)            //方向

#define LCD_DIR                 ST7735R_get_dir()               //获取方向


//通过函数调用，速度太慢了，改成宏定义调用
//#define LCD_WR_8DATA(data)      LCD_ST7735R_wr_data(data)       //写数据
//#define LCD_WR_8CMD(cmd)        LCD_ST7735R_wr_cmd(cmd)         //命令

#endif  //(USE_LCD == LCD_ST7735R)

/********************* ST7735R内部实现 ***********************/
//LCD 的管脚定义
#define     LCD_ST7735R_WR      PTC9
#define     LCD_ST7735R_RD      PTC10
#define     LCD_ST7735R_CS      PTC11
#define     LCD_ST7735R_RS      PTC12
#define     LCD_ST7735R_RST     PTC13

//此部分需要修改初始化里的代码
#define     LCD_ST7735R_P0              PTC_B0_OUT
#define     LCD_ST7735R_PIN             PTC_B0_IN
#define     LCD_ST7735R_PDDR_OUT()      (DDRC_B0 = 0xff)
#define     LCD_ST7735R_PDDR_IN()       (DDRC_B0 = 0)


#define ST7735R_DELAY()                         //do{asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");}while(0)
#define ST7735R_DELAY_MS(ms)    DELAY_MS(ms)


extern void     LCD_ST7735R_init();
extern void     LCD_ST7735R_dir(uint8 option);
extern void     LCD_ST7735R_ptlon(Site_t site, Size_t size);
extern uint16   ST7735R_get_h();
extern uint16   ST7735R_get_w();
extern uint8    ST7735R_get_dir();


extern void LCD_ST7735R_wr_data(uint8 data);
extern void LCD_ST7735R_wr_cmd(uint8 cmd);

#define     LCD_ST7735R_WR_OUT      PTXn_T(LCD_ST7735R_WR,OUT)
#define     LCD_ST7735R_RD_OUT      PTXn_T(LCD_ST7735R_RD,OUT)
#define     LCD_ST7735R_CS_OUT      PTXn_T(LCD_ST7735R_CS,OUT)
#define     LCD_ST7735R_RS_OUT      PTXn_T(LCD_ST7735R_RS,OUT)
#define     LCD_ST7735R_RST_OUT     PTXn_T(LCD_ST7735R_RST,OUT)

#define LCD_ST7735R_WR_8CMD(cmd)    do\
                            {\
                                LCD_ST7735R_RD_OUT=1;\
                                LCD_ST7735R_RS_OUT=0;\
                                LCD_ST7735R_CS_OUT=0;\
                                LCD_ST7735R_P0=(uint8)(cmd);\
                                LCD_ST7735R_WR_OUT=0;\
                                LCD_ST7735R_WR_OUT=1;\
                                LCD_ST7735R_CS_OUT=1;\
                            }while(0)   //LCD_WR=0;LCD_WR=1;产生一个上升沿

#define LCD_ST7735R_WR_8DATA(data)  do\
                            {\
                                LCD_ST7735R_RD_OUT=1;\
                                LCD_ST7735R_RS_OUT=1;\
                                LCD_ST7735R_CS_OUT=0;\
                                LCD_ST7735R_P0=(uint8)(data);\
                                LCD_ST7735R_WR_OUT=0;\
                                LCD_ST7735R_WR_OUT=1;\
                                LCD_ST7735R_CS_OUT=1;\
                            }while(0)   //LCD_WR=0;在这里写入数据到RAM


#define LCD_ST7735R_WR_DATA(data)       do{LCD_ST7735R_WR_8DATA((uint8)((data)>>8) );LCD_ST7735R_WR_8DATA( (uint8)(data) );}while(0)       //写数据



#endif  //_VCAN_LCD_ST7735R_H_
