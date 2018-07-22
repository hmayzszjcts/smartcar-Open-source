/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       VCAN_LCD_ILI9341.c
 * @brief      LCD ILI9341函数库
 * @author     山外科技
 * @version    v5.1
 * @date       2014-02-03
 */

#include "common.h"
#include "MK60_gpio.h"
#include "VCAN_LCD_ILI9341.h"



#if (USE_LCD == LCD_ILI9341)

//定义所用到的 IO管脚
#define LCD_RST    PTC13                //复位
#define LCD_BL     PTB8                 //背光


#define ILI9341_DELAY()          DELAY_MS(100)
#define ILI9341_DELAYMS(ms)      DELAY_MS(ms)


#define ILI9341_H       240
#define ILI9341_W       320

#if ((ILI9341_DIR_DEFAULT&1 )== 0)      //横屏
uint16  ili9341_h   = ILI9341_H;
uint16  ili9341_w   = ILI9341_W;
#else
uint16  ili9341_h   = ILI9341_W;
uint16  ili9341_w   = ILI9341_H;
#endif
uint8   ili9341_dir = ILI9341_DIR_DEFAULT;
/*!
 *  @brief      LCD_ILI9341初始化
 *  @since      v5.0
 */
void    LCD_ILI9341_init()
{
    gpio_init (LCD_BL, GPO, 1); //LCD背光管脚输出1，表示关闭LCD背光

    //复位LCD
    gpio_init (LCD_RST, GPO, 0);

    ILI9341_DELAYMS(1);
    GPIO_SET   (LCD_RST, 1);

    //初始化总线
    flexbus_8080_init();

    ILI9341_DELAY();
    LCD_ILI9341_WR_CMD(0xCF);
    LCD_ILI9341_WR_DATA(0x00);
    LCD_ILI9341_WR_DATA(0x81);
    LCD_ILI9341_WR_DATA(0x30);

    ILI9341_DELAY();
    LCD_ILI9341_WR_CMD(0xED);
    LCD_ILI9341_WR_DATA(0x64);
    LCD_ILI9341_WR_DATA(0x03);
    LCD_ILI9341_WR_DATA(0x12);
    LCD_ILI9341_WR_DATA(0x81);

    ILI9341_DELAY();
    LCD_ILI9341_WR_CMD(0xE8);
    LCD_ILI9341_WR_DATA(0x85);
    LCD_ILI9341_WR_DATA(0x10);
    LCD_ILI9341_WR_DATA(0x78);

    ILI9341_DELAY();
    LCD_ILI9341_WR_CMD(0xCB);
    LCD_ILI9341_WR_DATA(0x39);
    LCD_ILI9341_WR_DATA(0x2C);
    LCD_ILI9341_WR_DATA(0x00);
    LCD_ILI9341_WR_DATA(0x34);
    LCD_ILI9341_WR_DATA(0x02);

    ILI9341_DELAY();
    LCD_ILI9341_WR_CMD(0xF7);
    LCD_ILI9341_WR_DATA(0x20);

    ILI9341_DELAY();
    LCD_ILI9341_WR_CMD(0xEA);
    LCD_ILI9341_WR_DATA(0x00);
    LCD_ILI9341_WR_DATA(0x00);

    ILI9341_DELAY();
    LCD_ILI9341_WR_CMD(0xB1);
    LCD_ILI9341_WR_DATA(0x00);
    LCD_ILI9341_WR_DATA(0x1B);

    ILI9341_DELAY();
    LCD_ILI9341_WR_CMD(0xB6);
    LCD_ILI9341_WR_DATA(0x0A);
    LCD_ILI9341_WR_DATA(0xA2);

    ILI9341_DELAY();
    LCD_ILI9341_WR_CMD(0xC0);
    LCD_ILI9341_WR_DATA(0x35);

    ILI9341_DELAY();
    LCD_ILI9341_WR_CMD(0xC1);
    LCD_ILI9341_WR_DATA(0x11);

    LCD_ILI9341_WR_CMD(0xC5);
    LCD_ILI9341_WR_DATA(0x45);
    LCD_ILI9341_WR_DATA(0x45);

    LCD_ILI9341_WR_CMD(0xC7);
    LCD_ILI9341_WR_DATA(0xA2);

    LCD_ILI9341_WR_CMD(0xF2);
    LCD_ILI9341_WR_DATA(0x00);

    LCD_ILI9341_WR_CMD(0x26);
    LCD_ILI9341_WR_DATA(0x01);
    ILI9341_DELAY();
    LCD_ILI9341_WR_CMD(0xE0); //Set Gamma
    LCD_ILI9341_WR_DATA(0x0F);
    LCD_ILI9341_WR_DATA(0x26);
    LCD_ILI9341_WR_DATA(0x24);
    LCD_ILI9341_WR_DATA(0x0B);
    LCD_ILI9341_WR_DATA(0x0E);
    LCD_ILI9341_WR_DATA(0x09);
    LCD_ILI9341_WR_DATA(0x54);
    LCD_ILI9341_WR_DATA(0xA8);
    LCD_ILI9341_WR_DATA(0x46);
    LCD_ILI9341_WR_DATA(0x0C);
    LCD_ILI9341_WR_DATA(0x17);
    LCD_ILI9341_WR_DATA(0x09);
    LCD_ILI9341_WR_DATA(0x0F);
    LCD_ILI9341_WR_DATA(0x07);
    LCD_ILI9341_WR_DATA(0x00);
    LCD_ILI9341_WR_CMD(0XE1); //Set Gamma
    LCD_ILI9341_WR_DATA(0x00);
    LCD_ILI9341_WR_DATA(0x19);
    LCD_ILI9341_WR_DATA(0x1B);
    LCD_ILI9341_WR_DATA(0x04);
    LCD_ILI9341_WR_DATA(0x10);
    LCD_ILI9341_WR_DATA(0x07);
    LCD_ILI9341_WR_DATA(0x2A);
    LCD_ILI9341_WR_DATA(0x47);
    LCD_ILI9341_WR_DATA(0x39);
    LCD_ILI9341_WR_DATA(0x03);
    LCD_ILI9341_WR_DATA(0x06);
    LCD_ILI9341_WR_DATA(0x06);
    LCD_ILI9341_WR_DATA(0x30);
    LCD_ILI9341_WR_DATA(0x38);
    LCD_ILI9341_WR_DATA(0x0F);
    ILI9341_DELAY();


    ILI9341_DELAY();
    LCD_ILI9341_WR_CMD(0x3a); // Memory Access Control
    LCD_ILI9341_WR_DATA(0x55);
    LCD_ILI9341_WR_CMD(0x11); //Exit Sleep
    ILI9341_DELAY();
    LCD_ILI9341_WR_CMD(0x29); //display on


    LCD_SET_DIR(ili9341_dir);   //液晶方向显示函数

    LCD_ILI9341_WR_CMD(0x2c);

    PTXn_T(LCD_BL,OUT) = 0;     //开LCD背光
}

