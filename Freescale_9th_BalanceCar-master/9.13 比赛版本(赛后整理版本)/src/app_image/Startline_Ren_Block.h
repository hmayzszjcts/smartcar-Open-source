#ifndef _STARTLINE_REN_BLOCK_H
#define _STARTLINE_REN_BLOCK_H


#define Image_fire2Image(row,col) ((Image_fire[row][col/8]>>(7-col%8))&0x01)


void Judge_startline_by_camera();
void Judge_Ren();
void Block_judge();
void get_black_line_correct(unsigned char *ImageData_in,int hang);

extern unsigned char Slope_Check_enable;
extern unsigned char Slope_flag;
extern int Slope_time;
extern int Slope_delaytime;
void Slope_Check();


#endif