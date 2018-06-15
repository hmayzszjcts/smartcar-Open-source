#ifndef _PROCESS_H
#define _PROCESS_H


extern int black_centre;
extern int H;
extern unsigned char Multiple_line_enable;

//多行
extern int centre[Process_Hang+2];
extern int black_L[Process_Hang+2];
extern int black_R[Process_Hang+2];
extern int halfwidth[Process_Hang+2];
extern unsigned char getLeft_flag[Process_Hang+2];
extern unsigned char getRight_flag[Process_Hang+2];

//单行
extern int centre_oth[Process_Hang+2];
extern int black_L_oth[Process_Hang+2];
extern int black_R_oth[Process_Hang+2];
extern int halfwidth_oth[Process_Hang+2];
extern unsigned char getLeft_flag_oth[Process_Hang+2];
extern unsigned char getRight_flag_oth[Process_Hang+2];

//加权平均
extern int valid_farthest;
extern int near_hang;
extern int far_hang;
extern int _halfwidth_min[Process_Hang];
extern int _halfwidth_max[Process_Hang];

//历史值
extern int His_centre[30];
extern int His_black_L[30];
extern int His_black_R[30];
extern int His_halfwidth[30];
extern unsigned char His_Left_flag[30];
extern unsigned char His_Right_flag[30];

//函数
void Process_Image();
void get_ave_centre();
int get_black_line(unsigned char *ImageData_in,int hang);
int get_black_line_oth(unsigned char *ImageData_in,int hang);
int find_valid_farthest();


#endif


