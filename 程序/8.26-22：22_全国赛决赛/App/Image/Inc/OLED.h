#ifndef __MY_I2C_H__
#define __MY_I2C_H__
#include "common.h"
#include "include.h"

//OLED控制用函数

#define READ_SDA        gpio_get (PTC14)
#define OLED_IIC_SCL0() gpio_set (PTC15, 0)	// IO口输出低电平
#define OLED_IIC_SCL1() gpio_set (PTC15, 1)	// IO口输出高电平 

#define OLED_IIC_SDA0() gpio_set (PTC14, 0)	// IO口输出低电平//SCL IIC接口的数据信号
#define OLED_IIC_SDA1() gpio_set (PTC14, 1)	// IO口输出低电平
#define SDA_OUT()        gpio_ddr (PTC14, GPO)
#define OLED_SDA_IN()    gpio_ddr (PTC14, GPI) 

#define byte   unsigned char //自己加的
#define word  unsigned int   //自己加的

//OLED控制用函数

void OLED_IIC_Init(void);
void OLED_IIC_Start(void);
void OLED_IIC_Stop(void);
//void SendACK(uint8 ack_dat);
static int I2C_WaitAck(void);
void Write_OneByte(unsigned char Dat);
void WriteCmd(unsigned char IIC_Cmd);
void WriteDat(unsigned char IIC_Dat);
void OLED_Init(void);
void OLED_SetPos(unsigned char x, unsigned char y); 
void OLED_Fill(unsigned char fill_Data);
void OLED_CLS(void);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);
void OLED_ShowNum(uint8 x,uint8 y,uint8 num);
void OLED_ShowLongNum(uint8 x,uint8 y,uint16 num);

void OLED_WrDat(uint8 IIC_Data);
void OLED_Set_Pos(uint8 x, uint8 y) ;

void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);

void Display_img(void);



#endif