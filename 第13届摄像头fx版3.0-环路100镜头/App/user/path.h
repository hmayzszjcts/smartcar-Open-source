#ifndef _PATH_H_
#define _PATH_H_
#include "common.h"
#include "include.h"
#include "control.h"
#define IMG_BUF imgbuff   //宏定义图像数组名
#define IMG_H 58
#define IMG_W 80
#define FILL_LEN 70 //用于补全丢失的线          //70
#define H_U_AND_D IMG_H //上边的y坐标与下边y坐标的差值 
#define X_RENEW(x,y) (x-DOWN_EDGE/2)*DOWN_EDGE*H_U_AND_D/(UP_EDGE*H_U_AND_D+(DOWN_EDGE-UP_EDGE)*y)+DOWN_EDGE/2

#define FIND_X_MAX_COUNT 10		 //找边线时左右寻找的个数
#define xiao_wan 10

#define zhi_dao 3

//#define get_img_point(h,w) IMG_BUF[(IMG_W*(h)+(w))>>3]&(1 << (7 - ((w) & 0x07) ))
enum PATH_TYPE
{
    Long_zhidao,//长直道
    Short_zhidao,//短直道
    lean_zhidao,//斜入直道
    Shi_zi,//十字   
    Zhang_ai,//障碍    
    Xiao_S,//小S湾    
    Zhong_S,//中S弯  
    Da_S,//大S弯    
    Xiao_wan,//小弯    
    Zhong_wan,//中弯    
    Da_wan,//大弯   
    T_Da_wan,//特大弯
    Da_turn,//大转向
    Loop_road,//环路
};

extern uint8 guai_dian_count;
extern uint8 gl_zhidao_count;
extern uint8 donw_guan_dian;
extern uint8 S_offset_flag;
extern uint8 gl_path_type;
extern uint8 gl_shang_po_flag;
extern uint8 imgbuff[CAMERA_SIZE]; 
extern uint8 ring_road_w;
extern int center_num;
extern int ramp_flag;
extern int get_path();
extern int get_path_type(Point * path_array,int num);
uint8 If_straight(uint8 start,uint8 end,uint8 val);
int get_img_point(uint16 h,uint16 w);
void get_even_diff_s();
void with_left_to_center();
void with_right_to_center();
void bDistance(void);//赛道宽度法滤波 去除无效行
void xielv_lvbo(void);//跳变差值限制法滤波 去除无效行
int test_draw_point(uint16 line,uint16 x,uint16 color);
void If_LStraight();
int beleved_shizi_loop();
extern int shizi_wait;
extern int temp[60];
extern int zhidao_loop_flag;

extern int line_weight[60];

#endif
