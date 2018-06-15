#ifndef _infrared_
#define _infrared_


#include "common.h"
#include "bsp_my_oled.h"  


//°´¼üID¶¨Òå
#define BT_UP_PRESS  (1)
#define BT_DN_PRESS  (2)
#define BT_LE_PRESS  (3)
#define BT_RI_PRESS  (4)
#define BT_OK_PRESS  (5)

extern uint8 irkey_value;
extern uint8 user_code;
extern void ir_decode(void);
extern uint8 GetIRKey(void);
extern void SetIRKey(unsigned char key);

uint16 get_high_time(void);
uint16 get_low_time(void);
void code_mapping(void);
void ir_init(void);


#endif  /*_infrared_*/