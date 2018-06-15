#ifndef __ROUNDADOUT_H_
#define __ROUNDADOUT_H_

#include "common.h"

extern void round_deal(void);

extern int find_weight(void);

extern int start_guai_flag;//开始补线标志位
extern int ru_ring();

extern void ring_find_line(void);

extern int line_lockup(uint16 line[],int line_val);

extern int round_find_line(uint16 left_line[],uint16 rightline[]);

extern int find_same_distance_line(int num_line);//12

extern int ring_lose_line_flag;

extern int ring_weight_flag;

extern int ring_same_distance_flag;

extern int start_fill_line_flag;//开始补线标志位

extern int ring_max_diffe_flag;

#endif



