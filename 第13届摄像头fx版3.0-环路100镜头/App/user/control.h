#ifndef _CONTROI_H_
#define _CONTROI_H_
//#include "common.h"
#include "include.h"
#include "includes.h"
extern const char img_array[];

#define MAX_CENTER_POINT_COUNT 25 //最大的中心线点数
#define MAX_LINE_POINT_COUNT   60 //一条线最大的点数
#define MIN_LINE_POINT_COUNT   5 //一条线最大的点数
#define BREAKPOINT             40 //近处的道和远处的道路分界点  

#define RATE_COUNT	       8  //找拐点时候比较的点数

#define START_Y  5       

#define PIT_PORT PIT1 //控制周期的定时器
#define PIT_CNT 6 //定时10ms   6

#define PIT_PORT_time PIT2 //计时
#define PIT_time 20//定时16ms
typedef struct 
{
	int x;
	int y;
}Point;
typedef struct 
{
	int start_point; //起始点的数组下标,-1代表没找到
	int end_point;   //起始点的数组下标
	int point[MAX_LINE_POINT_COUNT];
}Line_struct;
typedef struct 
{
    int usr_count;
    int lift_point;
    int right_point;
    int site[MAX_CENTER_POINT_COUNT];
}center_point_Site;
extern uint8 imgbuff[CAMERA_SIZE];    
extern center_point_Site center_point;
extern Line_struct lift_line;
extern Line_struct right_line;
extern Line_struct gl_combine_line;
extern uint8 run_start_flag;
extern uint8 my_debug_flag;
extern uint32 total_time;

extern uint8 Lost_Line_count;
extern uint8 Lost_left_count;
extern uint8 Lost_right_count;
extern uint8 last_vline;
extern uint8 valid_line;//最大有效行
extern uint8 judge_vl;//用于判断的有效行
extern uint8 last_lost;	//上一场丢失行
extern uint16 Bline_diff;//两黑线距离

extern int RING_FLAG;//圆环标志位
extern int HIGHT_RING_FLAG;//圆环标志位
extern int ring_num;		//记判断圆环个数
extern int no_ring_num;

extern int big_fing_ring;
extern uint8 Shi_zi_flag;
extern int judge_xielv[CAMERA_H-5];	//斜率判断数组 
extern uint8 zhidao_count_flag;	//直道判断标志
extern uint8 last_zhidao_flag;
extern uint8 lost_w_count;//白色丢失行变量
extern uint8 lost_b_count;//黑色丢失行变量

extern int even_diff;	//中心线平均偏差
extern int even_diff_near;
extern float D_differen;    //
extern int jiaodu_num;	//角度值
extern int last_turn;	//上一次转向值
extern int dj_pid_num;	
extern uint16 maxBline_diff;
extern int dj_act_jiaodu;//舵机实际角度

extern uint8 run_start_flag;//是否开始跑
extern uint8 img_deal_flag;//开始采集处理图像
extern uint8 gl_zhangai_flag;
extern uint8 gl_zhangai_start;
extern uint8 gl_zhangai_end;
////////////////////////////////全局变量    定义////////////////////////////////////

extern uint16 Bline_left[CAMERA_H];	 //左边线存放数组
extern uint16 Bline_right[CAMERA_H];	 //右边线存放数组
extern uint16 Pick_table[CAMERA_H];	 //中心线存放数组

extern uint16 left_pos[CAMERA_H];	 //左边线存放数组
extern uint16 right_pos[CAMERA_H];	 //右边线存放数组
extern uint16 center_pos[CAMERA_H];	 //中心线存放数组

extern uint8  Pick_flag[CAMERA_H];//该行是否找到黑线标志数组
extern uint8  Deal_flag[CAMERA_H];//处理数据是否有效标志数组
extern uint16 lost_already;
extern uint8 Pick_line;
extern uint8 Out_flag;		//出界标志，无效图像标志

extern int all_var_init();
void init_control_circle();
extern int auto_drive();//获取图像后调用此函数控制速度和方向
extern void stable_del();
extern void ctrl_main();
void init_ctrl();
void getBlineCenter();
void shizi_find_line(void);//��������
void averageLvBo();
void InitPar(void);
extern int in_shizi_flag;
extern int out_shizi_flag;
extern int shizi_on_off;
extern int loop_guaidian_flag;
extern int other;
extern int other_num;
extern int ratio;

extern int START_FILL_FLAG;

extern int fact_weight[60];
extern int line_lose_flag;
extern int val;
extern int w;
extern int ww;
extern int ring_val_hang;
extern int ring_size;
extern int normal_flag;
extern int enter_ring_kp_flag;

extern int out;
#endif
