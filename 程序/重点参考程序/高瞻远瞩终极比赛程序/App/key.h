
#ifndef _KEY_H
#define _KEY_H

#include "common.h"
#include "include.h"
//#define LED_ON()        gpio_set (PORTA, 17,0u);
//#define LED_OFF()       gpio_set (PORTA, 17,1u);

extern int keyup_,keydown_,keyenter_,keyleft_,keyright_;//2015.6.23
/*
  gpio_init(PTE0, GPI, HIGH);  //left
  gpio_init(PTE1, GPI, HIGH);  //enter
  gpio_init(PTE3, GPI, HIGH);  //UP
  gpio_init(PTD14, GPI, HIGH);  //DOWN
  gpio_init(PTD15, GPI, HIGH);  //RIGHT
*/
#define keyUp gpio_get(PTE0);  //up
#define keyDown gpio_get(PTD15); //down
#define keyEnter gpio_get(PTE1); //enter
#define keyLeft gpio_get(PTE3); //left
#define keyRight gpio_get(PTD14); //right

/*
  gpio_init (PORTA,13, 0, 1);  //up
  gpio_init (PORTA,25, 0, 1);  //left
  gpio_init (PORTA,17, 0, 1);  //down
  gpio_init (PORTA,24, 0, 1);  //right
  gpio_init (PORTA,16, 0, 1);  //enter*/



//#define key1board1 PTB;

//extern int key2[5]={0,0,0,0,0};

void key_Init(void);
int key_status(void);
#endif
