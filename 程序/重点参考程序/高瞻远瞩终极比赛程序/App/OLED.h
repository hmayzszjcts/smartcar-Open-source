#ifndef	_OLED_H_
#define _OLED_H_

//#include "main.h"
//#include  "include.h"

#include "include.h"
//#include "calculation.h"

#define DIF(x,y)     (y>x?y-x:x-y)

#define ROWSIZE   8
#define COLSIZE    128


//
/*
#define D0    12
#define D1    6
#define RSET  7
#define DC    5
#define OLED_GPIO_RST  PTA
#define OLED_GPIO_DC   PTA
#define OLED_GPIO_D0   PTA
#define OLED_GPIO_D1   PTA

gpio_init(PTE2,GPO,1u);
gpio_init(PTE4,GPO,1u);  
gpio_init(PTE5,GPO,1u);
gpio_init(PTE7,GPO,1u);
*/
#define DCLOW()     gpio_set(PTE4 ,0);
#define DCHIGH()    gpio_set(PTE4 ,1);
#define CLKLOW()    gpio_set(PTE5 ,0);
#define CLKHIGH()   gpio_set(PTE5 ,1);
#define MISOLOW()   gpio_set(PTE2 ,0);
#define MISOHIGH()  gpio_set(PTE2 ,1);
#define RESLOW()    gpio_set(PTE7 ,0);
#define RESHIGH()   gpio_set(PTE7 ,1);
//

extern unsigned char F24x24[];
extern unsigned char F16x16tbm[];
/*
#define D0 GPIO_PIN_2
#define D1 GPIO_PIN_0
#define RSET GPIO_PIN_4
#define DC GPIO_PIN_3
#define OLED_GPIO_RST GPIOD
#define OLED_GPIO_DC GPIOD
#define OLED_GPIO_D0 GPIOD
#define OLED_GPIO_D1 GPIOD

#define DCLOW() GPIO_WriteLow(OLED_GPIO_DC, DC);
#define DCHIGH() GPIO_WriteHigh(OLED_GPIO_DC, DC);
#define CLKLOW() GPIO_WriteLow(OLED_GPIO_D0, D0);
#define CLKHIGH() GPIO_WriteHigh(OLED_GPIO_D0, D0);
#define MISOLOW() GPIO_WriteLow(OLED_GPIO_D1, D1);
#define MISOHIGH() GPIO_WriteHigh(OLED_GPIO_D1, D1);
#define RESLOW() GPIO_WriteLow(OLED_GPIO_RST, RSET);
#define RESHIGH()  GPIO_WriteHigh(OLED_GPIO_RST, RSET);
*/
#define byte uint8
#define word uint16


#define GPIO_PIN_MASK      0x1Fu    //0x1f=31,限制位数为0--31有效
#define GPIO_PIN(x)        (((1)<<(x & GPIO_PIN_MASK)))  //把当前位置1


extern unsigned char OLED_GRAM[];
extern unsigned char OLED_VIEW[];
extern unsigned char BMP3[];
extern unsigned char DRAWPOTm[];
extern unsigned char DRAWPOTtempm[];

 void OLED_Init(void);
 void LCD_DLY_ms(word ms);
 void LCD_CLS(void);
 void LCD_P6x8Str(byte x,byte y,byte ch[]);
 void LCD_P8x16Str(byte x,byte y,byte ch[]);
 void LCD_P14x16Str(byte x,byte y,byte ch[]);
 void LCD_Print(byte x, byte y, byte ch[]);
 void LCD_PutPixel(byte x,byte y);
 void LCD_Rectangle(byte x1,byte y1,byte x2,byte y2,byte gif);
 void Draw_LQLogo(void);
 void Draw_LibLogo(void);
 void Draw_Image(void);
 void Draw_BMP(byte x0,byte y0,byte x1,byte y1,byte bmp[]);
 void LCD_Fill(byte dat);
 void Dis_Num(byte y, byte x, unsigned int num,byte N) ;
 void LCD_Set_Pos(byte x, byte y);
 void Dis_Numbfb(byte y, byte x, uint8 num);
 void Draw_Power(u16 TSC,uint8 qie);
 void Draw_XH();
 void Draw_WZ(s16 WEI2);
 void Dis_err(byte y, byte x, s16 num);
 void Dis_LOGO();
 void Display_Bar(unsigned char x,unsigned char y,unsigned char num);

 void OLED_clearbuffer(void);
 void OLED_upbuffer(void);
 void DrawLine(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char color);
 void SetPixelbuffer(unsigned char x, unsigned char y, unsigned char color);

 void Draw_BMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
 void Draw_pot(unsigned char x,unsigned char y,unsigned char t);
 void LCD_PXXxYYCHtoBMP(unsigned char xx,unsigned char yy,unsigned char FXXxYY[],unsigned char x,unsigned char y,unsigned char N,unsigned char p);
 void ceshi(void);
 void ceshi2(void);
 void gundong(void);
 
void clear_drawpottempm(void);
void LCD_P8x16StrtoBMPL(unsigned char x,unsigned char y,signed char ch[],unsigned char z);
void LCD_PXXxYYCHtoBMPL(unsigned char xx,unsigned char yy,unsigned char FXXxYY[],unsigned char x,unsigned char y,unsigned char N,unsigned char p,unsigned char c);//字符大小XX*YY,对应大小的数组， x坐标，y坐标，第几个数或者字符，阴文/阳文,写入哪个GRAM
void ceshi_lock(void);


void OLED_Draw_pot(unsigned char x,unsigned char y,unsigned char t,unsigned char c);
void OLED_DrawLine_X(unsigned char x0,unsigned char x1,unsigned char y,unsigned char c);
void OLED_DrawLine_Y(unsigned char y0,unsigned char y1,unsigned char x,unsigned char c);
void OLED_write_6X8String(signed char x,signed char y,signed char ch[],unsigned char z);
void OLED_write_8X16String(unsigned char x,unsigned char y,char ch[],unsigned char z);
void OLED_Diamondhero_Draw_XXYY(unsigned char xx,unsigned char yy,unsigned char FXXxYY[],unsigned char x,unsigned char y,unsigned char N,unsigned char p,unsigned char c);
void OLED_VIEW_OUT(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
void clear_OLED_GRAM(void);
void VIEW_Rolling_effect(void);

void countprint(int x,char y[]);
//void countprint(float x,char y[]);
//int abs(long double);

void OLED_ceshi(void);
void OLED_ceshi2(void);
void OLED_ceshi3(void);
void OLED_ceshi4(void);
void OLED_ceshi5(void);

void VIEW_Rolling_effect2(void);
void VIEW_Rolling_effect_R(void);
void VIEW_Rolling_effect_R2(void);


 void LCD_PXXxYYCHtoBMPL(unsigned char xx,unsigned char yy,unsigned char FXXxYY[],unsigned char x,unsigned char y,unsigned char N,unsigned char p,unsigned char c);


//int Backtom(int flag,int k);
//unsigned int gundong_128(uint flag);
uint gundong_128(uint flag);

void DrawLineX_fL(unsigned char x0,unsigned char x1,unsigned char y,unsigned char c);
 void DrawLineY_fL(unsigned char y0,unsigned char y1,unsigned char x,unsigned char c) ;
 void Draw_potL(unsigned char x,unsigned char y,unsigned char t,unsigned char c);
 
 #endif
