/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       VCAN_LCD.h
 * @brief      LCD 函数库
 * @author     山外科技
 * @version    v5.1
 * @date       2013-09-02
 */



#include "VCAN_LCD_ILI9341.h"
#include "VCAN_LCD_ST7735S.h"
#include "VCAN_LCD_ST7735R.h"

#include "VCAN_ASCII.h"
#include "VCAN_LCD_CH.h"                        //汉字显示


/******常用颜色*****/
#define RED     0XF800    //红色
#define GREEN   0X07E0    //绿色
#define BLUE    0X001F    //蓝色
#define BRED    0XF81F
#define GRED    0XFFE0    //灰色
#define GBLUE   0X07FF    //
#define BLACK   0X0000    //黑色
#define WHITE   0XFFFF    //白色
#define YELLOW  0xFFE0    //黄色

#define FCOLOUR         BLUE    //定义字体颜色
#define BCOLOUR         RED     //定义背景颜色

//提取RGB的3元素，rgb565必须是16位
#define RGB565_R(rgb565)                ((uint8)(((uint16)(rgb565)>>11) &0x1F))
#define RGB565_G(rgb565)                ((uint8)(((uint16)(rgb565)>> 5) &0x3F))
#define RGB565_B(rgb565)                ((uint8)( (uint16)(rgb565)      &0x1F))


#define GRAY_2_RGB565(gray)             ((uint16)((((uint8)(gray)>>3)<<11)|(((uint8)(gray)>>2)<<5)|((uint8)(gray)>>3)))

#define RGB565_2_GRAY(rgb565)           ((uint8)(((RGB565_R(rgb565)*235+RGB565_G(rgb565)*613+RGB565_B(rgb565)*625)+1)>>8))  //  31*235+63*613+31*625+1  = 255*256


#define RGB565_H(rgb565)                ((uint8)(((uint16)(rgb565))>>8))
#define RGB565_L(rgb565)                (uint8)(rgb565))

#define RGB24_RGB565(R,G,B)             ((uint16)((((uint8)(R)>>3)<<11)|(((uint8)(G)>>2)<<5)|((uint8)(B)>>3)))
#define RGB555_RGB565(rgb555)           (((((uint16)(rgb555)) & (uint16)(~0x1F)) << 1) |  (((uint16)(rgb555)) & 0x1F))
#define BGR555_RGB565(bgr555)           ((((uint16)(bgr555)<<11) & (uint16)(0x1F<<11)) | (((uint16)(bgr555)) & (uint16)(0x1F<<5)) | (((uint16)(bgr555)>>10) & (uint16)(0x1F)))



//需要底层实现的几个宏定义：
//LCD_H                         //高
//LCD_W                         //宽

//LCD_INIT()                    //初始化
//LCD_PTLON(site,size)          //开窗
//LCD_RAMWR()                   //写模式
//LCD_WR_DATA(data)             //写数据
//LCD_WR_DATA_B(data)           //写数据(大端)，16位液晶与LCD_WR_DATA一致即可，8位液晶交换顺序

//如下为可选
//LCD_RAMRD()                   //读模式
//LCD_SET_DIR(opt)              //设置显示方向(0~3)
//LCD_DIR                       //获取显示方向(0~3)


//如下为内部调用，或者上面的宏定义调用
//LCD_WR_CMD(cmd)               //命令
//LCD_RD_DATA()                 //读数据

//8位液晶可能需要（上面默认是16位）
//LCD_WR_8CMD(cmd)               //命令
//LCD_WR_8DATA(data)            //写数据


//根据字库定义 英文字母的 宽高。
#define LCD_EN_W        8
#define LCD_EN_H        16


/***************  LCD初始化  ***************/

void LCD_init(void);                                                                                    //LCD初始化

/***************  LCD绘画  ***************/

extern void LCD_point          (Site_t,                                        uint16 rgb565);                 //画点
extern void LCD_points         (Site_t *site,  uint32 point_num,               uint16 rgb565);                 //画一堆点
extern void LCD_rectangle      (Site_t, Size_t,                                uint16 rgb565);                 //画矩形
extern void LCD_char           (Site_t,        uint8 ascii,                    uint16 Color, uint16 bkColor);  //显示8*16字符
extern void LCD_str            (Site_t,        uint8 *Str,                     uint16 Color, uint16 bkColor);  //显示8*16字符串

extern void LCD_cross          (Site_t,uint16 len,                              uint16 rgb565);                 //画十字形

extern void LCD_clear          (uint16 rgb565);     //清屏


extern void LCD_num            (Site_t,        uint32 num,                     uint16 Color, uint16 bkColor);  //显示数字
#define MAX_NUM_BIT 5                                                                                   //数字的最大位数（用于清掉多余的屏幕残留数字）
#define LCD_num_C(site,num,color,bkColor)           LCD_num_BC(site, num,MAX_NUM_BIT,color,bkColor)     //显示数字（清掉多余的屏幕残留数字）
extern void LCD_num_BC         (Site_t,        uint32 num, uint8 max_num_bit,  uint16 Color, uint16 bkColor);  //显示数字（清掉多余的屏幕残留数字）

extern void LCD_Img_gray       (Site_t site, Size_t size, uint8 *img);                   //显示灰度图像
extern void LCD_Img_gray_Z     (Site_t site, Size_t size, uint8 *img, Size_t imgsize);   //显示灰度图像(可缩放)

#define BINARY_BGCOLOR  WHITE       //定义二值化图像背景颜色
#define BINARY_COLOR    BLACK       //定义二值化图像前景颜色
extern void LCD_Img_Binary     (Site_t site, Size_t size, uint8 *img);                   //显示二值化图像
extern void LCD_Img_Binary_Z   (Site_t site, Size_t size, uint8 *img, Size_t imgsize);   //显示二值化图像(可缩放)

extern void LCD_wave(Site_t site,Size_t size,uint8 *img,uint8 maxval,uint16 Color ,uint16 bkColor); //波形显示，带背景颜色（maxval 最大值表示超过此值的数都当作最大值处理）
extern void LCD_wave_display(Site_t site,Size_t size,uint8 *img,uint8 maxval,uint16 Color);        //波形显示，不带背景颜色


