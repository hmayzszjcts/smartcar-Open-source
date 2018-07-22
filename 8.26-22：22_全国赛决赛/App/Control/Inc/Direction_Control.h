#ifndef __DIRECTION_CONTROL_H__
#define __DIRECTION_CONTROL_H__

#include "common.h"
#include "include.h"


extern uint8 screen_mid;		  //屏幕中点
extern uint8    Point_Mid;
extern uint8    Point_Left;
extern uint8    Point_Right;
extern uint8    Hazard_Left_Right;
extern int32 S3010_Duty;      //       //4320  4740 5180
extern uint8 Point;	//目标点横坐标
extern float    CS_P_Right,CS_P_Left;
extern int8    Point_Num;
extern int8     CS_Point_Num;




void S3010_Control(void);
void CS_Control(void);

#endif /* __CONTROL_H__ */