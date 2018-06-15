/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		TFT
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.23
 * @Target core		S9KEA128AMLK
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-11-6
 * @note		
					1.8 TFT液晶接线定义：
					------------------------------------ 
						模块管脚            单片机管脚
						SCL                 B2
						SDA                 B3
						RES                 B1
						DC                  B0
						CS                  B5
						
						电源引脚
						BL  3.3V电源（背光控制引脚，也可以接PWM来控制亮度）
						VCC 3.3V电源
						GND 电源地
	
					------------------------------------ 

 ********************************************************************************************************************/



#ifndef _SEEKFREE_18TFT_H
#define _SEEKFREE_18TFT_H

#include "headfile.h"


#define	TFT_X_MAX	128	//液晶X方宽度
#define TFT_Y_MAX	160 //液晶Y方宽度


#define DC_PIN		B0	//液晶命令位硬件定义
#define REST_PIN	B1 //液晶复位引脚定义
#define DC(x)   	gpio_set(DC_PIN,x);
#define REST(x) 	gpio_set(REST_PIN,x);

//-------常用颜色----------
#define RED     	0XF800    //红色
#define GREEN   	0X07E0    //绿色
#define BLUE    	0X001F    //蓝色
#define BRED    	0XF81F
#define GRED    	0XFFE0    //灰色
#define GBLUE   	0X07FF    //
#define BLACK   	0X0000    //黑色
#define WHITE   	0XFFFF    //白色
#define YELLOW  	0xFFE0    //黄色



//定义写字笔的颜色
#define PENCOLOR RED

//定义背景颜色
#define BGCOLOR	 WHITE


extern  const unsigned char gImage_qq[3200];



void lcd_init(void);
void dsp_single_colour(int color);
void lcd_showchar(uint16 x,uint16 y,uint8 dat);
void lcd_showstr(uint16 x,uint16 y,uint8 dat[]);
void lcd_showint8(uint16 x,uint16 y,int8 dat);
void lcd_showint16(uint16 x,uint16 y,int16 dat);
void showimage(const unsigned char *p);



#endif
