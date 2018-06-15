
#ifndef _headfile_h
#define _headfile_h


#include "common.h"
#include "KEA128_port_cfg.h"
#include "stdio.h"

//--------函数库--------
#include "KEA128_ftm.h"
#include "KEA128_pit.h"
#include "KEA128_gpio.h"
#include "KEA128_port.h"
#include "KEA128_adc.h"
#include "KEA128_irq.h"
#include "KEA128_uart.h"
#include "KEA128_kbi.h"
#include "KEA128_flash.h"
#include "KEA128_systick.h"
#include "KEA128_i2c.h"
#include "KEA128_spi.h"




#include "SEEKFREE_18TFT.h"
#include "SEEKFREE_FUN.h"
#include "SEEKFREE_IIC.h"
#include "SEEKFREE_L3G4200D.h"
#include "SEEKFREE_MMA8451.h"
#include "SEEKFREE_MPU6050.h"
#include "SEEKFREE_NRF24L01.h"
#include "SEEKFREE_OLED.h"
#include "SEEKFREE_TSL1401.h"


/*功能型GPIO宏定义*/ //一般数据保存区
#define ACC_Z           adc_once(ADC0_SE0, ADC_10bit)
#define ANGLE           adc_once(ADC0_SE1, ADC_10bit)
#define GYRO_Z          adc_once(ADC0_SE2, ADC_10bit)
#define GYRO_X          adc_once(ADC0_SE3, ADC_10bit)

#define L1              adc_once(ADC0_SE4, ADC_10bit)//14
#define L2              adc_once(ADC0_SE5, ADC_10bit)
#define L3              adc_once(ADC0_SE6, ADC_10bit)
#define L4              adc_once(ADC0_SE7, ADC_10bit)


//此处应修改  根据实际硬件  修改管脚 并在 CarBalanceSystemInit里初始化使用到的引脚
#define DOWN            gpio_get(I0) //DOWN
#define LIFE            gpio_get(I0)//LIFE 
#define RIGHT           gpio_get(I0) //Right
#define UP              gpio_get(I0) //UP
#define ENTER           gpio_get(I0) //ENTER


#define SW6             gpio_get(I0) //BM6
#define SW5             gpio_get(I0)//BM5
#define SW4             gpio_get(I0) //BM4
#define SW3             gpio_get(I0) //BM3
#define SW2             gpio_get(I0) //BM2
#define SW1             gpio_get(I0) //BM1



#define PWMMAX          2000//1800
#define PWMMIN          0//200

 
  
typedef struct { //PID结构体数据类型
  
  float Kp;
  float Ki;
  float Kd;
  float dt;
  float err;
  float err_k[10];
  float next_err;
  float last_err;
  float offset;
  float llast_err;
  float integral; 
  float out;
  float out_k[10];
 
}PID_Type_Def;

typedef struct { //陀螺仪 加速计结构体 数据类型
  
  int16 X_Value[3]; 
  int16 X_Final;//X最终值
  int16 X_Last;
  int16 X_Offset;//X静态偏移
  
  int16 Y_Value[3];
  int16 Y_Final;
  int16 Y_Last;
  int16 Y_Offset;
  
  int16 Z_Value[3];
  int16 Z_Final;
  int16 Z_Last;
  int16 Z_Offset;

}Acc_Type_Def,Gyro_Type_Def;

typedef struct { //角度结构体数据类型
  
  int16 PiontValue;//目标值
  int16 ActualValue[3];//实际值  123次
  int16 FinalValue; //最终值  由实际值滤波出来的
  int16 LastValue;

  
}Angle_Type_Def;

typedef struct { //PWM结构体数据类型
  
  
  int16 PiontValue;
  int16 ActualValue;
  int16 LiftValue;
  int16 RightValue;
  int16 Differential;
  
  
}PWM_Type_Def;

typedef struct { //速度结构体数据类型
  
  int16 PiontSpeed;       //速度目标
  int16 PiontSpeedMax;
  int16 PiontSpeedMin;
  int16 PiontDifferential;//差速目标
  int16 PiontLiftPulse;  //左脉宽目标值
  int16 PiontRightPulse; //右脉宽目标值
  
  int16 ActualSpeed[3];
  int16 ActualDifferential[3];
  int16 ActualLiftPulse[16];//左脉宽实际值
  int16 ActualRightPulse[16];//右脉宽实际值
  int16 ActualInstantaneous[3];//实际瞬时值
  int16 ActualAcceleration;//实际加速度
  
  uint32 ActualDistance;//实际路程
  uint32 FaultDistance;//故障路程
  int16  SlowDistance;//减速距离  一般 3000  3米
  int16  AccDistance;
  
}Speed_Type_Def;

typedef struct { //电感结构体数据类型
  
  int16 LeftValue[3];
  int16 LeftFinal;
  int16 LeftMax;
  int16 LeftMin;
  
  int16 RightValue[3];
  int16 RightFinal;
  int16 RightMax;
  int16 RightMin;
  
  int16 AndValues;//和值
  int16 LeftDerivative;//导数变化率
  int16 RightDerivative;

  
}Inductor_Type_Def;

typedef struct { //按键结构体数据类型
    
  int8 Up[10];
  int8 Up_Final;
  
  int8 Down[10];
  int8 Down_Final;
  
  int8 Life[10];
  int8 Life_Final;
  
  int8 Right[10];
  int8 Right_Final;
  
  int8 Enter[10];
  int8 Enter_Final;
  
}Button_Type_Def;

typedef struct { //拨码开关结构体数据类型
  
  int8 Bm1[3];
  int8 Bm1_Final;
  int8 Bm2[3];
  int8 Bm2_Final;
  int8 Bm3[3];
  int8 Bm3_Final;
  int8 Bm4[3];
  int8 Bm4_Final;
  int8 Bm5[3];
  int8 Bm5_Final;
  int8 Bm6[3];
  int8 Bm6_Final;
  
}Switch_Type_Def;

typedef struct { //标志位结构体数据类型
  
  uint8 Flag[10];
  
}RUN_Type_Def, Ramp_Type_Def, OUT_Type_Def, Stop_Type_Def, Curve_Type_Def;



#endif
