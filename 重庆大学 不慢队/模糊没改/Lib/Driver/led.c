/*!
 * @file       LED.C
 * @brief      LED常用函数

 */
#include "common.h"
#include "include.h"
 /*
    函数作用 :板载led初始化
    注：核心板上有三个可编程LED，端口分别是 PTA19,PTE6,PTC18,低电平点亮
      端口定义在 led.h
 */  
void led_init()
{
  gpio_init (LED_RED, GPO,1);
  gpio_init (LED_GREEN, GPO,1); 
  gpio_init (LED_BLUE, GPO,1);

}

void water_lights()
{
  LED_GREEN_ON;
  LED_RED_OFF;
  LED_BLUE_OFF;
  DELAY_MS(150);
  LED_GREEN_OFF;
  LED_RED_ON;
  LED_BLUE_OFF;
  DELAY_MS(150);
  LED_GREEN_OFF;
  LED_RED_OFF;
  LED_BLUE_ON;
  DELAY_MS(150);
}
void led_flash()
{
  LED_GREEN_ON;
  LED_RED_ON;
  LED_BLUE_ON;
  DELAY_MS(500);
  LED_GREEN_OFF;
  LED_RED_OFF;
  LED_BLUE_OFF;
  DELAY_MS(500);
}