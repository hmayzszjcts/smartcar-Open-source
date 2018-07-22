#include "common.h"
#include "include.h"



#if  OpenDialSwitch


void DialSwitchInit(void)
{
  //往上波了是0，不波的时候是1
     gpio_init(PTD9, GPI,0);//1
     gpio_init(PTD10,GPI,0);//2
     gpio_init(PTD8,GPI,0);//3
     gpio_init(PTD11,GPI,0);//4
     
     gpio_init(PTD12,GPI,0);//5
     gpio_init(PTD13,GPI,0);//6
     gpio_init(PTD14,GPI,0);//7   
}

#endif