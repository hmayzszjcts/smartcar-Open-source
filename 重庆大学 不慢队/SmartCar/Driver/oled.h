#ifndef	_OLED_H_
#define _OLED_H_

#define RESET  PTA16_OUT
#define DC     PTA17_OUT
#define D0     PTA24_OUT
#define D1     PTA25_OUT

 extern uint8  reverse; //反转显示
 typedef unsigned char byte;
 typedef unsigned int word;
 
 void OLED_Set_Pos(byte x, byte y);
 void OLED_Init(void);
 void OLED_CLS(void);
 //重载OLED_WrDat
 void OLED_WrDat(byte data, byte revs);
 //显示一个6x8标准ASCII字符
void OLED_P6x8Char(unsigned char ch);
 void OLED_P6x8Str(byte x,byte y,byte ch[]);
//将一个char型数转换成3位数进行显示 
void OLED_PrintValueC(unsigned char x, unsigned char y, int data);
//功能描述：将一个int型数转换成5位数进行显示
void OLED_PrintValueI(unsigned char x, unsigned char y, int data);
//显示unsigned int型
void OLED_PrintValueFP(unsigned int data, unsigned char num);
//功能描述：将一个float型数转换成整数部分5位带小数和符号的数据并进行显示
void OLED_PrintValueF(unsigned char x, unsigned char y, float data, unsigned char num);
 void OLED_Fill(byte dat);
#endif