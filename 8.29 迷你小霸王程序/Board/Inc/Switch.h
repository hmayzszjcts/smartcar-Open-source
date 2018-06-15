/*
* @file         Switch.h
  * @brief      拨码开关相关函数实现
  * @author     xiao_fang
  * @version    v1.0
  * @date       2017-3-13
*/
#ifndef __SWITCH_H__
#define __SWITCH_H__

void switch_button_init(void);         //初始化拨码开关
uint8 switch_read(void);        //读取拨码开关状态
uint8 button_read(void);         //读取按键状态
#endif
