#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#include  "common.h"

/*
 * Include 用户自定义的头文件
 */
#include  "MKL_BME.h"           //位操作
#include  "MKL_wdog.h"          //看门狗
#include  "MKL_gpio.h"          //IO口操作
#include  "MKL_uart.h"          //串口
#include  "MKL_SysTick.h"       //滴答定时器
#include  "MKL_lptmr.h"         //低功耗定时器(延时、脉冲计数、定时、计时)
#include  "MKL_i2c.h"           //I2C
#include  "MKL_spi.h"           //SPI
#include  "MKL_tpm.h"           //TPM（类似K60的 FTM ，pwm、脉冲计数）
#include  "MKL_pit.h"           //PIT
#include  "MKL_adc.h"           //ADC
#include  "MKL_dac.h"           //DAC
#include  "MKL_dma.h"           //DMA
#include  "MKL_FLASH.h"         //FLASH


#include  "VCAN_LED.H"          //LED
#include  "VCAN_NRF24L0.h"      //无线模块NRF24L01+
#include "Servo_motor.h"        //舵机相关函数
#include "Senser.h"             //传感器采集相关函数
#include "24C02.h"              //24C02相关函数实现
#include "Switch.h"             //拨码开关相关函数实现
#include "Motor.h"              //电机相关函数实现
#include "Encoder.h"            //编码器测速相关函数实现
#include "Servo_motor_control.h"  //舵机控制相关函数实现
#include "MKL_Scope.h"          //山外虚拟示波器例程
#include "Motor_control.h"
#include "OLED.h"
#include "Fuzzy.h"
#include "Reed.h"
#include "Gyroscope.h"
#include "IIC_sim.h"
#endif  //__INCLUDE_H__
