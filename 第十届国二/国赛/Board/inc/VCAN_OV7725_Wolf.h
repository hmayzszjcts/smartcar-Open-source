#ifndef _VCAN_OV7725_WOLF_H_
#define _VCAN_OV7725_WOLF_H_

#include "VCAN_OV7725_REG.h"
#if ( USE_CAMERA == CAMERA_OV7725_WOLF )
//配置摄像头的特性
#define CAMERA_USE_HREF     0               //是否使用 行中断 (0 为 不使用，1为使用)
#define CAMERA_COLOR        1               //摄像头输出颜色 ， 0 为 黑白二值化图像 ，1 为 灰度 图像 ，2 为 RGB565 图像
#define CAMERA_POWER        0               //摄像头 电源选择， 0 为 3.3V ,1 为 5V

//配置摄像头顶层接口
#define camera_init(imgaddr)    ov7725_wolf_init(imgaddr)
#define camera_get_img()        ov7725_wolf_get_img()

#define camera_vsync()          ov7725_wolf_vsync()
#define camera_href()           //ov7725_wolf_href()
#define camera_dma()            ov7725_wolf_dma()

//配置 摄像头 参数
#define CAMERA_DMA_CH       OV7725_WOLF_DMA_CH                          //定义摄像头的DMA采集通道
#define CAMERA_W            OV7725_WOLF_W                               //定义摄像头图像宽度
#define CAMERA_H            OV7725_WOLF_H                               //定义摄像头图像高度

#define CAMERA_SIZE         OV7725_WOLF_SIZE                            //图像占用空间大小
#define CAMERA_DMA_NUM      OV7725_WOLF_DMA_NUM                         //DMA采集次数

#endif  //#if ( USE_CAMERA == CAMERA_OV7725_WOLF )  

//配置摄像头 属性
#define OV7725_WOLF_DMA_CH       DMA_CH0                                //定义摄像头的DMA采集通道
#define OV7725_WOLF_W            80                                     //定义摄像头图像宽度（用户不能修改）
#define OV7725_WOLF_H            60                                     //定义摄像头图像高度（用户不能修改）
#define OV7725_WOLF_SIZE         (OV7725_WOLF_W * OV7725_WOLF_H )       //图像占用空间大小
#define OV7725_WOLF_DMA_NUM      (OV7725_WOLF_SIZE )                    //DMA采集次数


extern  uint8   ov7725_wolf_init(uint8 *imgaddr);
extern  void    ov7725_wolf_get_img(void);

extern  void    ov7725_wolf_vsync(void);
extern  void    ov7725_wolf_dma(void);


#endif  //_VCAN_OV7725_WOLF_H_


