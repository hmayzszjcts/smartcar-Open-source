/*
* @file       Servo motor.h
  * @brief      舵机相关函数实现
  * @author     xiao_fang
  * @version    v1.0
  * @date       2017-3-12
*/
#ifndef __SERVO_MOTOR_H__
#define __SERVO_MOTOR_H__

#define SERVO_FRE 100            //定义舵机频率，50~330HZ
#define SERVO_MID 13500          //定义舵机中值占空比13500
#define SERVO_LEFT_DEAD_ZONE 15500                //定义舵机左极值占空比  偏差值为正数
#define SERVO_RIGHT_DEAD_ZONE 12000               //定义舵机右极值占空比  偏差值为负数

void servo_motor_init(void);    //初始化舵机
uint32 servo_motor_pwm_set(uint32 pwm_duty);      //设置舵机PWM

#endif
