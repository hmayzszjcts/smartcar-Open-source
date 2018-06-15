
#ifndef MY_CFG
#define MY_CFG
#include "common.h"
/**************************************************/
/*
    调试说明：已将将常用的参数基本都放到my_cfg.c和my_cfg.h中，
              先把整个这个文件的注释看一遍
    山外小彩屏使用说明：按键上下是上下选项和加减操作，向左是返回，
                        向右是确定，任何状态下按中间都是开始跑
    第一步先调图像的梯形矫正，改变下边的DOWN_EDGE和UP_EDGE
    第二步，调节舵机中值，以及极值与中值的差值，差值尽量在350到500之间，
            可以通过调节 FTM3_PRECISON 来改变差值，在MK60_FTM.h中
    第三步，测试舵机，把舵机固定在中值上，用手推车能走一段长距离直到即可
            测试舵机左右的差值是否是左右正好打死角
    第四步，打开宏 OPEN_CIRCLE_CONTROL 进行开环调试，逐渐增加OPEN_CIRCLE_SPEED的值
            测试速度能到多少，可以先开环调试，改变转向P来改变路径
    第五步，关掉宏 OPEN_CIRCLE_CONTROL 进行闭环调试，调试PID，以及最大速度和最小速度
*/

//#define OPEN_CIRCLE_CONTROL //开环控制
#define OPEN_CIRCLE_SPEED 4300

#define WAY_P 0  //选择 分段p还是固定p(舵机)  
                 //  1；分段P   0:固定P
#define DUOJI_P 15  //全程的P的大小（直道除外,直道越小越好） 15

//****下边是用于梯形矫正的公式及常量的宏
//***把车放在直道上，数出最下边的白色点数给 DOWN_EDGE 赋值
//**改变 UP_EDGE 的值，是彩屏上显示的还原的线接近平行，说明矫正成功
#define DOWN_EDGE 60 //63  //图像下边沿的像素点个数 64 70 63
#define UP_EDGE   12//9   //在与图像下边沿所表示的等长的距离在图形上边沿所占的像素点个数  16
//63  12
/*图像的使用距离，计算误差用的*/
extern uint8 MIX_distance;//最近的距离  15 10 5  10 12
extern uint8 diff_valid_line;//前瞻，可以通过小彩屏调节

extern int gl_var_1;//备用的变量，可以在彩屏set img中调数
extern int gl_var_2;//备用的变量，可以在彩屏set img中调数
extern int gl_var_3;//备用的变量，可以在彩屏set img中调数
extern int gl_var_4;//备用的变量，可以在彩屏set img中调数
/*舵机参数  引脚--PTE5*/
#define DUO_JI FTM3 //舵机用的定时器
#define DUO_JI_CH FTM_CH0//舵机定时器的通道
#define dj_center 5188 //舵机中心值   5422  5400  5166
//舵机中值越大越往右偏
#define DJ_DIFF 1250   //最大打角与中心值的差值  600 350  1200  
extern int direct_kp_array[];//舵机转向的P值，在my_cfg.c文件中改
extern uint8 direct_Kd;//转向D的值，在my_cfg.c文件中改
extern float kp_val;//所有的p都会加上这个数
extern uint8 max_distance_array[];
extern uint8 max_speed_distance_array[];
extern uint8 Specia_type_speed[];
/*电机参数 引脚可在PORT_CFG.h中更改*/
#define MOTOR_FTM   FTM0//定时器0控制
#define MOTOR1_PWM  FTM_CH5//通道5 PTD5
#define MOTOR2_PWM  FTM_CH6//通道6 PTD6
#define MOTOR3_PWM  FTM_CH2//通道4 PTA7
#define MOTOR4_PWM  FTM_CH4//通道3 PTA5
#define MOTOR7_PWM  FTM_CH7//通道7 PTD7
extern uint8 zhidao_speed;//直道速度，可用小彩屏调节
extern uint8 L_zhidao_speed;
extern uint8 CD_speed;	  //全局速度，可用小彩屏调节
extern float speed_P,speed_D;//电机的PID参数，在my_cfg.c文件中改
extern float speed_I;

extern uint8 speed_table[][2];//小彩屏调参时候的默认三个速度，在my_cfg.c文件中改
extern float diff_speed;//差速因子



/*编码器对应引脚，用的正交解码，可以在PORT_cfg.h中改*/

//      模块通道    端口          可选范围              建议
//#define FTM1_CH0_PIN    PTA8       //PTA8、PTA12、PTB0
//#define FTM1_CH1_PIN    PTA9       //PTA9、PTA13、PTB1
//#define FTM2_CH0_PIN    PTA10       //PTA10、PTB18
//#define FTM2_CH1_PIN    PTA11       //PTA11、PTB19

#endif


