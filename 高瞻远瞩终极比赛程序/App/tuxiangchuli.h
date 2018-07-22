#include "include.h"
extern s16 current_deviation;

extern uint8 imgErwei[CAMERA_W][CAMERA_H]; 
extern char SXTzhongjian[61];
extern int SXTzuobian[60],SXTyoubian[60];
void tuxiangzhuanhuan(void);
void bianxiantiqu(void);
void LCD_huaxian(int x_0,int y_0,int x_1,int y_1);
//void DDALine(int x0,int y0,int x1,int y1);

void DSJ_pianchachuli(void);
void DST_SXT_duojizhuanxiang(int err);