#ifndef _DIRECTION_H_
#define _DIRECTION_H_
#include "common.h"

extern void init_dircetion();
extern void duoji_init();
extern void change_angle(uint32 duty);
extern int control_direction();
extern void duoji_PD(int err);
void way_control();


#endif
