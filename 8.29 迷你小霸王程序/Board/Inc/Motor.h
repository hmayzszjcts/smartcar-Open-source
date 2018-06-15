/*
* @file         Motor.h
  * @brief      电机相关函数实现
  * @author     xiao_fang
  * @version    v1.0
  * @date       2017-3-13
*/
#ifndef __MOTOR_H__
#define __MOTOR_H__

#define MOTOR_FRE 10000               //定义电机驱动PWM周期，单位HZ

void motor_init(void);          //初始化电机
void motor_pwm_set(int32 pwm_duty);   //设置电机PWM

#endif