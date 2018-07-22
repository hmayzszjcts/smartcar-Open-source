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
#include  "MK60_rtc.h"      //RTC
#include  "MK60_adc.h"      //ADC
#include  "MK60_dac.h"      //DAC
#include  "MK60_dma.h"      //DMA
#include  "MK60_FLASH.h"    //FLASH

#include  "stdio.h"
#include  "led.h"
#include  "button.h"
#include  "oled12864.h"
#include  "camera.h"
#include  "eeprom.h"
#include  "sdcard.h"
#include  "function.h"
#include  "I2C.h"
#include  "control.h"
#include  "debug.h"
#include  "search.h"
#include  "fuzzy.h"

#endif  //__INCLUDE_H__
