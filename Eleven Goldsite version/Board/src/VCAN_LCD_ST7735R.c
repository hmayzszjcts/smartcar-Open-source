/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       VCAN_LCD_ST7735R.c
 * @brief      LCD 函数库
 * @author     山外科技
 * @version    v5.1
 * @date       2014-04-26
 */


#include "common.h"
#include "MK60_gpio.h"
#include "VCAN_LCD_ST7735R.h"





//定义所用到的 IO管脚
#define LCD_RST    PTC13                //复位

#define ST7735R_H       128
#define ST7735R_W       128

uint16  st7735r_h   = ST7735R_H;
uint16  st7735r_w   = ST7735R_W;
uint8   st7735r_dir = ST7735R_DIR_DEFAULT;

void LCD_ST7735R_wr_data(uint8 data)
{
    LCD_ST7735R_RD_OUT = 1;
    LCD_ST7735R_RS_OUT = 1;
    LCD_ST7735R_CS_OUT = 0;

    LCD_ST7735R_P0 = data;

    ST7735R_DELAY();

    LCD_ST7735R_WR_OUT = 0;
    LCD_ST7735R_WR_OUT = 1;
    LCD_ST7735R_CS_OUT = 1;
}

void LCD_ST7735R_wr_cmd(uint8 cmd)
{
    LCD_ST7735R_RD_OUT = 1;
    LCD_ST7735R_RS_OUT = 0;
    LCD_ST7735R_CS_OUT = 0;

    LCD_ST7735R_P0 = cmd;

    ST7735R_DELAY();
    LCD_ST7735R_WR_OUT = 0;
    LCD_ST7735R_WR_OUT = 1;
    LCD_ST7735R_CS_OUT = 1;
}

/*!
 *  @brief      LCD_ST7735R初始化
 *  @since      v5.0
 */
