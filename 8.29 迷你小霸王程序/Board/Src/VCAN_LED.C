 /*!
  *     COPYRIGHT NOTICE
  *     Copyright (c) 2013,山外科技
  *     All rights reserved.
  *     技术讨论：山外论坛 http://www.vcan123.com
  *
  *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
  *     修改内容时必须保留山外科技的版权声明。
  *
  * @file       VCAN_LED.c
  * @brief      led驱动函数实现
  * @author     山外科技
  * @version    v5.0
  * @date       2013-07-9
  */

/*
 * 包含头文件
 */
#include "common.h"
#include "MKL_port.h"
#include "MKL_gpio.h"
#include "VCAN_LED.H"


/*
 * 定义LED 编号对应的管脚
 */
PTXn_e LED_PTxn[LED_MAX] = {PTC3,PTB0};


/*!
 *  @brief      初始化LED端口
 *  @param      LED_e    LED编号
 *  @since      v5.0
 *  Sample usage:       led_init (LED0);    //初始化 LED0
 */
void    led_init(LED_e ledn)
{
    if(ledn < LED_MAX)
    {
        gpio_init(LED_PTxn[ledn],GPO,LED_OFF);
    }
    else
    {
        ledn = LED_MAX;
        while(ledn--)
        {
            gpio_init(LED_PTxn[ledn],GPO,LED_OFF);
        }

    }
}



/*!
 *  @brief      设置LED灯亮灭
 *  @param      LED_e           LED编号（LED0、LED1、LED2）
 *  @param      LED_status      LED亮灭状态（LED_ON、LED_OFF）
 *  @since      v5.2
 *  Sample usage:       led (LED0,LED_ON);    //点亮 LED0
 */
void    led(LED_e ledn,LED_status status)
{
    if(ledn < LED_MAX)
    {
        gpio_set(LED_PTxn[ledn],status);
    }
    else
    {
        ledn = LED_MAX;
        while(ledn--)
        {
            gpio_set(LED_PTxn[ledn],status);
        }
    }
}

/*!
 *  @brief      设置LED灯亮灭反转
 *  @param      LED_e           LED编号（LED0、LED1、LED2、LED3）
 *  @since      v5.2
 *  Sample usage:       led_turn (LED0);    // LED0灯亮灭反转
 */
void led_turn(LED_e ledn)
{
    if(ledn < LED_MAX)
    {
        gpio_turn(LED_PTxn[ledn]);
    }
    else
    {
        ledn = LED_MAX;
        while(ledn--)
        {
            gpio_turn(LED_PTxn[ledn]);
        }
    }
}
/*!
 *  @brief      启动时闪烁LED灯
 *  @since      v5.0
 *  @author     xiao_fang
 */
void lightwave(void)
{
    led_init(LED0);
    led_init(LED1);
    led(LED0,LED_ON);
    led(LED1,LED_ON);
    systick_delay_ms(50);
    led(LED0,LED_OFF);
    led(LED1,LED_OFF);    
}