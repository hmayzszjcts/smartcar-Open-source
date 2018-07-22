#ifndef  _TX_PROCESS_H
#define  _TX_PROCESS_H

#define    z           0
#define    y          79

#define  white       1
#define  black       0
#define  num        45
extern uint8 imgbuff[CAMERA_SIZE];    //定义存储接收图像的数组
extern uint8 img[CAMERA_H][CAMERA_W];
//extern uint32 angle_pwm;

 extern  void  PROCESS();
 extern int16    abs_int(int16 i);
 extern float    abs_flt(float i);
 extern  void    Centerline();
 extern  void    shizi_handle();
 extern  void    nihe_center();
 extern  void    pinghua();
 extern  void    youxiao_hang();
#endif



