#ifndef __ROUNDADOUT_H_
#define __ROUNDADOUT_H_

#include "common.h"

extern void round_deal(void);

extern int find_weight(void);

extern int line_lockup(uint16 line[],int line_val);

extern int round_find_line(uint16 left_line[],uint16 rightline[]);

extern int find_same_distance_line(int num_line);//12

extern int ring_lose_line_flag;

extern int ring_weight_flag;

extern int ring_same_distance_flag;

extern int start_fill_line_flag;//开始补线标志位

extern int ring_max_diffe_flag;
extern int diffe_num_flag;//
extern int line_chose_flag;

extern int direct_ring_flag;//环路补线后 入环 打死 标志

extern int no_shizi_flag;

extern int order_flag;
extern int ring_max_hang;

extern int right_max_guai_flag;
extern int left_max_guai_flag;
extern int in_ring_flag;
extern int diuhang_num_flag;

extern int short_buxian_flag;
extern int ring_vl_flag;//环路有效行标志

int ring_pick_table_deal(uint16 center_line[]);
extern int ring_time_num;//出环路计时 
extern void ring_clear();
extern int ring_left_flag;
extern int get_val(int start,int end,uint16 a[],int flag);
 
extern int ring_right_flag;
 
extern int ring_weight_change_flag;
 
extern int ring_fill_line_flag;

extern int ring_maxjiao_flag;
extern int maxjiao_num;
extern uint8 pick_pos(uint16 ary[]);
extern uint8 pick_pos1(uint16 ary[]);
extern void analysis(int * x, int * y, int start,int n)  ;
extern int analysis_hang[60];
extern int a,b,mx,my,r;
extern int normal_ring_flag;
extern int out_max_ring_jiao_flag;

extern int ring_hight_flag;
extern int out_ring_ok_flag;
extern int find_out_ring_flag(int start,int end);//寻找出环路的标志
extern void out_ring_under_find_line();

extern int under_flag;
extern int add_under_hang();

extern int guai_shizi_ring_flag;
extern int out_ring_flag;
extern int press_flag;
extern int ring_hang_pos,ring_lie_pos;
extern int  get_out_ring_zhedian();
extern int out_dasi_flag;
extern int recognition_B_num;//环路尺寸 黑点数量
extern int black_point_start;
extern int start_line_ring_flag;//

#endif



