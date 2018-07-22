#ifndef _LCDkongzhi_H
#define _LCDkongzhi_H
#include "include.h"
/**********************LCD功能标志位******************************/
extern uint8 flag_LCD;                         //LCD界面标志
extern uint8 flag_LCD_revise;                  //resive参数修改选择
extern uint8 flag_LCD_queren;                  //确认标志位

extern uint8 flag_LCD_first;                      //first界面
extern unsigned char select_tm[10]; 
extern unsigned char flag_tubiao_viewm[10];
extern unsigned int flag_gundongs[10];
extern uint8 flag_selectm;                     //液晶选择标志
extern uint8 flag_gundongm;                    //液晶返回滚动标志

extern s16 piancha_lcd;
extern int DCsignal[4];

extern uint8 flag_blueteethope;                  //蓝牙小图标显示标志
extern uint8 flag_stop;                           //停车小图标标志，包括停车是否控制标志


extern int keyceshi;
//extern uint8 img[CAMERA_W*CAMERA_H];                           //由于鹰眼摄像头是一字节8个像素，因而需要解压为 1字节1个像素，方便处理
//extern uint8 imgyiwei[CAMERA_W][CAMERA_H]; 
extern uint8 imgyiwei[60][80]; 




extern int MMA7361_sj;
extern int GYRO_X;
extern int GYRO_Y;

typedef struct
{
  u16 CCD1_int;           
  char CCD1_char[4];
  u16 CCD2_int;
  char CCD2_char[4];
  u16 CCD3_int;
  char CCD3_char[4];
  u16 CCD4_int;
  char CCD4_char[4];
   u16 CCD5_int;
  char CCD5_char[4];
    u16 CCD6_int;
  char CCD6_char[4];
  
}CCDxianshi;

extern CCDxianshi CCD_LIVE_WATCH;      


/*************************END************************/
void zhujiemian(void);
void LCD_view(void);
void diyihangview(void);

void ceshi_piancha(void);
void LCD_6x8StrtoBMPL(unsigned char x,unsigned char y,signed char ch[],unsigned char z);
void xiaoche_lock(void) ;
void clear_drawpotm_k60(void);
void ceshi_stop(void);


void dijiujie_speed_tiaoshi(void);
int Backtom(int flag,int k);

void ceshi_tuxiangxianshi(void);
int Parameterset(int p);  

int Parameterset20(int p);   
//参数set
void ceshi_speedlevel(void);

void dijiujie_speed_tiaoshi2(void);
void LCDcanshuchushihua();
void DMX_chengxuchushihua();
void DSYJzhilishujuxianshi();
void OLED_flash_memories();//flash写入显示
void OLED_flash_memories_queren();//flash写入显示
void OLED_starting_car();//flash写入显示
void OLED_start_finish();//flash写入显示
void OLED_speed_set();//flash写入显示
void VIEW_CCD_dongtai(u8 x[]);
#endif