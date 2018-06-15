/*
* @file         Reed.h
  * @brief      干簧管相关函数实现
  * @author     xiao_fang
  * @version    v1.0
  * @date       2017-5-23
*/
#ifndef __REED_H__
#define __REED_H__

void reed_detection(void);     //干簧管停车检测
void portaIRQHandler(void);     //干簧管外部中断

#endif