/*!
 *  @brief      设置ILI9341GRAM指针扫描方向
 *  @param      option    方向选择（0~3）
 *  @since      v5.0
 */
void LCD_ILI9341_dir(uint8 option)
{

    option = option % 4;


    ili9341_dir = option;


    switch(option)
    {
        case 0:
        {
            /*横屏*/
            LCD_ILI9341_WR_CMD(0x36);
            LCD_ILI9341_WR_DATA(0xA8);    //横屏

            LCD_ILI9341_WR_CMD(0X2A);
            LCD_ILI9341_WR_DATA(0x00);  //start
            LCD_ILI9341_WR_DATA(0x00);
            LCD_ILI9341_WR_DATA(0x01);  //end
            LCD_ILI9341_WR_DATA(0x3F);

            LCD_ILI9341_WR_CMD(0X2B);
            LCD_ILI9341_WR_DATA(0x00);   //start
            LCD_ILI9341_WR_DATA(0x00);
            LCD_ILI9341_WR_DATA(0x00);   //end
            LCD_ILI9341_WR_DATA(0xEF);

            ili9341_h   = ILI9341_H;
            ili9341_w   = ILI9341_W;
        }
        break;
        case 1:
        {
            /*竖屏*/
            LCD_ILI9341_WR_CMD(0x36);
            LCD_ILI9341_WR_DATA(0xD8);  //竖屏

            LCD_ILI9341_WR_CMD(0X2A);
            LCD_ILI9341_WR_DATA(0x00);
            LCD_ILI9341_WR_DATA(0x00);
            LCD_ILI9341_WR_DATA(0x00);
            LCD_ILI9341_WR_DATA(0xEF);

            LCD_ILI9341_WR_CMD(0X2B);
            LCD_ILI9341_WR_DATA(0x00);
            LCD_ILI9341_WR_DATA(0x00);
            LCD_ILI9341_WR_DATA(0x01);
            LCD_ILI9341_WR_DATA(0x3F);

            ili9341_h   = ILI9341_W;
            ili9341_w   = ILI9341_H;
        }
        break;
        case 2:
        {
            /*横屏*/
            LCD_ILI9341_WR_CMD(0x36);
            LCD_ILI9341_WR_DATA(0x68);    //横屏

            LCD_ILI9341_WR_CMD(0X2A);
            LCD_ILI9341_WR_DATA(0x00);  //start
            LCD_ILI9341_WR_DATA(0x00);
            LCD_ILI9341_WR_DATA(0x01);  //end
            LCD_ILI9341_WR_DATA(0x3F);

            LCD_ILI9341_WR_CMD(0X2B);
            LCD_ILI9341_WR_DATA(0x00);   //start
            LCD_ILI9341_WR_DATA(0x00);
            LCD_ILI9341_WR_DATA(0x00);   //end
            LCD_ILI9341_WR_DATA(0xEF);
            ili9341_h   = ILI9341_H;
            ili9341_w   = ILI9341_W;
        }
        break;
        case 3:
        {
            /*竖屏*/
            LCD_ILI9341_WR_CMD(0x36);
            LCD_ILI9341_WR_DATA(0x18);  //竖屏

            LCD_ILI9341_WR_CMD(0X2A);
            LCD_ILI9341_WR_DATA(0x00);
            LCD_ILI9341_WR_DATA(0x00);
            LCD_ILI9341_WR_DATA(0x00);
            LCD_ILI9341_WR_DATA(0xEF);

            LCD_ILI9341_WR_CMD(0X2B);
            LCD_ILI9341_WR_DATA(0x00);
            LCD_ILI9341_WR_DATA(0x00);
            LCD_ILI9341_WR_DATA(0x01);
            LCD_ILI9341_WR_DATA(0x3F);

            ili9341_h   = ILI9341_W;
            ili9341_w   = ILI9341_H;
        }
        break;
        default:
            //由于开头有校验，因而不会执行到这里
            //LCD_ILI9341_dir(ILI9341_DIR_DEFAULT);
            break;

    }

}

