#ifndef _VCAN_KEY_EVNET_H_
#define _VCAN_KEY_EVNET_H_

#include "common.h"
#include "VCAN_key.h"



extern void key_event_init();                              //初始化按键消息
extern void deal_key_event(void);                          //处理按键消息（自动切换选中的数字，自动对数字进行加减）



#endif  //_VCAN_KEY_EVNET_H_