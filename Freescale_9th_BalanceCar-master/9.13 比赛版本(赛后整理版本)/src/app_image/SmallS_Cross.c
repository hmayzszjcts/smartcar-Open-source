#include"include.h"
/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名       ：SmallS_Cross.c
 * 描述         ：小S与十字的判断
 * 实验平台     ：凌立印象开发板
 * 库版本       ：基于野火库
 * 嵌入系统     ：
 * 作者         ：xuxu
**********************************************************************************/ 



//十字标志位
unsigned char Cross_flag=0;
#define Cross_THPoint 130
//小S
unsigned char smallS_flag=0;




/*************************************************************************
*  函数名称：Store_History
*  功能说明：队列记录历史值
*  参数说明：无
*  函数返回：无
*  修改时间：2012-2-14    已测试
*************************************************************************/
void Store_History()
{
    #define His_data_num 10
    for(int i=His_data_num;i>0;i--)
    {
        His_centre[i] = His_centre[i-1];
        His_black_L[i] = His_black_L[i-1];
        His_black_R[i] = His_black_R[i-1];
        His_halfwidth[i] = His_halfwidth[i-1];
        His_Left_flag[i] = His_Left_flag[i-1];
        His_Right_flag[i] = His_Right_flag[i-1];
    }
    His_centre[0] = centre_oth[H];
    His_black_L[0] = black_L_oth[H];
    His_black_R[0] = black_R_oth[H];
    His_halfwidth[0] = halfwidth_oth[H];
    His_Left_flag[0] = getLeft_flag_oth[H];
    His_Right_flag[0] = getRight_flag_oth[H];
}



/*************************************************************************
*  函数名称：Cross_judge
*  功能说明：判断有无十字
*  参数说明：无
*  函数返回：无
*  修改时间：2012-2-14    已测试
*************************************************************************/
void Cross_judge()
{
    Cross_flag = 0;
    for(int i=H+20;i>H-20;i--)
    {
        if(sum_OneRow(i)==0)
        {
            Cross_flag = 1;
            black_centre = 160;
            break;
        }
    }
}
//计算一行40个数的总和
int sum_OneRow(unsigned int hang)
{
    int sum=0;
    for(int i=0;i<40;i++)
        sum+=Image_fire[hang][i];
    return sum;
}
//计算一列240个数的总和
int sum_OneCol(unsigned int lie)
{
    int sum=0;
    for(int i=0;i<240;i++)
        sum+=Image_fire[i][lie];
    return sum;
}




/*************************************************************************
*  函数名称：Judge_smallS
*  功能说明：判断小S，主要用于直冲过去，直到国赛还没有用。。。
*  参数说明：无
*  函数返回：无
*  修改时间：2012-2-14    no测试
*************************************************************************/
unsigned char Judge_smallS()
{
    return 0;
}