#ifndef __FUN_H__
#define __FUN_H__

#include "includes.h"
//#include "core_cm4.h"

#define TEST_KEY(x,y) do{x=(GPIO_PDIR_REG(PORTB)&0x0000FFFF);y=(uint16)x;}while(0)
extern uint8 gl_zhangai_x;
extern uint8 gl_zhangai_y;
extern uint8 gl_zhangai_flag;
extern uint8 stop_car_line;
extern float zhnagai_rate ;
void check_and_stop(int num,int flag);
void duoji_control(uint16 jiaodu);//舵机控制函数，注意舵机数组修改后对直道影响
void ti_jiaozheng(uint8 start,uint8 end);//矫正梯形失真
void PickCenter_diff(uint16 line);
int32 PickCenter_near();//寻找近处中心线
void PickCenter_new();
uint32 abs_sub(uint32 diff1,uint32 diff2);//两个数相减的绝对值
void lvbo(uint8 );//滤波函数
void bu_xian();//补线函数
void find_deal_zhangai();//找障碍物和补线
void center_buxian();//中心线补线
extern int mark_stop();
int check_and_stop1();
extern int stop_car_flag;
int regression(uint16 Pick_table[],int startline,int endline);//计算斜率函数
int vl_ratio();//有效行的斜率

extern void single_line_jiaozheng(int line_ary[],int start ,int end);

#endif 
		 //gpio_reverse(PORTB,10);