void    LCD_ST7735R_init()
{
    uint8 n ;

    for(n = 0; n < 8; n++)
    {
        gpio_init  ((PTXn_e)(PTC0 + n), GPO, 0);
    }


    gpio_init  (LCD_ST7735R_WR, GPO, 0);
    gpio_init  (LCD_ST7735R_RD, GPO, 1);
    gpio_init  (LCD_ST7735R_CS, GPO, 1);
    gpio_init  (LCD_ST7735R_RS, GPO, 0);
    gpio_init  (LCD_ST7735R_RST, GPO, 0);

    port_init  (LCD_ST7735R_WR , ALT1 | HDS);
    port_init  (LCD_ST7735R_RD , ALT1 | HDS);
    port_init  (LCD_ST7735R_CS , ALT1 | HDS);
    port_init  (LCD_ST7735R_RS , ALT1 | HDS);

    //初始化总线
    LCD_ST7735R_RST_OUT = 0;
    ST7735R_DELAY();
    LCD_ST7735R_RST_OUT = 1;
    ST7735R_DELAY_MS(500);      //上电给足够时间

    LCD_ST7735R_WR_8CMD(0x11);           //Sleep out   退出睡眠模式
    ST7735R_DELAY_MS(120);      //Delay 120ms
    //------------------------------------ST7735R Frame Rate-----------------------------------------//
    /* Set the frame frequency of the full colors normal mode. */
    LCD_ST7735R_WR_8CMD(0xB1);     //In Normal Mode (Full Colors)    全屏模式
    LCD_ST7735R_WR_8DATA(0x05);    //设置 RTNA Set 1-line  Period  一行周期
    LCD_ST7735R_WR_8DATA(0x3A);    //设置 FPA: Front Porch
    LCD_ST7735R_WR_8DATA(0x3A);    //设置 BPA: Back Porch
    //Frame rate=fosc/((RTNA x 2 + 40) x (LINE + FPA + BPA +2))
    //其中 fosc = 850kHz

    /* Set the frame frequency of the Idle mode.  */
    LCD_ST7735R_WR_8CMD(0xB2);     //In Idle Mode (8-colors)
    LCD_ST7735R_WR_8DATA(0x05);
    LCD_ST7735R_WR_8DATA(0x3A);
    LCD_ST7735R_WR_8DATA(0x3A);

    /* Set the frame frequency of the Partial mode/ full colors. */
    LCD_ST7735R_WR_8CMD(0xB3);
    LCD_ST7735R_WR_8DATA(0x05);
    LCD_ST7735R_WR_8DATA(0x3A);
    LCD_ST7735R_WR_8DATA(0x3A);
    LCD_ST7735R_WR_8DATA(0x05);
    LCD_ST7735R_WR_8DATA(0x3A);
    LCD_ST7735R_WR_8DATA(0x3A);

    //------------------------------------End ST7735R Frame Rate-----------------------------------------//
    LCD_ST7735R_WR_8CMD(0xB4);   //Display Inversion Control  显示反转控制

    LCD_ST7735R_WR_8DATA(0x07);  //LCD_ST7735R_WR_8DATA(0x03); 原来为3，改为7
    // 低三位从高到低，分别为  full colors normal mode 、Idle mode 、
    // full Colors partial mode  的 点反转 或 列反转 ，1为列反转

    LCD_ST7735R_WR_8CMD(0xC0);   //Power Control Setting  电源控制设置
    LCD_ST7735R_WR_8DATA(0x28);
    LCD_ST7735R_WR_8DATA(0x08);
    LCD_ST7735R_WR_8DATA(0x84);
    LCD_ST7735R_WR_8CMD(0xC1);
    LCD_ST7735R_WR_8DATA(0XC0);
    LCD_ST7735R_WR_8CMD(0xC2);
    LCD_ST7735R_WR_8DATA(0x0C);
    LCD_ST7735R_WR_8DATA(0x00);
    LCD_ST7735R_WR_8CMD(0xC3);
    LCD_ST7735R_WR_8DATA(0x8C);
    LCD_ST7735R_WR_8DATA(0x2A);
    LCD_ST7735R_WR_8CMD(0xC4);
    LCD_ST7735R_WR_8DATA(0x8A);
    LCD_ST7735R_WR_8DATA(0xEE);
    //---------------------------------End ST7735R Power Sequence-------------------------------------//
    LCD_ST7735R_WR_8CMD(0xC5);   //  VCOM 电压配置
    LCD_ST7735R_WR_8DATA(0x0C); //  -0.725
    //------------------------------------ST7735R Gamma Sequence-----------------------------------------//
    LCD_ST7735R_WR_8CMD(0xE0);
    LCD_ST7735R_WR_8DATA(0x05);
    LCD_ST7735R_WR_8DATA(0x1A);
    LCD_ST7735R_WR_8DATA(0x0C);
    LCD_ST7735R_WR_8DATA(0x0E);
    LCD_ST7735R_WR_8DATA(0x3A);
    LCD_ST7735R_WR_8DATA(0x34);
    LCD_ST7735R_WR_8DATA(0x2D);
    LCD_ST7735R_WR_8DATA(0x2F);
    LCD_ST7735R_WR_8DATA(0x2D);
    LCD_ST7735R_WR_8DATA(0x2A);
    LCD_ST7735R_WR_8DATA(0x2F);
    LCD_ST7735R_WR_8DATA(0x3C);
    LCD_ST7735R_WR_8DATA(0x00);
    LCD_ST7735R_WR_8DATA(0x01);
    LCD_ST7735R_WR_8DATA(0x02);
    LCD_ST7735R_WR_8DATA(0x10);
    LCD_ST7735R_WR_8CMD(0xE1);
    LCD_ST7735R_WR_8DATA(0x04);
    LCD_ST7735R_WR_8DATA(0x1B);
    LCD_ST7735R_WR_8DATA(0x0D);
    LCD_ST7735R_WR_8DATA(0x0E);
    LCD_ST7735R_WR_8DATA(0x2D);
    LCD_ST7735R_WR_8DATA(0x29);
    LCD_ST7735R_WR_8DATA(0x24);
    LCD_ST7735R_WR_8DATA(0x29);
    LCD_ST7735R_WR_8DATA(0x28);
    LCD_ST7735R_WR_8DATA(0x26);
    LCD_ST7735R_WR_8DATA(0x31);
    LCD_ST7735R_WR_8DATA(0x3B);
    LCD_ST7735R_WR_8DATA(0x00);
    LCD_ST7735R_WR_8DATA(0x00);
    LCD_ST7735R_WR_8DATA(0x03);
    LCD_ST7735R_WR_8DATA(0x12);
    //------------------------------------End ST7735R Gamma Sequence-----------------------------------------//

    LCD_ST7735R_WR_8CMD(0x3A); //65k mode
    LCD_ST7735R_WR_8DATA(0x05);
    LCD_ST7735R_WR_8CMD(0x29); //开显示 Display on
    LCD_ST7735R_WR_8CMD(0x2c); //

    LCD_ST7735R_dir(st7735r_dir);

}

