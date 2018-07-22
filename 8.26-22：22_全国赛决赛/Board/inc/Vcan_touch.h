#ifndef _VCAN_TOUCH_H_
#define _VCAN_TOUCH_H_


//以下是硬件配置
#define TOUCH_SPI         SPI1
#define TOUCH_CS          SPI_PCS0

#define TOUCH_INT_PTXn    PTB9

#define _CS_PTXn(spi)        spi##_PCS0
#define CS_PTXn(spi)        _CS_PTXn(spi)
#define TOUCH_CS_PTXn       CS_PTXn(TOUCH_SPI)

#define TOUCH_YCh_12bit      0x90
#define TOUCH_XCh_12bit      0xd0

#define TOUCH_YCh_8bit       0x98
#define TOUCH_XCh_8bit       0xd8


typedef int touch_call_back_func(uint8 * buff,uint32 len);

extern void touch_init(void);       //触摸屏初始化

extern int touch_calibrate(void);
extern int touch_get_point(Site_t *site);

extern int touch_load(touch_call_back_func);    //加载校验参数
extern int touch_save(touch_call_back_func);    //保存校验参数

extern uint16 xpt2046_read_ch(uint8_t _ucCh);
extern void   xpt2046_get_xy(Site_t * site);
extern uint8  xpt2046_read(Site_t * site );







#endif
