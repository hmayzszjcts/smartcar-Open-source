/*!
 * @file       LED.C
 * @brief      LED常用函数

 */
#include "common.h"
#include "include.h"
#include "button.h"
 /*
    函数作用 :板载led初始化
    注：核心板上有三个可编程LED，端口分别是 PTA19,PTE6,PTC18,低电平点亮
      端口定义在 led.h
 */  

void button_init()
{
  gpio_init (BT_CANCEL, GPI,1);
  port_init_NoALT (BT_CANCEL, PULLUP );  
  gpio_init (BT_LEFT, GPI,1);
  port_init_NoALT (BT_LEFT, PULLUP ); 
  gpio_init (BT_UP, GPI,1);
  port_init_NoALT (BT_UP, PULLUP ); 
  gpio_init (BT_DOWN, GPI,1);
  port_init_NoALT (BT_DOWN, PULLUP );
  gpio_init (BT_YES, GPI,1);
  port_init_NoALT (BT_YES, PULLUP ); 
  gpio_init (BT_RIGHT, GPI,1);
  port_init_NoALT (BT_RIGHT, PULLUP ); 
  
  
  gpio_init(BEEP,GPO,0);

}

void switch_init()
{
  gpio_init (SW1, GPI,1);
  port_init_NoALT (SW1, PULLUP );  
  gpio_init (SW2, GPI,1);
  port_init_NoALT (SW2, PULLUP ); 
  gpio_init (SW3, GPI,1);
  port_init_NoALT (SW3, PULLUP ); 
  gpio_init (SW4, GPI,1);
  port_init_NoALT (SW4, PULLUP );

}


