#include "Address.h"


uint8  imgbuff1[CAMERA_SIZE];         //采集图像存放数组1
uint8  imgbuff2[CAMERA_SIZE];         //采集图像存放数组2
uint8 *  p_imgbuff1 = (uint8 *)((uint8 *)&imgbuff1);   //图像地址1
uint8 *  p_imgbuff2 = (uint8 *)((uint8 *)&imgbuff2);   //图像地址2
uint8  flag_imgbuff = 0;
uint8 img[CAMERA_W*CAMERA_H]; //由于鹰眼摄像头是一字节8个像素，因而需要解压为 1字节1个像素，方便处理

//用于oled显示图像的两个数组
uint8  bmp_buff[1024]={0};			
uint8  compress_buff[120][128]={0};
