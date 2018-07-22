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
#include  "MKL_spi.h"           //SPI
#include  "MKL_tpm.h"           //TPM（类似K60的 FTM ，pwm、脉冲计数）
#include  "MKL_pit.h"           //PIT
#include  "MKL_adc.h"           //ADC
#include  "MKL_FLASH.h"         //FLASH



#include "CCD.h"
#include "common.h"
#include "oled.h"
#include "init.h"
#include "i2c.h"
#include "debug.h"
#include "control.h"
#include "SD_Card.h"
#include "CCD.h"
#include "Search.h"
#include "fuzzy.h"
#include "eeprom.h"




#endif  //__INCLUDE_H__
