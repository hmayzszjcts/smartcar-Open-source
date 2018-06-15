/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		NRF24L01
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.23
 * @Target core		S9KEA128AMLK
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-11-6
 * @note	
					接线定义：
					------------------------------------ 
					    OLED液晶    单片机                        
    					D0          G2
    					D1          G0            
    					RES         G3    
    					DC          G1
					------------------------------------ 
 ********************************************************************************************************************/



#ifndef _SEEKFREE_OELD_H
#define _SEEKFREE_OELD_H

#include "headfile.h"

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

//----宏定义OLED引脚----	 
#define  OLED_CLK_PIN	G2
#define  OLED_DIN_PIN	G0
#define  OLED_RST_PIN G3
#define  OLED_RS_PIN	G1
#define  OLED_CS_PIN	E4



			    
void OLED_Init(void);//初始化SSD1306		
void OLED_Refresh_Gram(void);//更新显存到LCD		
void OLED_Clear(void);//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	     
void OLED_Display_Off(void); //关闭OLED显示    
void OLED_Display_On(void);//开启OLED显示
void OLED_WR_Byte(uint8 dat,uint8 cmd);//向SSD1306写入一个字节。
uint32 mypow(uint8 m,uint8 n);//m^n函数

void OLED_DrawPoint(uint8 x,uint8 y,uint8 t);//画点 
void OLED_Fill(uint8 x1,uint8 y1,uint8 x2,uint8 y2,uint8 dot);//x1,y1,x2,y2 填充区域的对角坐标  
void OLED_ShowChar(uint8 x,uint8 y,uint8 chr,uint8 size,uint8 mode);//在指定位置显示一个字符,包括部分字符
void OLED_ShowNum(uint8 x,uint8 y,uint32 num,uint8 len,uint8 size, uint8 mode);
void OLED_ShowString(uint8 x,uint8 y,const uint8 *p);
/********************************************************************/

#endif
