#ifndef _SMALLS_CROSS_H
#define _SMALLS_CROSS_H


extern unsigned char Cross_flag;
extern unsigned char smallS_flag;

void Store_History();
void Cross_judge();
int sum_OneRow(unsigned int hang);
int sum_OneCol(unsigned int lie);
unsigned char Judge_smallS();

#endif