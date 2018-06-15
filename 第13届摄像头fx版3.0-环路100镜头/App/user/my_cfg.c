#include "my_cfg.h"
//                                    长直到  短直到  斜入直到  十字      障碍    小S湾    中S弯  大S弯    小弯    中弯    大弯   特大弯  大转向   环路
///*转向Kp*/   int direct_kp_array[]={    -6 ,   -5   ,    3   ,  8    ,  7     ,   8   ,   12   ,  14  ,  14    ,  15   ,  18  ,  18   ,    20 ,      80  };
///*转向Kp基准*/ float kp_val=6;//所有的p都会加上这个数 6 8
//                                    长直到  短直到  斜入直到  十字      障碍    小S湾    中S弯  大S弯    小弯    中弯    大弯   特大弯  大转向   环路
///*转向Kp*/   int direct_kp_array[]={    -6 ,   -4   ,    3   ,  3    ,  3     ,   6   ,   8   ,  12  ,  10    ,  12   ,  12  ,  12   ,    12 ,      80  };
/*转向Kp*/   int direct_kp_array[]={    -6 ,   -4   ,    13   ,  13    ,  13     ,   20   ,   20   ,  25  ,  25    ,  25   ,  25  ,  25   ,    25 ,      90  };
/*转向Kp基准*/ float kp_val=14;//所有的p都会加上这个数 12
/*转向Kd */  uint8 direct_Kd = 45;  // 45

///*电机*/   float speed_P=50  ,speed_I=8.2, speed_D=20   ;  
//float speed_P=80  ,speed_I=10, speed_D=10;
float speed_P=80  ,speed_I=5.2, speed_D=10;
/***********************小彩屏的默认最大速度，最小速度***************************************/
//uint8 zhidao_speed=160;	//直道速度（最大速度） 160
//uint8 L_zhidao_speed=180;//  180
//uint8 CD_speed=105;	//全局速度最慢  110


uint8 zhidao_speed=110;	//直道速度（最大速度）   180
uint8 L_zhidao_speed=110;//  150 
uint8 CD_speed=110;	//全局速度最慢  100


//                       max      min 
uint8 speed_table[][2]={{105  ,   95}, //    //彩屏上三个默认速度
                        {100  ,   90},
                        {85  ,   85},};
float diff_speed=53;//差速因子53  57
uint8 diff_valid_line=45;//4545
uint8 MIX_distance   =15;//最近的距离  15 

int gl_var_1=0;//备用的变量，可以在彩屏set img中调数
int gl_var_2=0;//备用的变量，可以在彩屏set img中调数
int gl_var_3=0;//备用的变量，可以在彩屏set img中调数
int gl_var_4=0;//备用的变量，可以在彩屏set img中调数
/*图像的使用距离，计算误差用的*/
//////                          60,70,80,90,100,>110
//uint8 max_distance_array[]={30,32,37,40, 45, 45};
////                          60,70,80,90,100,>110
////uint8 max_distance_array[]={45,45,45,45, 45, 45};
//uint8 max_speed_distance_array[]={45,45,45,45, 45, 45};
/*****************************************************************************************
全局舵机一个P
两个速度 85 100  很稳
（在不跳轮的前提下）
45 10
**************************************/











