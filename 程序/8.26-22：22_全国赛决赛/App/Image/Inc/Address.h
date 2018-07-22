#ifndef __ADDRESS_H__
#define __ADDRESS_H__
#include "common.h"
#include "include.h"

/**************************摄像头接收解压数组******************************/
extern uint8  imgbuff1[600];         //采集图像存放数组1
extern uint8  imgbuff2[600];         //采集图像存放数组2
extern uint8 *  p_imgbuff1;   //图像地址1
extern uint8 *  p_imgbuff2;   //图像地址2
extern uint8  flag_imgbuff;
extern uint8  img[60*80];

extern uint8  bmp_buff[1024];
extern uint8  compress_buff[120][128];


#endif