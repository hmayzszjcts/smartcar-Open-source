#ifndef _OLED_H_
#define _OLED_H_
//定义输出

extern uint8  reverse; //反转显示\

void OLED_WrCmd(unsigned char cmd);

//显示一个6x8标准ASCII字符
void OLED_P6x8Char(unsigned char ch);
//清屏
void OLED_Fill(unsigned char bmp_data);
//功能描述：写入一组6x8标准ASCII字符串
void OLED_P6x8Str(unsigned char x,unsigned char y,char ch[]); 
//将一个char型数转换成3位数进行显示 
void OLED_PrintValueC(unsigned char x, unsigned char y, int data);
//功能描述：将一个int型数转换成5位数进行显示
void OLED_PrintValueI(unsigned char x, unsigned char y, int data);
//显示unsigned int型
void OLED_PrintValueFP(unsigned int data, unsigned char num);
//功能描述：将一个float型数转换成整数部分5位带小数和符号的数据并进行显示
void OLED_PrintValueF(unsigned char x, unsigned char y, float data, unsigned char num);
//写数据函数
void OLED_WrDat(unsigned char data,unsigned char revs);
//设置显示坐标位置
void OLED_Set_Pos(unsigned char x, unsigned char y);
//OLED初始化
void OLED_Init(void);

#endif