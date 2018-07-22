#include "include.h"
uint8  LMR[3][60];  //分别对应左边线，总线，右边线。
uint8  RoadType=0;
float Previous_Error[12];
void Push_And_Pull(float *buff,int len,float newdata)
{
 int i;
 for(i=len-1;i>0;i--)
 {
   *(buff+i)=*(buff+i-1);
 }
   *buff=newdata; 
}
void Search()
{
  //从底部往上搜线
  int i,j,cnt=0;
  float miderror=0;
  int white_begin=0; //白色区域开始的横坐标0
  int white_end=79;   //白色区域结束的横坐标79
//  int old_white_begin=0; //白色区域开始的横坐标0
//  int old_white_end=0;   //白色区域结束的横坐标79
//  uint8 buxianc_count=0;
  uint8 *start;
  for(i=0;i<60;i++)  //清空数组
  {
    
          LMR[0][i]=0;
          LMR[1][i]=0;
          LMR[2][i]=0;
  }
  for(i=59;i>0;i--) //从第59行开始搜线
  {
    start=&(img[i*80]);//行的初始地址
    white_begin=0;
    white_end=79; 
//    if(i!=59&&((LMR[0][i+1]!=0)||(LMR[2][i+1]!=0))) //上一行至少一边搜到边线了
//    {
//      if(LMR[0][i+1]!=0)
//     {  
//       for(j=LMR[0][i+1]-3;j<(LMR[0][i+1]+3);j++)   //因为上次搜到边线了，故这一行的边线从上一行边线的附近搜
//       {
//        if(j<0) break;
//            if((*(start+j)!=0)&&(*(start+j-1)==0)) //由黑变白
//            { 
//              LMR[0][i]=j; //找到一个跳变点就认为搜到线了
//      
//              if(LMR[2][i+1]==0) 
//              {
//                for(j=j+10;j<79;j++)
//                {
//                     if((*(start+j)==0)&&(*(start+j-1)!=0)) //由白变黑
//                    { 
//                      LMR[2][i]=j; //找到一个跳变点就认为受到线了
//                      break; 
//                    }
//                 }
//              }
//              break; 
//            }
//        }
//      } 
//      if(LMR[2][i+1]!=0)
//      {
//        for(j=LMR[2][i+1]-3;j<(LMR[2][i+1]+3);j++)   //因为上次搜到边线了，故这一行的边线从上一行边线的附近搜
//       {
//          if(j>79) break;
//              if((*(start+j)==0)&&(*(start+j-1)!=0)) //由白变黑
//            { 
//              LMR[2][i]=j; //找到一个跳变点就认为受到线了
//               if(LMR[0][i+1]==0) 
//              {
//                for(j=j-10;j>0;j--)
//                {
//                      if((*(start+j)!=0)&&(*(start+j-1)==0)) //由黑变白
//                    { 
//                       LMR[0][i]=j; //找到一个跳变点就认为受到线了
//                      break; 
//                    }
//                 }
//              }   
//              break; 
//            }
//        }
//       }
//    }
//    else 
      if(i>1)           //上一行没有同时搜到边线且不是顶部的行（顶部的行容易出错，排掉）
    {
        for(j=40;j>1;j--)  //从中间往两边搜线
        {
       
            if((*(start+j)!=0)&&(*(start+j-1)==0)&&(*(start+j-2)==0)&&(white_begin==0))                  white_begin=j;  //由黑变白    
            if((*(start+80-j+1)==0)&&(*(start+80-j)==0)&&(*(start+80-j-1)!=0)&&(white_end==79))            white_end=80-j;    //由白变黑
            
            if((white_end!=79)&&(white_begin!=0)&&(white_begin<white_end)&&(white_end-white_begin>10))
            {
              LMR[0][i]=white_begin;  //左边线
              LMR[2][i]=white_end;    //右边线
//              old_white_begin=white_begin;
//              old_white_end=white_end;
              break;
            }
//            buxianc_count++;
//            if(j<5&&(*(start+j)!=0)&&(*(start+80-j-1)!=0)&&(white_begin==0)&&(white_end==79))
//            {
//              LMR[0][i]=old_white_begin;  //左边线
//              LMR[2][i]=old_white_end;    //右边线
//            }
        }
    }
  
    if(white_begin!=0&&white_end==79)
    {
         LMR[0][i]=white_begin;
    }
    if(white_begin==0&&white_end!=79)
    {
         LMR[2][i]=white_end;
    }
    if((LMR[2][i]!=0)&&(LMR[0][i]!=0))
     LMR[1][i]=(LMR[2][i]+LMR[0][i])/2;  //中心线
    else
    {
       if(LMR[2][i]!=0)
       LMR[1][i]=(LMR[2][i])/2;  //中心线
        if(LMR[0][i]!=0)
       LMR[1][i]=(LMR[0][i]+80)/2;  //中心线
    }
    
//    if(i>30&&i<50&&LMR[1][i]!=0)
//    {
//      cnt++;
//      miderror+=(LMR[1][i]-46);
//    }
//     buxianc_count=0;
  }
  for(i=31;i>30&&i<50&&LMR[1][i]!=0;i++)
  {
    cnt++;
    miderror+=(LMR[1][i]-46);
  }
  if(cnt!=0)  Middle_Err=miderror*1.0/cnt;
//  Middle_Err=Middle_Err_Filter(Middle_Err);
  Push_And_Pull(Previous_Error,10,Middle_Err);
  
}
