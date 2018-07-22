#ifndef __MOTOR_CONTROL_H__
#define __MOTOR_CONTROL_H__

#include "common.h"
#include "include.h"

//定义电机端口		//PTC1~	PTC4
#define MOTOR_FTM   FTM3
#define Left_MOTOR1_PWM  FTM_CH6     //PTC3
#define Left_MOTOR2_PWM  FTM_CH7     //PTC4
#define Right_MOTOR1_PWM FTM_CH4     //PTC1
#define Right_MOTOR2_PWM FTM_CH5     //PTC2
#define MOTOR_HZ    15000	//滑行模式下，频率应该是 30~100。选用20kHz
								//常规模式下，频率应该是 20k 左右
extern int32 Left_MOTOR_Duty;       //电机左
extern int32 Right_MOTOR_Duty;       //电机右
extern int32 Left_MOTOR_Speed;
extern int32 Right_MOTOR_Speed;
extern uint8 Run_Flag;
extern uint8 Game_End;
extern uint8 Stop_Flag;
extern uint8 BangBang_Flag;
extern uint8 Point;
extern int16 High_Speed;
extern int16 Ramp_Speed;
extern char Set,Speed_Set;
extern uint16 Ramp_Delay; 
extern float g_fDirectionControlOut;
extern int8    Point_Num;
//定义舵机端口		//PTC9		
#define S3010_FTM FTM0
#define S3010_CH  FTM_CH0
#define S3010_HZ  50
//#define S3010_MID 4750	//  4770
//#define S3010_LEFT 5200	//  5300
//#define S3010_RIGHT 4320	//  4400
#define S3010_MID 4750	//  4770
#define S3010_LEFT 5180	//  5300
#define S3010_RIGHT 4320	//  4400
extern uint8   Point_Mid;//目标点横坐标





/*********** 函数声明 ************/
void Adjust_Speed(void);
void speed_measure(void);		//电机速度测量
void MOTOR_Control(void);	//增量式PID调控电机速度
void Starting_Check(void);
void DirectionControl(uchar Point);

int32 range_protect(int32 duty, int32 min, int32 max); //限幅保护

#endif /* __CONTROL_H__ */