/*!
 *  @brief      设置ST7735R GRAM指针扫描方向
 *  @param      option    方向选择（正看（焊接排线在下面） 为 0 ，90度为1，180度为2，270度为2）
 *  @since      v5.0
 */
void LCD_ST7735R_dir(uint8 option)
{
    ASSERT(option < 4);

    st7735r_dir = option;
    switch(option)
    {
    case 0:
        LCD_ST7735R_WR_8CMD(0x36); // 内存数据访问控制  MX 列地址顺序, MY 行地址顺序 ，MV 行列交换 ，ML 垂直刷新顺序 ,RGB  RGB-BGA顺序
        LCD_ST7735R_WR_8DATA(0xc8);//0xc8  0xA8 0x08 0x68

        LCD_ST7735R_WR_8CMD(0x2a);
        LCD_ST7735R_WR_8DATA(0x00);
        LCD_ST7735R_WR_8DATA(0x00);
        LCD_ST7735R_WR_8DATA(0x00);
        LCD_ST7735R_WR_8DATA(128);

        LCD_ST7735R_WR_8CMD(0x2B);
        LCD_ST7735R_WR_8DATA(0x00);
        LCD_ST7735R_WR_8DATA(50);
        LCD_ST7735R_WR_8DATA(0x00);
        LCD_ST7735R_WR_8DATA(128+50);

        st7735r_h   = ST7735R_H;
        st7735r_w   = ST7735R_W;
        break;
    case 1:
        LCD_ST7735R_WR_8CMD(0x36); // 内存数据访问控制  MX 列地址顺序, MY 行地址顺序 ，MV 行列交换 ，ML 垂直刷新顺序 ,RGB  RGB-BGA顺序
        LCD_ST7735R_WR_8DATA(0xA8);//0xc8  0xA8 0x08 0x68

        LCD_ST7735R_WR_8CMD(0x2a);
        LCD_ST7735R_WR_8DATA(0x00);
        LCD_ST7735R_WR_8DATA(0x03);
        LCD_ST7735R_WR_8DATA(0x00);
        LCD_ST7735R_WR_8DATA(0x82);

        LCD_ST7735R_WR_8CMD(0x2B);
        LCD_ST7735R_WR_8DATA(0x00);
        LCD_ST7735R_WR_8DATA(0x02);
        LCD_ST7735R_WR_8DATA(0x00);
        LCD_ST7735R_WR_8DATA(0x81);

        st7735r_h   = ST7735R_W;
        st7735r_w   = ST7735R_H;
        break;
    case 2:
        LCD_ST7735R_WR_8CMD(0x36); // 内存数据访问控制  MX 列地址顺序, MY 行地址顺序 ，MV 行列交换 ，ML 垂直刷新顺序 ,RGB  RGB-BGA顺序
        LCD_ST7735R_WR_8DATA(0x08);//0xc8  0xA8 0x08 0x68

        LCD_ST7735R_WR_8CMD(0x2a);
        LCD_ST7735R_WR_8DATA(0x00);
        LCD_ST7735R_WR_8DATA(0x02);
        LCD_ST7735R_WR_8DATA(0x00);
        LCD_ST7735R_WR_8DATA(0x81);

        LCD_ST7735R_WR_8CMD(0x2B);
        LCD_ST7735R_WR_8DATA(0x00);
        LCD_ST7735R_WR_8DATA(0x03);
        LCD_ST7735R_WR_8DATA(0x00);
        LCD_ST7735R_WR_8DATA(0x82);

        st7735r_h   = ST7735R_H;
        st7735r_w   = ST7735R_W;
        break;
    case 3:
        LCD_ST7735R_WR_8CMD(0x36); // 内存数据访问控制  MX 列地址顺序, MY 行地址顺序 ，MV 行列交换 ，ML 垂直刷新顺序 ,RGB  RGB-BGA顺序
        LCD_ST7735R_WR_8DATA(0x68);//0xc8  0xA8 0x08 0x68

        LCD_ST7735R_WR_8CMD(0x2a);
        LCD_ST7735R_WR_8DATA(0x00);
        LCD_ST7735R_WR_8DATA(0x03);
        LCD_ST7735R_WR_8DATA(0x00);
        LCD_ST7735R_WR_8DATA(0x82);

        LCD_ST7735R_WR_8CMD(0x2B);
        LCD_ST7735R_WR_8DATA(0x00);
        LCD_ST7735R_WR_8DATA(0x00);
        LCD_ST7735R_WR_8DATA(0x00);
        LCD_ST7735R_WR_8DATA(128);

        st7735r_h   = ST7735R_W;
        st7735r_w   = ST7735R_H;
        break;
    default:
        break;
    }

}

