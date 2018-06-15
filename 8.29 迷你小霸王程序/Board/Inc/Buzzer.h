/*
* @file         Buzzer.h
  * @brief      蜂鸣器相关函数实现
  * @author     xiao_fang
  * @version    v1.0
  * @date       2017-3-14
*/
#ifndef __BUZZER_H__
#define __BUZZER_H__

#define BEEP_TIME 200           //单次短响时长，单位ms

void Buzzer_init(void);                 //初始化蜂鸣器
void Buzzer_beep(uint8 beep_time);              //蜂鸣器短响

#define BEEP_ON (GPIO_SET(PTE6,1))             //蜂鸣器开
#define BEEP_OFF (GPIO_SET(PTE6,0))            //蜂鸣器关

#endif