/*!
 *  @brief      设置ILI9341开窗
 *  @param      site        左上角坐标位置
 *  @param      size        开窗大小
 *  @since      v5.0
 */
void LCD_ILI9341_ptlon(Site_t site, Size_t size)
{

    LCD_ILI9341_WR_CMD(0X2A);
    LCD_ILI9341_WR_DATA(site.x >> 8); //start
    LCD_ILI9341_WR_DATA(site.x & 0xFF);
    LCD_ILI9341_WR_DATA((site.x + size.W - 1) >> 8); //end
    LCD_ILI9341_WR_DATA((site.x  + size.W - 1) & 0xFF);

    LCD_ILI9341_WR_CMD(0X2B);
    LCD_ILI9341_WR_DATA(site.y >> 8); //start
    LCD_ILI9341_WR_DATA(site.y & 0xFF);
    LCD_ILI9341_WR_DATA((site.y + size.H - 1) >> 8); //end
    LCD_ILI9341_WR_DATA((site.y + size.H - 1) & 0xFF);
}

/*!
 *  @brief      获取 ILI9341 高度
 *  @return     ILI9341 高度
 *  @since      v5.0
 */
uint16 ILI9341_get_h()
{
    return ili9341_h;
}

/*!
 *  @brief      获取 ILI9341 宽度
 *  @return     ILI9341 宽度
 *  @since      v5.0
 */
uint16 ILI9341_get_w()
{
    return ili9341_w;
}

/*!
 *  @brief      获取 ILI9341 显示方向
 *  @return     ILI9341 方向
 *  @since      v5.0
 */
uint8 ILI9341_get_dir()
{
    return ili9341_dir;
}


#endif //(USE_LCD == LCD_ILI9341)

