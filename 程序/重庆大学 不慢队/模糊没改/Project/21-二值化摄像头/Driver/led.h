/*!
 * @file       LED.h
 * @brief      LED常用函数头文件
 */

#ifndef _LED_H_
#define _LED_H_


#define LED_RED    PTC18
#define LED_GREEN PTA19
#define LED_BLUE  PTE6

#define LED_GREEN_ON    gpio_set(LED_GREEN,0)
#define LED_GREEN_OFF   gpio_set(LED_GREEN,1)
#define LED_GREEN_TURN  gpio_turn(LED_GREEN)

#define LED_BLUE_ON     gpio_set(LED_BLUE,0)
#define LED_BLUE_OFF    gpio_set(LED_BLUE,1)
#define LED_BLUE_TURN   gpio_turn(LED_BLUE)

#define LED_RED_ON      gpio_set(LED_RED,0)
#define LED_RED_OFF     gpio_set(LED_RED,1)
#define LED_RED_TURN    gpio_turn(LED_RED)

void led_init();
void water_lights(); 
void led_flash();

#endif /* _LED_H_ */