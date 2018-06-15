#ifndef __SPEED_H__
#define __SPEED_H__

#include  "common.h"



/**********全局变量外部申明********/
extern int32 g_nLeftpulse;
extern int32 g_nRighpulse;
extern float g_fSpeedError;
extern float g_fRealSpeed;
extern float g_fLeftRealSpeed;
extern float g_fRighRealSpeed;
extern float g_fSpeedFilter;
extern float fSpeedErrorInteg;
extern float g_fSpeedControlOut;
extern float g_PWMOut;
extern int32 g_nLeftPWM, g_nRighPWM;
extern float g_speedControl_P;		//差速比例
extern float g_speedControl_I;		//位移比例
extern float g_fExpectSpeed;

/***********函数声明***********/
void MotorInit(void);
void PWMOut(void);
void CalSpeedError(void);
void SpeedControl(void);


#endif