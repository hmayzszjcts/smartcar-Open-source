#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#include  "common.h"

/*
 * Include 用户自定义的头文件
 */
#include  "MK60_wdog.h"
#include  "MK60_gpio.h"     //IO口操作
#include  "MK60_uart.h"     //串口
#include  "MK60_SysTick.h"
#include  "MK60_lptmr.h"    //低功耗定时器(延时)
#include  "MK60_i2c.h"      //I2C
#include  "MK60_spi.h"      //SPI
#include  "MK60_ftm.h"      //FTM
#include  "MK60_pit.h"      //PIT
#include  "MK60_FLASH.h"    //FLASH
#include  "MK60_adc.h"    //FLASH

#include  "oled.h" 
#include  "I2C.h"
#include  "init.h" 
#include  "debug.h" 
#include  "EM.h" 
#include  "control.h"
//#include  "SD_Card.h"
#include "math.h"
#include "fuzzy.h"
#include "Search.h"
   
   

#define LED_GREEN_ON    gpio_set(PTE10,0)
#define LED_GREEN_OFF   gpio_set(PTE10,1)
#define LED_GREEN_TURN  gpio_turn(PTE10)

#define LED_RED_ON      gpio_set(PTE11,0)
#define LED_RED_OFF     gpio_set(PTE11,1)
#define LED_RED_TURN    gpio_turn(PTE11)

#define LED_BLUE_ON     gpio_set(PTE12,0)
#define LED_BLUE_OFF    gpio_set(PTE12,1)
#define LED_BLUE_TURN   gpio_turn(PTE12)

#endif  //__INCLUDE_H__
