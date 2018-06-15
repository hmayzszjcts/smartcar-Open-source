#include "include.h"


/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
* 文件名       ：Process.c
* 描述         ：最高端霸气的图像处理代码
* 实验平台     ：凌立印象开发板
* 库版本       ：基于野火库
* 嵌入系统     ：
* 作者         ：xuxu
**********************************************************************************/



//真实输出
int black_centre=CENTER;
int H=120;
unsigned char Multiple_line_enable=0;
//坐标
int centre[Process_Hang+2]={0};
int black_L[Process_Hang+2]={0};
int black_R[Process_Hang+2]={0};
int halfwidth[Process_Hang+2]={0};
unsigned char getLeft_flag[Process_Hang+2]={0};
unsigned char getRight_flag[Process_Hang+2]={0};
//坐标
int centre_oth[Process_Hang+2]={0};
int black_L_oth[Process_Hang+2]={0};
int black_R_oth[Process_Hang+2]={0};
int halfwidth_oth[Process_Hang+2]={0};
unsigned char getLeft_flag_oth[Process_Hang+2]={0};
unsigned char getRight_flag_oth[Process_Hang+2]={0};
//加权平均
int centre_ratio_step=0;
int valid_farthest=180;
int near_hang=180;
int far_hang=70;
int _halfwidth_min[Process_Hang]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,32,32,33,33,34,34,35,35,36,37,37,38,38,39,39,40,40,41,41,42,42,43,43,44,44,45,46,46,46,47,48,48,49,49,50,50,51,52,52,52,53,54,54,55,55,56,56,57,57,58,58,59,59,60,60,61,61,62,62,63,63,64,64,65,65,66,67,67,67,68,68,69,69,69,70,71,71,71,72,72,73,73,74,74,74,75,75,76,76,77,77,78,78,79,79,80,80,81,81,82,82,83,83,84,84,84,85,85,86,86,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int _halfwidth_max[Process_Hang]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,88,88,89,89,89,90,90,91,91,92,92,92,93,93,94,94,94,95,96,96,96,97,97,98,98,98,99,99,100,100,100,101,101,101,102,102,103,103,103,104,104,105,105,105,106,106,107,107,107,108,108,109,109,109,109,110,110,111,111,111,112,112,112,113,113,114,114,114,115,115,116,116,116,116,116,117,117,118,118,118,118,118,119,119,119,120,120,120,120,121,121,121,121,122,122,122,123,123,123,123,123,124,124,124,124,125,125,125,125,125,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
float centre_ratio[Process_Hang]={132.4,132.0,131.6,131.2,130.8,130.4,130.0,129.6,129.2,128.8,128.4,128.0,127.6,127.2,126.8,126.4,126.0,125.6,125.2,124.8,124.4,124.0,123.6,123.2,122.8,122.4,122.0,121.6,121.2,120.8,120.4,120.0,119.6,119.2,118.8,118.4,118.0,117.6,117.2,116.8,116.4,116.0,115.6,115.2,114.8,114.4,114.0,113.6,113.2,112.8,112.4,112.0,111.6,111.2,110.8,110.4,110.0,109.6,109.2,108.8,108.4,108.0,107.6,107.2,106.8,106.4,106.0,105.6,105.2,104.8,104.4,104.0,103.6,103.2,102.8,102.4,102.0,101.6,101.2,100.8,100.4,100.0,99.6,99.2,98.8,98.4,98.0,97.6,97.2,96.8,96.4,96.0,95.6,95.2,94.8,94.4,94.0,93.6,93.2,92.8,92.4,92.0,91.6,91.2,90.8,90.4,90.0,89.6,89.2,88.8,88.4,88.0,87.6,87.2,86.8,86.4,86.0,85.6,85.2,84.8,84.4,84.0,83.6,83.2,82.8,82.4,82.0,81.6,81.2,80.8,80.4,80.0,79.6,79.2,78.8,78.4,78.0,77.6,77.2,76.8,76.4,76.0,75.6,75.2,74.8,74.4,74.0,73.6,73.2,72.8,72.4,72.0,71.6,71.2,70.8,70.4,70.0,69.6,69.2,68.8,68.4,68.0,67.6,67.2,66.8,66.4,66.0,65.6,65.2,64.8,64.4,64.0,63.6,63.2,62.8,62.4,62.0,61.6,61.2,60.8,60.4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
float centre_ratio_1[Process_Hang]={1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,10.9,11.1,11.3,11.4,11.6,11.7,11.9,12.1,12.2,12.4,12.5,12.7,12.9,13.0,13.2,13.3,13.5,13.6,13.7,13.9,14.0,14.2,14.3,14.4,14.5,14.7,14.8,14.9,15.0,15.1,15.2,15.3,15.4,15.5,15.6,15.7,15.8,15.9,16.0,16.0,16.1,16.2,16.2,16.3,16.3,16.4,16.4,16.5,16.5,16.5,16.6,16.6,16.6,16.6,16.6,16.6,16.6,16.6,16.6,16.6,16.6,16.5,16.5,16.5,16.4,16.4,16.3,16.3,16.2,16.2,16.1,16.0,16.0,15.9,15.8,15.7,15.6,15.5,15.4,15.3,15.2,15.1,15.0,14.9,14.8,14.7,14.5,14.4,14.3,14.2,14.0,13.9,13.7,13.6,13.5,13.3,13.2,13.0,12.9,12.7,12.5,12.4,12.2,12.1,11.9,11.7,11.6,11.4,11.3,11.1,10.9,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};
float centre_ratio_2[Process_Hang]={1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,10.9,11.1,11.3,11.4,11.6,11.7,11.9,12.1,12.2,12.4,12.5,12.7,12.9,13.0,13.2,13.3,13.5,13.6,13.7,13.9,14.0,14.2,14.3,14.4,14.5,14.7,14.8,14.9,15.0,15.1,15.2,15.3,15.4,15.5,15.6,15.7,15.8,15.9,16.0,16.0,16.1,16.2,16.2,16.3,16.3,16.4,16.4,16.5,16.5,16.5,16.6,16.6,16.6,16.6,16.6,16.6,16.6,16.6,16.6,16.6,16.6,16.5,16.5,16.5,16.4,16.4,16.3,16.3,16.2,16.2,16.1,16.0,16.0,15.9,15.8,15.7,15.6,15.5,15.4,15.3,15.2,15.1,15.0,14.9,14.8,14.7,14.5,14.4,14.3,14.2,14.0,13.9,13.7,13.6,13.5,13.3,13.2,13.0,12.9,12.7,12.5,12.4,12.2,12.1,11.9,11.7,11.6,11.4,11.3,11.1,10.9,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};
float centre_ratio_new[111]={0.3,0.4,0.5,0.5,0.7,0.8,0.9,1.1,1.3,1.5,1.8,2.1,2.4,2.8,3.2,3.7,4.2,4.8,5.5,6.2,7.0,7.9,8.9,10.0,11.1,12.4,13.7,15.1,16.6,18.2,19.9,21.7,23.5,25.4,27.4,29.4,31.4,33.5,35.6,37.7,39.8,41.8,43.8,45.7,47.6,49.3,51.0,52.5,53.9,55.1,56.2,57.1,57.8,58.3,58.6,58.7,58.7,58.6,58.4,58.3,58.0,57.8,57.4,57.1,56.7,56.2,55.7,55.2,54.6,53.9,53.3,52.6,51.9,51.1,50.3,49.5,48.7,47.9,47.0,46.1,45.2,44.3,43.4,42.5,41.6,40.6,39.7,38.8,37.9,36.9,36.0,35.1,34.2,33.4,32.5,31.7,30.8,30.0,29.2,28.4,27.7,27.0,26.2,25.6,24.9,24.2,23.6,23.0,22.4,21.9,21.3};
//历史值
int His_centre[30]={0};
int His_black_L[30]={0};
int His_black_R[30]={0};
int His_halfwidth[30]={0};
unsigned char His_Left_flag[30]={0};
unsigned char His_Right_flag[30]={0};




/*************************************************************************
*  函数名称：Capture_Process_Image
*  功能说明：处理图像信息，算出中线值，并且算出偏差来量
*  参数说明：无
*  函数返回：无
*  修改时间：2012-2-14    已测试
*  运行时间：?ms,在200M情况下
*************************************************************************/
void Process_Image()
{
    int i;
    int centre_sum = 0;

    //起跑线检测
    if(StartLine.enable==1 && Time_1ms > 15000 && StartLine.flag==0)
        Judge_startline_by_camera();


    //采集中线
    if(Speed.Mode==1 || Speed.Mode==6 || Speed.Mode==7)//1档，4700
    {
        H = 150;
        black_centre = (int)(get_black_line_oth(Image_fire[H],H));
        Store_History();
        if(Multiple_line_enable==1)//加权平均才运行
        {
            for(i=near_hang;i>far_hang;i--)
                get_black_line(Image_fire[i],i);
            valid_farthest = find_valid_farthest();
            if(valid_farthest==-1 || valid_farthest>130)
            {       
            }
            else
            {
                centre_sum = 0;
                float sum_ratio=0;
                for(i=near_hang;i>valid_farthest;i--)
                {
                    centre_sum += centre[i]*centre_ratio[i];
                    sum_ratio += centre_ratio[i];
                }
                black_centre = (int)(centre_sum/sum_ratio);
            } 
        }
    }
    else if(Speed.Mode==2)//2档，5300
    {
        H = 137;
        black_centre = (int)(get_black_line_oth(Image_fire[H],H));
        Store_History();
        if(Multiple_line_enable==1)//加权平均才运行
        {
            for(i=near_hang;i>far_hang;i--)
            {
                get_black_line(Image_fire[i],i);
            }
            valid_farthest = find_valid_farthest();
            if(valid_farthest == -1 || valid_farthest > 120)
            {          
            }
            else
            {
                centre_sum = 0;
                float sum_ratio=0;
                for(i=near_hang;i>valid_farthest;i--)
                {
                    centre_sum += centre[i]*centre_ratio[i];
                    sum_ratio += centre_ratio[i];
                }
                if(valid_farthest>95)
                {
                    for(i=valid_farthest;i>95;i--)
                    {
                        centre_sum += centre[valid_farthest+1]*centre_ratio[i];
                        sum_ratio += centre_ratio[i];
                    }
                }
                black_centre = (int)(centre_sum/sum_ratio);
            }
        }
    }
    else if(Speed.Mode==3)//3档，5800
    {
        H = 125;
        black_centre = (int)(get_black_line_oth(Image_fire[H],H));
        Store_History();
        if(Multiple_line_enable==1)//加权平均才运行
        {
            for(i=near_hang;i>far_hang;i--)
                get_black_line(Image_fire[i],i);
            valid_farthest = find_valid_farthest();
            if(valid_farthest == -1)
            {       
            }
            else
            {
                centre_sum = 0;
                float sum_ratio=0;
                for(i=190;i>valid_farthest;i--)
                {
                    if(i>175)
                        centre_ratio_step = 0;        //8       
                    else if(i>160) 
                        centre_ratio_step = 9;        //10
                    else if(i>145)
                        centre_ratio_step = 9;       //13
                    else if(i>130) 
                        centre_ratio_step = 9;       //16
                    else if(i>115)     //130~115行  属于更靠近自己的行段，单调这段的权重（+2），入90度弯明显内切
                        centre_ratio_step = 20;       //18
                    else if(i>100)
                        centre_ratio_step = 21;       //16
                    else if(i>90)      //100~90行   属于较远离自己的行段，单调这段的权重（+1），入90度弯明显的内切就消失了
                        centre_ratio_step = 16;       //13
                    else if(i>80)
                        centre_ratio_step = 15;        //9    
                    else if(i>70)
                        centre_ratio_step = 15;        //9  
                    else if(i>60)
                        centre_ratio_step = 13;        //9  
                    else if(i>50)
                        centre_ratio_step = 10;        //9  
                    else if(i>40)
                        centre_ratio_step = 5;        //9  
                    else
                        centre_ratio_step = 0;        //9  
                    centre_sum += centre[i]*centre_ratio_step;
                    sum_ratio += centre_ratio_step;                        
                }
                if(valid_farthest>105)
                {
                    for(i=valid_farthest;i>105;i--)
                    {
                        if(i>175)
                            centre_ratio_step = 8;
                        else if(i>160) 
                            centre_ratio_step = 10;
                        else if(i>145)
                            centre_ratio_step = 13;
                        else if(i>130)
                            centre_ratio_step = 16;
                        else 
                            centre_ratio_step = 18;
                        centre_sum += centre[valid_farthest+1]*centre_ratio_step;
                        sum_ratio += centre_ratio_step;                         
                    }
                }
                black_centre = (int)(centre_sum/sum_ratio);
            }
        }
    }
    else if(Speed.Mode==4)//4档，6300
    {
        H = 120;
        black_centre = (int)(get_black_line_oth(Image_fire[H],H));
        Store_History();
        if(Multiple_line_enable==1)
        {
            for(i=near_hang;i>far_hang;i--)
                get_black_line(Image_fire[i],i);
            valid_farthest = find_valid_farthest();
            if(valid_farthest == -1)
            {       
            }
            else
            {
                centre_sum = 0;
                float sum_ratio=0;
                for(i=190;i>valid_farthest;i--)
                {
                    if(i>175)
                        centre_ratio_step = 0;        //8       
                    else if(i>160) 
                        centre_ratio_step = 10;        //10
                    else if(i>145)
                        centre_ratio_step = 14;       //13
                    else if(i>130) 
                        centre_ratio_step = 16;       //16
                    else if(i>115)     //130~115行  属于更靠近自己的行段，单调这段的权重（+2），入90度弯明显内切
                        centre_ratio_step = 19;       //18
                    else if(i>100)
                        centre_ratio_step = 21;       //16
                    else if(i>90)      //100~90行   属于较远离自己的行段，单调这段的权重（+1），入90度弯明显的内切就消失了
                        centre_ratio_step = 24;       //13
                    else if(i>80)
                        centre_ratio_step = 24;        //9    
                    else if(i>70)
                        centre_ratio_step = 24;        //9  
                    else if(i>60)
                        centre_ratio_step = 24;        //9  
                    else if(i>50)
                        centre_ratio_step = 20;        //9  
                    else if(i>40)
                        centre_ratio_step = 15;        //9  
                    else
                        centre_ratio_step = 0;        //9  
                    centre_sum += centre[i]*centre_ratio_step;
                    sum_ratio += centre_ratio_step;                        
                }
                if(valid_farthest>105)
                {
                    for(i=valid_farthest;i>105;i--)
                    {
                        if(i>175)
                            centre_ratio_step = 8;
                        else if(i>160) 
                            centre_ratio_step = 10;
                        else if(i>145)
                            centre_ratio_step = 13;
                        else if(i>130)
                            centre_ratio_step = 16;
                        else 
                            centre_ratio_step = 18;
                        centre_sum += centre[valid_farthest+1]*centre_ratio_step;
                        sum_ratio += centre_ratio_step;                         
                    }
                }
                black_centre = (int)(centre_sum/sum_ratio);
            }
        }
    }
    else if(Speed.Mode==5)//5档，5500
    {
        H = 130;
        black_centre = (int)(get_black_line_oth(Image_fire[H],H));
        Store_History();
        if(Multiple_line_enable==1)
        {
            for(i=near_hang;i>far_hang;i--)
                get_black_line(Image_fire[i],i);
            valid_farthest = find_valid_farthest();
            if(valid_farthest == -1)
            {       
            }
            else
            {
                centre_sum = 0;
                float sum_ratio=0;
                for(i=190;i>valid_farthest;i--)
                {
                    if(i>175)
                        centre_ratio_step = 0;        //8       
                    else if(i>160) 
                        centre_ratio_step = 20;        //10
                    else if(i>145)
                        centre_ratio_step = 20;       //13
                    else if(i>130) 
                        centre_ratio_step = 20;       //16
                    else if(i>115)     //130~115行  属于更靠近自己的行段，单调这段的权重（+2），入90度弯明显内切
                        centre_ratio_step = 25;       //18
                    else if(i>100)
                        centre_ratio_step = 30;       //16
                    else if(i>90)      //100~90行   属于较远离自己的行段，单调这段的权重（+1），入90度弯明显的内切就消失了
                        centre_ratio_step = 35;       //13
                    else if(i>80)
                        centre_ratio_step = 40;        //9    
                    else if(i>70)
                        centre_ratio_step = 40;        //9  
                    else if(i>60)
                        centre_ratio_step = 40;        //9  
                    else if(i>50)
                        centre_ratio_step = 40;        //9  
                    else if(i>40)
                        centre_ratio_step = 40;        //9  
                    else if(i>30)
                        centre_ratio_step = 35;        //9  
                    else if(i>20)
                        centre_ratio_step = 10;        //9 
                    else if(i>10)
                        centre_ratio_step = 5;        //9
                    else
                        centre_ratio_step = 0;        //9  
                    centre_sum += centre[i]*centre_ratio_step;
                    sum_ratio += centre_ratio_step;                        
                }
                if(valid_farthest>105)
                {
                    for(i=valid_farthest;i>105;i--)
                    {
                        centre_ratio_step = 30;
                        centre_sum += centre[valid_farthest+1]*centre_ratio_step;
                        sum_ratio += centre_ratio_step;                         
                    }
                }
                black_centre = (int)(centre_sum/sum_ratio);
            }
        }
    }
    else//预防拨码坏了：1档，4700
    {
        H = 140;
        black_centre = (int)(get_black_line(Image_fire[H],H));
        Store_History();
    }

    
    
    
    //人字识别
    if(Ren.enable==1 && Ren.flag==0)
        Judge_Ren();
    //坡道检测
    if(Slope_Check_enable==1 && Speed.Mode!=0 && Slope_flag==0 && Time_1ms > 5500)
        Slope_Check();
    else if(Slope_flag==1 && Time_1ms > Slope_time + Slope_delaytime)
        Slope_flag = 0;
    //十字识别
    if(Ren.flag==0) 
        Cross_judge();
    //障碍识别
    if(Ren.flag==0 && Cross_flag==0 && Block.flag==0 && Time_1ms > Block.time + 1000)//Block.enable==1 && //有机会进国赛再改
        Block_judge();
    
    
    //SD(赛场上要干掉)
//    if(SD_enable==1)
//    {
//        if(Time_1ms > Stand_Time)
//            SD_Writedata();
//    }
    //LCD刷屏
    if(Speed.Mode==0)//以下为LCD显示
    {
        //加权平均
        centre_sum = 0;
        float sum_ratio=0;
        for(i=near_hang;i>far_hang;i--)
        {
            get_black_line(Image_fire[i],i);
            Image_fire[i][centre[i]/8] = Image_fire[i][centre[i]/8] | (0x80>>(centre[i]%8));
        } 
        valid_farthest = find_valid_farthest();
        if(valid_farthest!=-1)
        {
            for(i=near_hang;i>valid_farthest;i--)
            {
                centre_sum += centre[i]*centre_ratio[i];
                sum_ratio += centre_ratio[i];
            }
            black_centre = (int)(centre_sum/sum_ratio);
        }
        LCD_Put_Int(0,200,"va_far:",valid_farthest,Red,Green);
        LCD_Put_Int(0,220,"centre:",black_centre,Red,Green);
        Draw_single_line('L',black_centre,BLACK);
        Draw_single_line('H',near_hang,BLACK);
        if(valid_farthest != -1)
            Draw_single_line('H',valid_farthest,BLACK);
        Draw_single_line('H',far_hang,BLACK);
        Send_Image_to_LCD(Image_fire);
    }
}




/*************************************************************************
*  函数名称：get_black_line_oth（和get_black_line的区别是，里面用的变量不是同一套。防止人字障碍对寻线的影响）
*  功能说明：第八届一行处理的ccd算法
*  参数说明：ImageData_in为某一行的初地址，hang为第几行
*  函数返回：中点的值
*  修改时间：2012-2-14    已测试
*************************************************************************/
int get_black_line_oth(unsigned char *ImageData_in,int hang)  //限值
{ 
    int i,ccd_start=5,ccd_end=315;
    unsigned char getleft_flag=0,getright_flag=0; 
    int Left_Count, Right_Count;
    static int _black_R,_black_L,Middle=CENTER,_halfwidth=80;
    static unsigned char ImageData[Lie+2];
    
    static unsigned char first_run=0;
    if(first_run==0)
    {
        first_run++;
        for(int count=0;count<240;count++)
        {
            centre_oth[count] = 160;
            halfwidth_oth[count] = 80;
        }
    }
    else
    {
        Middle = centre_oth[hang];
        _halfwidth = halfwidth_oth[hang];
    }
    //解压
    for(i=0;i<40;i++)
        for(int k=0;k<8;k++)
            ImageData[i*8+k] = (ImageData_in[i]>>(7-k))&0x01;
    
    Right_Count = Middle;
    while(!(ImageData[Right_Count+3]==BLACK 
            && ImageData[Right_Count+2]==BLACK
                && ImageData[Right_Count+1]==BLACK)
          && Right_Count < ccd_end)
    {Right_Count++;}
    if(Right_Count<ccd_end)
    {
        _black_R = Right_Count;
        getright_flag=1;
    }
    else
        getright_flag=0;
    
    //4.找到左面deal left infomation
    Left_Count = Middle;
    while(!(ImageData[Left_Count-3]==BLACK 
            && ImageData[Left_Count-2]==BLACK
                && ImageData[Left_Count-1]==BLACK)
          && Left_Count > ccd_start)	  
    {Left_Count--;}
    if(Left_Count > ccd_start)
    {
        _black_L = Left_Count;
        getleft_flag=1;
    } 
    else
        getleft_flag=0;
    
    //返回算中线  
    if(getleft_flag==0 && getright_flag==0)
    {
        Middle = CENTER;
    }
    else if(getleft_flag!=1 && getright_flag==1)
    {
        Middle = _black_R - _halfwidth;
        _black_L = _black_R - _halfwidth*2;
    }
    else if(getleft_flag==1 && getright_flag!=1)
    {
        Middle = _black_L + _halfwidth;
        _black_R = _black_L + _halfwidth*2; 
    }
    else if(getleft_flag==1 && getright_flag==1) 
    {
        //半宽限幅
        _halfwidth=(int)((_black_R - _black_L)/2.0) ;
        if(_halfwidth < 80)
            _halfwidth = 80;
        else if(_halfwidth > 110)
            _halfwidth = 110; 
        Middle = (int)((_black_R + _black_L)/2.0) ;
    }
    //limit	 Middle
    if(Middle<60)
        Middle=60;
    else if(Middle>250)
        Middle=250;
    //data record 
    centre_oth[hang] = Middle;
    black_L_oth[hang] = _black_L;
    black_R_oth[hang] = _black_R;
    halfwidth_oth[hang] = _halfwidth;
    getLeft_flag_oth[hang] = getleft_flag;
    getRight_flag_oth[hang] = getright_flag;
    return(Middle);
}



/*************************************************************************
*  函数名称：加权平均get_black_line
*  功能说明：某一行第八届ccd算法
*  参数说明：lie为多少列
*  函数返回：Ren_falg，1为是，0为不是
*  修改时间：2012-2-14    已测试
*************************************************************************/
int get_black_line(unsigned char *ImageData_in,int hang)  //限值
{ 
    int i,ccd_start=5,ccd_end=315;
    unsigned char getleft_flag=0,getright_flag=0; 
    int Left_Count, Right_Count;
    static int _black_R,_black_L,Middle=CENTER,_halfwidth=80;
    static unsigned char ImageData[Lie+2];
    
    static unsigned char first_run=0;
    if(first_run==0)
    {
        first_run++;
        for(int count=0;count<240;count++)
        {
            centre[count] = 160;
            halfwidth[count] = 80;
        }
    }
    else
    {
        if(hang==180)
        { 
            Middle = centre[180];
            _halfwidth = halfwidth[180];
        }
        else
        {
            Middle = centre[hang+1];
            _halfwidth = halfwidth[hang+1];
        }
        
    }
    //解压
    for(i=0;i<40;i++)
        for(int k=0;k<8;k++)
            ImageData[i*8+k] = (ImageData_in[i]>>(7-k))&0x01;
    
    Right_Count = Middle;
    while(!(ImageData[Right_Count+3]==BLACK 
            && ImageData[Right_Count+2]==BLACK
                && ImageData[Right_Count+1]==BLACK)
          && Right_Count < ccd_end)
    {Right_Count++;}
    if(Right_Count<ccd_end)
    {
        _black_R = Right_Count;
        getright_flag=1;
    }
    else
        getright_flag=0;
    
    //4.找到左面deal left infomation
    Left_Count = Middle;
    while(!(ImageData[Left_Count-3]==BLACK 
            && ImageData[Left_Count-2]==BLACK
                && ImageData[Left_Count-1]==BLACK)
          && Left_Count > ccd_start)	  
    {Left_Count--;}
    if(Left_Count > ccd_start)
    {
        _black_L = Left_Count;
        getleft_flag=1;
    } 
    else
        getleft_flag=0;
    
    //返回算中线  
    if(getleft_flag==0 && getright_flag==0)
    {
        Middle = CENTER;
    }
    else if(getleft_flag!=1 && getright_flag==1)
    {
        Middle = _black_R - _halfwidth;
        _black_L = _black_R - _halfwidth*2;
    }
    else if(getleft_flag==1 && getright_flag!=1)
    {
        Middle = _black_L + _halfwidth;
        _black_R = _black_L + _halfwidth*2; 
    }
    else if(getleft_flag==1 && getright_flag==1) 
    {
        //半宽限幅
        _halfwidth=(int)((_black_R - _black_L)/2.0) ;
        if(_halfwidth < _halfwidth_min[hang])
            _halfwidth = _halfwidth_min[hang];
        else if(_halfwidth > _halfwidth_max[hang])
            _halfwidth = _halfwidth_max[hang]; 
        Middle = (int)((_black_R + _black_L)/2.0) ;
    }
    //limit	 Middle
    if(Middle<50)
        Middle=50;
    else if(Middle>270)
        Middle=270;
    //data record 
    centre[hang] = Middle;
    black_L[hang] = _black_L;
    black_R[hang] = _black_R;
    halfwidth[hang] = _halfwidth;
    getLeft_flag[hang] = getleft_flag;
    getRight_flag[hang] = getright_flag;
    return(Middle);
}



/*************************************************************************
*  函数名称：find_valid_farthest
*  功能说明：找到
*  参数说明：lie为多少列
*  函数返回：Ren_falg，1为是，0为不是
*  修改时间：2012-2-14    已测试
*************************************************************************/
int find_valid_farthest()
{
    int double_sides_ct=0,addr_2=near_hang;
    int left_side_ct=0,addr_1L=near_hang;
    int right_side_ct=0,addr_1R=near_hang;
    int zero_side_ct=0,addr_0=near_hang;
    int LRflag_fore,LRflag=1,change_cn=0,add;
    
    
    //寻找各个边线
    for(int i=near_hang;i>far_hang;i--)
    {
        if(getLeft_flag[i]==1 && getRight_flag[i]==1)
        {
            LRflag_fore=LRflag;
            LRflag = 1;
            if(double_sides_ct==0)
            {
                double_sides_ct++;
                addr_2=i;
            }
        }
        if(getLeft_flag[i]==1 && getRight_flag[i]==0)
        {
            LRflag_fore=LRflag;
            LRflag = 2;
            if(left_side_ct==0)
            {
                left_side_ct++;
                addr_1L=i;
            }
        }
        if(getLeft_flag[i]==0 && getRight_flag[i]==1)
        {
            LRflag_fore=LRflag;
            LRflag = 3;
            if(right_side_ct==0)
            {
                right_side_ct++;
                addr_1R=i;
            }
        }
        if(getLeft_flag[i]==0 && getRight_flag[i]==0)
        {
            LRflag_fore=LRflag;
            LRflag = 4;
            if(zero_side_ct==0)
            {
                zero_side_ct++;
                addr_0=i;
            }
        }
        if(i!=near_hang && LRflag!=LRflag_fore)
        {
            change_cn++;
            if(change_cn>=2)
            {
                add=i;
                break;
            }
        }
    }
    
    
    //直线。所有行都有双边
    if(change_cn==0 && (!(getLeft_flag[near_hang]==0 && getRight_flag[near_hang]==0)))
        return (far_hang+1);
    //弯道或者十字。一共两种沿
    else if(change_cn==1)
    {
        //先有双边后（单边 || 十字）
        if(getLeft_flag[near_hang]==1 && getRight_flag[near_hang]==1)
        {
            //有突变的左边，防止十字
            if(left_side_ct==1)
            {
                if(black_L[addr_1L] < black_L[near_hang]-20)//防止弯着入十字
                    return (addr_1L+1);
            }
            else
                return (far_hang+1);
            //有突变的右边，防止十字
            if(right_side_ct==1)
            {
                if(black_R[addr_1R] > black_R[near_hang]+20)//防止弯着入十字
                    return (addr_1R+1);
            }
            else
                return (far_hang+1);
        }
        //先有左边后（双边 || 十字）
        else if(getLeft_flag[near_hang]==1 && getRight_flag[near_hang]==0)
        {
            if(double_sides_ct==1)
                return(addr_2+1);
            else if(zero_side_ct==1)
                return(addr_0+1);
        }
        //先有右边后（双边 || 十字）
        else if(getLeft_flag[near_hang]==0 && getRight_flag[near_hang]==1)
        {
            if(double_sides_ct==1)
                return(addr_2+1);
            else if(zero_side_ct==1)
                return(addr_0+1); 
        }
        //先有十字后（双边 || 单边）
        else if(getLeft_flag[near_hang]==0 && getRight_flag[near_hang]==0)
        {
            return(-1);//此时要特殊处理，因为要出十字了
        }
    }
    //弯道。一共三种种沿
    else if(change_cn==2)
    {
        //先有双边后（单边 || 十字）
        if(getLeft_flag[near_hang]==1 && getRight_flag[near_hang]==1)
        {
            if(zero_side_ct==1)
                return(-1);
            else if(left_side_ct==1 && right_side_ct==1)
                return(-1);
            else if(left_side_ct==1 && right_side_ct==0)
            {
                if(black_L[add] < black_L[near_hang]-20)
                    return(-1);
                else
                    return(add+1);
            }
            else if(left_side_ct==0 && right_side_ct==1)
            {
                if(black_R[add] > black_R[near_hang]+20)//防止弯着入十字
                    return(-1);
                else
                    return(add+1);
            }
        }
        //先有单边后（单边 || 十字）
        else if(getLeft_flag[near_hang]==1 && getRight_flag[near_hang]==0)
        {
            if(zero_side_ct==1 || right_side_ct==1)
                return(-1);
            else if(double_sides_ct==1)
                return(addr_2+1);
        }
        else if(getLeft_flag[near_hang]==0 && getRight_flag[near_hang]==1)
        {
            if(zero_side_ct==1 || left_side_ct==1)
                return(-1);
            else if(double_sides_ct==1)
                return(addr_2+1);
        }
    }
    return(-1);
}