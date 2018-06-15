#ifndef __MY_OLED_H
#define __MY_OLED_H
#include "common.h"


void oled_fill(char uc_data);
void oled_display_6x8char(uint8 uc_posx, uint8 uc_posy, char uc_data);
void oled_display_6x8str(uint8 uc_posx, uint8 uc_posy, char uc_dataStr[]);
void oled_print_char(uint8 uc_posx, uint8 uc_posy, char c_data);
void oled_print_short(uint8 uc_posx, uint8 uc_posy, int16 s_data);
void oled_display_16x8char(uint8 uc_posx, uint8 uc_posy, char uc_data);
void oled_display_16x8char_hl(uint8 uc_posx, uint8 uc_posy, char uc_data);
void oled_display_16x8str(uint8 uc_posx, uint8 uc_posy, char uc_dataStr[]);
void oled_display_16x8str_hl(uint8 uc_posx, uint8 uc_posy, char uc_dataStr[]);
void oled_print_16x8char(uint8 uc_posx, uint8 uc_posy, int8 c_data);
void oled_print_16x8short(uint8 uc_posx, uint8 uc_posy, int16 s_data);
void oled_print_16x8short_hl(uint8 uc_posx, uint8 uc_posy, int16 s_data, int8 bits_num);
void oled_display_6x8char_hl(uint8 uc_posx, uint8 uc_posy, char uc_data);
void oled_display_6x8str_hl(uint8 uc_posx, uint8 uc_posy, char uc_dataStr[]);
void oled_print_char_hl(uint8 uc_posx, uint8 uc_posy, int8 c_data);
void oled_print_short_hl(uint8 uc_posx, uint8 uc_posy, int16 s_data, int8 bits_num);
#endif

