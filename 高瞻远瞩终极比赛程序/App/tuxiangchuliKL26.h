#include "include.h"
extern s16 current_deviation;

extern int zhongxiantingzhihang;
extern int youxiaohangxiayihang_zhongxian;
extern int youxiaohangxiayihang_zuobian,youxiaohangxiayihang_youbian;
extern int DST_SXT_duoji_kp;//30;//25、、30..31
extern float DSJ_XL;
extern int DSJ_YXH;
extern int DSJ_piancha;

extern int DSJ_SXT_zhongjian_count;
extern float DSJ_zhongjian_ave;
extern float DSJ_ave_piancha;

extern int quanbai_flag;
extern int danbian_flag;
extern int guaidian_flag;
extern int zhijiao_guaidian_flag;
extern int Zhijiao_zuoguai_flag,Zhijiao_youguai_flag,Zhijiao_guai_count;

extern int danxian_chushizhongdian;

extern int shuangxian_danxia,danxian_shuangxian;
extern int zuobian_52hang,youbian_52hang,last_zuobian_52hang,last_youbian_52hang,zhongjian_52hang,danxian_chixu_count;

extern int diyitiao_tiaobianhang,shuangxianbiandanxian;

extern int yihangheidian_count,dierhangheidian_count;

extern int DSJ_shuangxianjindanxian_flag,DSJ_danxian_flag,DSJ_danxianjinshuangxian_flag,DSJ_shuangxian_flag;
extern int danxianjinshuangxian_diyihang;
extern int SXT_chundan_flag,SXT_chunshuang_flag,SXT_dan_shuang_flag,SXT_shuang_dan_flag;
extern int SXT_chundan_flag_count,SXT_chunshuang_flag_count,SXT_dan_shuang_flag_count,SXT_shuang_dan_flag_count;
//extern uint8 imgErwei[CAMERA_W][CAMERA_H]; 
extern char last_SXTzhongjian[61],SXTzhongjian[61];
extern int SXTzuobian[60],SXTyoubian[60];


extern int zhijiao_queren_zuo_flag,zhijiao_queren_you_flag;
void tuxiangzhuanhuan(void);
void bianxiantiqu(void);
void LCD_huaxian(int x_0,int y_0,int x_1,int y_1);
//void DDALine(int x0,int y0,int x1,int y1);

//extern uint8 img[CAMERA_W*CAMERA_H];                           //由于鹰眼摄像头是一字节8个像素，因而需要解压为 1字节1个像素，方便处理
//extern uint8 imgyiwei[CAMERA_W][CAMERA_H]; 
//extern uint8 imgyiwei[60][80]; 

void DSJ_pianchachuli(void);
void DST_SXT_duojizhuanxiang(int err);
void danxianchuli(void);
//void shuangxian_danxianpanduan(void);


int danxian_shuangxianpanduan(void);
int shuangxian_danxianpanduan(void);
void flag_clr(void);
void shuangxianchuli(void);
void shuang_dan_chuli(void);