#ifndef _VCAN_SD_APP_H_
#define _VCAN_SD_APP_H_

#define UI_PIC_MAX_W    320    //定义ui最大的宽度


#define USE_UI_PIC_NULL     0
#define USE_UI_PIC_SD       1
#define USE_UI_PIC_FLASH    2
#define USE_UI_PIC          USE_UI_PIC_FLASH   //ui_pic 是否使用 sd卡，不使用 sd 卡，则用 flash

typedef enum
{
    UI_NUM0,
    UI_NUM1,
    UI_NUM2,
    UI_NUM3,
    UI_NUM4,
    UI_NUM5,
    UI_NUM6,
    UI_NUM7,
    UI_NUM8,
    UI_NUM9,
    UI_PIC_BG,

    UI_NUM_MAX,

}ui_pic_num_t;


extern uint8 ui_pic_init(void);
extern void  LCD_ui_pic(uint16 num,Site_t site);


//下面函数 用于 计时器界面
void LCD_ui_time_init(void);
void LCD_ui_time(uint16 ms);





#endif      //_VCAN_SD_APP_H_