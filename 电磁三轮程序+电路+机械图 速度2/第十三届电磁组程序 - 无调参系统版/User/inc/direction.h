#ifndef __DIRECTION_H__
#define __DIRECTION_H__

#include  "common.h"



/**********全局变量外部申明********/
extern float g_dirControl_P;
extern float g_dirControl_D;
extern float g_fDirectionError[2];
extern float g_fDirectionError_dot[2];
extern float g_fDirectionControlOut;
extern int16 g_ValueOfAD[4];
extern int16 g_ValueOfADFilter[4];




/***********函数声明***********/

void DirectionControl(void);
void Read_ADC(void);

#endif