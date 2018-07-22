/*!
 * @file       LED.h
 * @brief      LED常用函数头文件
 */

#ifndef _BUTTON_H_
#define _BUTTON_H_

#define BT_CANCEL   PTC6
#define BT_RIGHT     PTC8
#define BT_UP       PTB22
#define BT_DOWN     PTB23
#define BT_YES      PTB20
#define BT_LEFT    PTB21

#define SW4      PTA4
#define SW3      PTA5
#define SW2      PTA13
#define SW1      PTA17

#define SW1_IN   gpio_get(SW1)
#define SW2_IN   gpio_get(SW2)
#define SW3_IN   gpio_get(SW3)
#define SW4_IN   gpio_get(SW4)

#define BEEP      PTC7
#define BEEP_ON   gpio_set (PTC7,1)
#define BEEP_OFF  gpio_set (PTC7,0)


#define BT_CANCEL_IN gpio_get(BT_CANCEL)
#define BT_LEFT_IN   gpio_get(BT_LEFT)
#define BT_UP_IN     gpio_get(BT_UP)
#define BT_DOWN_IN   gpio_get(BT_DOWN)
#define BT_YES_IN    gpio_get(BT_YES)
#define BT_RIGHT_IN  gpio_get(BT_RIGHT)

void button_init();
void switch_init();

#endif /* _LED_H_ */