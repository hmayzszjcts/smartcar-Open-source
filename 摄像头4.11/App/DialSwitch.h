#ifndef __DIALSWITCH_H__
#define __DIALSWITCH_H__

#include "common.h"
#include "include.h"

#define OpenDialSwitch 1  //启用拨码开关功能

#define  DialSwitch_1    gpio_get(PTD9 )
#define  DialSwitch_2    gpio_get(PTD10)
#define  DialSwitch_3    gpio_get(PTD8 )
#define  DialSwitch_4    gpio_get(PTD11)
#define  DialSwitch_5    gpio_get(PTD12)
#define  DialSwitch_6    gpio_get(PTD13)
#define  DialSwitch_7    gpio_get(PTD14)

extern void DialSwitchInit(void);


#endif