/*!
 *  @brief      设置 ST7735R 开窗
 *  @param      site        左上角坐标位置
 *  @param      size        开窗大小
 *  @since      v5.0
 */
void LCD_ST7735R_ptlon(Site_t site, Size_t size)
{

    if(st7735r_dir&0x01)
    {
        site.x += 32;     //液晶需要偏移一下，避免四周看不到的行
        //site.y += 2;
    }
    else
    {
        //site.x += 2;     //液晶需要偏移一下，避免四周看不到的行
        site.y += 32;
    }

    LCD_ST7735R_WR_8CMD(0x2a);   //Partial Mode On  局部模式
    LCD_ST7735R_WR_8DATA((uint8)(site.x >> 8)); //高8位 Sx
    LCD_ST7735R_WR_8DATA((uint8)site.x);   //低8位 Sx
    LCD_ST7735R_WR_8DATA((uint8)((site.x + size.W - 1) >> 8));
    LCD_ST7735R_WR_8DATA((uint8)(site.x + size.W - 1));

    LCD_ST7735R_WR_8CMD(0x2B);//Row Address Set  行地址设置
    LCD_ST7735R_WR_8DATA((uint8)(site.y >> 8));
    LCD_ST7735R_WR_8DATA((uint8)site.y);
    LCD_ST7735R_WR_8DATA((uint8)((site.y + size.H - 1) >> 8));
    LCD_ST7735R_WR_8DATA((uint8)(site.y + size.H - 1));

}

/*!
 *  @brief      获取 ST7735R 高度
 *  @return     ILI9341 高度
 *  @since      v5.0
 */
uint16 ST7735R_get_h()
{
    return st7735r_h;
}

/*!
 *  @brief      获取 ST7735R 宽度
 *  @return     ILI9341 宽度
 *  @since      v5.0
 */
uint16 ST7735R_get_w()
{
    return st7735r_w;
}

/*!
 *  @brief      获取 ST7735R 显示方向
 *  @return     ST7735R 方向
 *  @since      v5.0
 */
uint8 ST7735R_get_dir()
{
    return st7735r_dir;
}




