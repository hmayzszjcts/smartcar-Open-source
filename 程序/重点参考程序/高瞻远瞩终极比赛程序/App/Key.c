#include "common.h"
#include "include.h"
int key_5xiang[5]={0,0,0,0,0};
int keyup_,keydown_,keyenter_,keyleft_,keyright_;//2015.6.23

void key_Init(void)
{
  //gpio_init (PTA17, GPO, 1u);// LED

  gpio_init(PTE0, GPI, HIGH);  //left
  gpio_init(PTE1, GPI, HIGH);  //enter
  gpio_init(PTE3, GPI, HIGH);  //UP
  gpio_init(PTD14, GPI, HIGH);  //DOWN
  gpio_init(PTD15, GPI, HIGH);  //RIGHT
}

/*int key_status(void)
{
  
  key_5xiang[0]=gpio_get(PORTB, 2);
  key_5xiang[1]=gpio_get(PORTA, 26);
  key_5xiang[2]=gpio_get(PORTA, 29);
  key_5xiang[3]=gpio_get(PORTA, 28);
  key_5xiang[4]=gpio_get(PORTA, 27);
    if(key_5xiang[0]==0)
      return 1;
    else if(key_5xiang[1]==0)
      return 2;
    else if(key_5xiang[2]==0)
      return 3;
    else if(key_5xiang[3]==0)
      return 4;
    else if(key_5xiang[4]==0)
      return 5;
    else
      return 0;
}*/