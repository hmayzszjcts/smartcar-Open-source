/*
* @file         OLED.h
  * @brief      oled相关函数实现
  * @author     xiao_fang
  * @version    v1.0
  * @date       2017-5-11
*/
#ifndef __OLED_H__
#define __OLED_H__

void Dly_ms(uint16_t ms);
void OLED_WrDat(uint8_t data);
void OLED_Wr6Dat(uint8_t data);
void OLED_Wr4Dat(uint8_t data);
void OLED_WrCmd(uint8_t cmd);
void OLED_Set_4Pos(uint8_t x, uint8_t y);
void OLED_Set_Pos(uint8_t x, uint8_t y);
void OLED_Fill(uint8_t bmp_data);
void OLED_CLS(void);
void OLED_DLY_ms(uint16_t ms);
void OLED_Init(void);
void OLED_PutPixel(uint8_t x,uint8_t y);
void OLED_Rectangle(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t gif);
void OLED_P6x8Str(uint8_t x,uint8_t y,uint8_t ch[]);
void OLED_P8x16Str(uint8_t x,uint8_t y,uint8_t ch[]);
void OLED_HEXACSII(uint16_t hex,uint8_t* Print);
void OLED_Print_Num(uint8_t x, uint8_t y, uint16_t num);
void OLED_Print_Str(uint8_t x, uint8_t y, uint8_t ch[]);
void Draw_BMP(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint8_t bmp[]);
void Draw_team_logo(void);
void draw_sensor_value(uint16 * sensor_value);
#endif
