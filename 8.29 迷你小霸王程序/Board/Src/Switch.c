/*
* @file         Switch.c
  * @brief      拨码开关相关函数实现
  * @author     xiao_fang
  * @version    v1.0
  * @date       2017-3-13
*/
#include "common.h"
#include "Switch.h"
#include "MKL_port.h"
#include "MKL_gpio.h"
#include "Motor.h"
#include "Buzzer.h"

/*
 *  @brief      初始化拨码开关
 *  @since      v2.0
*/
void switch_button_init(void)
{
  gpio_init  (PTD6, GPI, 0);
  gpio_init  (PTD7, GPI, 0);
  gpio_init  (PTD4, GPI, 0);
  gpio_init  (PTD5, GPI, 0);
  gpio_init  (PTD0, GPI, 0);
  gpio_init  (PTD1, GPI, 0);
  gpio_init  (PTC16, GPI,0);
  gpio_init  (PTC13,GPI, 0);
  
  port_init(PTD6 , PULLUP | ALT1 | PF ); //上拉 无源滤波 复用为GPIO SW1
  port_init(PTD7 , PULLUP | ALT1 | PF ); //上拉 无源滤波 复用为GPIO SW2
  port_init(PTD4 , PULLUP | ALT1 | PF ); //上拉 无源滤波 复用为GPIO SW3
  port_init(PTD5 , PULLUP | ALT1 | PF ); //上拉 无源滤波 复用为GPIO SW4
  port_init(PTD0 , PULLUP | ALT1 | PF ); //上拉 无源滤波 复用为GPIO SW5
  port_init(PTD1 , PULLUP | ALT1 | PF ); //上拉 无源滤波 复用为GPIO SW6
  port_init(PTC16 ,PULLUP | ALT1 | PF ); //上拉 无源滤波 复用为GPIO SW7
  port_init(PTC13, PULLUP | ALT1 | PF ); //上拉 无源滤波 复用为GPIO SW8
  
  gpio_init  (PTB19, GPI, 0);
  gpio_init  (PTB20, GPI, 0);
  gpio_init  (PTB21, GPI, 0);
  
  port_init(PTB19 , PULLUP | ALT1 | PF ); //上拉 无源滤波 复用为GPIO SW1
  port_init(PTB20 , PULLUP | ALT1 | PF ); //上拉 无源滤波 复用为GPIO SW2
  port_init(PTB21 , PULLUP | ALT1 | PF ); //上拉 无源滤波 复用为GPIO SW3
}
/*
 *  @brief      读取下层拨码开关状态
 *  @since      v1.0
*/
uint8 switch_read(void)
{
  uint8 data = 0x00;
  data = (GPIO_GET(PTD6)) + (GPIO_GET(PTD7)<<1) + (GPIO_GET(PTD4)<<2) + (GPIO_GET(PTD5)<<3) + (GPIO_GET(PTD0)<<4) + (GPIO_GET(PTD1)<<5) + (GPIO_GET(PTC16)<<6) + (GPIO_GET(PTC13)<<7);
  return data;
}

/*
 *  @brief      读取按键状态
 *  @since      v1.0
*/
uint8 button_read(void)
{
  uint8 data = 0x00;
  data = (GPIO_GET(PTB19)) + (GPIO_GET(PTB20)<<1) + (GPIO_GET(PTB21)<<2);
  return data;
}

