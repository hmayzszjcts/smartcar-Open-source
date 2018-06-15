#ifndef MY_UI_H_
#define MY_UI_H_
#define STR_BUF_LEN 30
typedef enum
{
    MAIN_UI  =   0,         //按键按下时对应电平
    SET_SPEED_UI    =   1,         //按键弹起时对应电平
    IMAGE_UI,               //长按按键(用于定时按键扫描)
    SET_IMG_UI,
    OPEN_IMG_UI,
} UI_STATUS_e;

extern void renew_UI();
extern void show_run_info();
#endif

