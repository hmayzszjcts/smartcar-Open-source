/* *****************************************       
      图像处理函数
      功能：找到两边黑线
            找出中线
            处理十字弯
            求解有效行
            处理直角弯 
 **************  3月13日   ******************/  
#include "common.h"
#include "include.h" 
#include "init.h"
#include "tx_process.h" 
/*
#define    P_1       10   //9  
#define    P_2        4
#define    P_3        9   //8
#define    D_1       12
#define    D_2        4   //0
#define    D_3       13*/

#define    S_1         0
#define    S_2         0.5
#define    S_3         0

#define    DP_1        0
#define    DD_1        0



#define   ZP_1         6 
#define   ZD_1         2
#define   ZS_1         0

   
uint8   P_1=5; 
uint8   P_2=2; 
uint16  P_3=5; 
uint16  D_1=6;
uint32  D_2=1; 
uint32  D_3=6;

int16   blackright[num],blackleft[num],center[num],center_2[num];
uint8   valid_row,valid_row_;
int32   angle_pwm;
int32   last_angle;

extern uint8 heixian_flag_0;
extern int16  val_z, val_y;  

int16   bxiankuan[52]= {   32,32,32,31,31,31,30,30,30,29,29,
                           28,28,27,27,27,26,26,25,25,25,
                           24,24,23,23,22,22,21,21,20,20,
                           19,19,19,18,18,17,17,16,16,16,
                           15,15,14,14,13,13,13,13,12,12 ,11};
int16   Fxiankuan[52]= {   16,16,16,16,16,16,15,15,15,15,15,
                           14,14,14,14,14,13,13,13,13,13,
                           12,12,12,12,11,11,11,11,10,10,
                           10,10,10, 9, 9, 9, 9, 8, 8, 8,
                            8, 8, 7, 7, 7, 7, 7, 7, 6, 6,6};
/*
uint8   bxiankuan[52]= {   32,32,31,31,30,30,29,29,28,27,26,
                           26,26,25,25,24,24,24,23,23,23,
                           22,22,21,21,20,19,19,18,18,18,
                           17,17,17,16,16,15,15,14,14,14,
                           13,13,12,12,12,12,12,12,12,12 ,12};*/

uint8  sample_row[52]={59,55,54,53,52,51,50,49,48,47,
                       46,45,44,43,42,41,40,39,38,37,
                       36,35,34,33,32,31,30,29,28,27,
                       26,25,24,23,22,21,20,19,18,17,
                       16,14,13,12,11,10, 9, 8, 7, 6, 5, 4};

float  quanzhi[52]=    {  1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.1, 1.1, 1.1, 1.1, 1.1, 1.1,
                          1.3, 1.3, 1.3, 1.3, 1.3, 1.3, 1.5, 1.5, 1.5, 1.5, 1.5,
                          1.6, 1.6, 1.6, 1.6, 1.6, 1.6, 1.6, 1.6, 1.6, 1.6, 1.6,
                          1.8, 1.8, 1.8, 1.8, 1.8, 1.8, 1.8, 1.8, 1.8, 1.8, 1.8,
                          2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0 };
float abs_flt(float i)//作用： 求绝对值  浮点型
{
     float A;
     A=i;
     if(A<0) return (-A);
     else    return A;
} 

int16 abs_int(int16 i)//作用： 求绝对值  int型
{
     int16 B;
      B=i;
     if(B<0) return (-B);
     else    return B;
} 
/*************函数功能********************        
          滤波采用中值滤波
          
          可优化 
*************3月13号  验证通过 ***********/  
uint8  MiddleFilter(uint8 a,uint8 b,uint8 c)
{  
  uint8 temp;
  if(a>b) 
  {
    temp=b;
    b=a;
    a=temp;
  }
  if(b>c)
  {
    temp=c;
    c=b;
    b=temp;
  }
  if(a>b)
  {
    temp=b;
    b=a;
    a=temp;
  }
  return b;
}


int16 max(int16 a,int16 b,int16 c)
{
     uint16 temp;
     temp=a;
     if(b>temp) 
     {
       temp=b;
     }
     if(c>temp)
     {
       temp=c;
     }
      if(temp==a&&temp!=b&&temp!=c)
      return 49;
      if(temp==b&&temp!=a&&temp!=c)
      return 50;
      if(temp==c&&temp!=a&&temp!=b)
      return 51;
      if(temp==a&&temp==b&&temp!=c)
      return 50;
      if(temp==a&&temp!=b&&temp==c)
      return 50;
      if(temp!=a&&temp==b&&temp==c)
      return 51;
      if(temp==c&&temp==b&&temp==a)
      return 51;
}
/*************函数功能********************        
          预判左右直角的方向
            
          可优化  
*************3月13号  验证通过 ***********/  


/*预判函数，判断直角的左右方向，避免误判，思路如下：
重新寻线，如果左边连续三行丢线且为白色，右边没丢线，且远小于79.则为左直角
，反之，则为近端丢线*/
int8  y_zj_flag=0,z_zj_flag=0;
void yupan_zy()
{
   uint8 i;
   for(i=0;i<40;i++)
   {
      if(blackleft[i]!=0&&blackright[i]==79&&
         blackleft[i+1]!=0&&blackright[i+1]==79)
      {
          if(i!=0)       //排除近端丢线
          {
               y_zj_flag=1;              
               break;
          }
          else
            break;
       }
       else if(blackleft[i]==0&&blackright[i]!=79&&
                blackleft[i+1]==0&&blackright[i+1]!=79)
       {
           if(i!=0)       //排除近端丢线
           {
                z_zj_flag=1;               
                break;
           }
           else
             break;
       }
    }
}



uint16 yp_left=0,yp_right=0,szis=0,is,centerzuoyupan[52];
uint16 yupan_fangxiang(uint16 yp_qishi)//只是预判，与最终中线无关！！！
{
  yp_left=0,yp_right=0,szis=0;
  for(is=yp_qishi;is<=48;is++)
  {
     if(blackright[is]>r-2&&blackleft[is]==0)
       centerzuoyupan[is]=r/2;//中心预判
     else if(blackright[is]>r-2&&blackleft[is]!=0&&(is>0))
       centerzuoyupan[is]=blackleft[is]+bxiankuan[is];//一端没线，粗略地根据赛道进行平移
     else if(blackright[is]<r-2&&blackleft[is]==0&&(is>0))
       centerzuoyupan[is]=blackright[is]-bxiankuan[is];
     else 
       centerzuoyupan[is]=(blackright[is]+blackleft[is])/2;//两边都有，取中值
     szis++;
   if(szis>=1)
   {
      if((centerzuoyupan[is]-centerzuoyupan[is-1])<=-1)
        yp_right++;//右倾
      else if((centerzuoyupan[is]-centerzuoyupan[is-1])>=1)
        yp_left++;//左倾
   }
 }
 if((yp_right>=(yp_right+yp_left+1)/2))
   return 1 ;//右倾较多
 if((yp_left>=(yp_right+yp_left+1)/2))
   return 2;//左倾较多
}

/*************函数功能********************        
          改进的跟踪边沿检测法
          
          可优化  
*************3月13号  验证通过 ***********/  
int8  SX_flag=0,DX_flag=0,DS_flag=0,SD_flag=0; 
uint8  danxian_count=0,suangxian_count=0;
int16  blackright_0[6]={79,79,79,79,79,79},blackleft_0[6]={0,0,0,0,0,0};
int8   z_diuxian=0,y_diuxian=0;
uint8  white_count;
int8   qianduan_flag=0,zhongduan_flag=0,houduan_flag=0; 
int8   last_pic;
int8   qianduan_count,zhongduan_count,houduan_count;
uint8  zhongdian;
void ShiBie_DX()
{
    int8 i,j,sample_row_1,flag=0; 
    zhongdian=0;
    danxian_count=0;
    suangxian_count=0;
    white_count=0;
    DX_flag=0;
    SX_flag=0; 
    DS_flag=0;
    SD_flag=0; 
    qianduan_flag=0;
    zhongduan_flag=0;
    houduan_flag=0; 
    qianduan_count=0;
    zhongduan_count=0;
    houduan_count=0;
       for(j=0;j<6;j++)
       {
          if(j==0)   sample_row_1=sample_row[4];
          if(j==1)   sample_row_1=sample_row[5];       //前段
          if(j==2)   sample_row_1=sample_row[20];
          if(j==3)   sample_row_1=sample_row[21];      //中段
          if(j==4)   sample_row_1=sample_row[35];
          if(j==5)   sample_row_1=sample_row[36];      //后段
          for(i=0;i<79-3;i++)       //只扫描近处30行的图像数据
          {
             if(img[sample_row_1][i]==1&&img[sample_row_1][i+1]==1&&img[sample_row_1][i+2]==0&&img[sample_row_1][i+3]==0)
             {
                blackright_0[j]=i+1;
                break;
             }      
           }
           for(i=79;i>0+3;i--)       //只扫描近处30行的图像数据
           {
              if(img[sample_row_1][i]==1&&img[sample_row_1][i-1]==1&&img[sample_row_1][i-2]==0&&img[sample_row_1][i-3]==0)
              {
                 blackleft_0[j]=i-1;
                 break;
              }   
           }
           for(i=0;i<79;i++)       //只扫描近处30行的图像数据
           {
              if(img[sample_row_1][i]==white)
              {
                 white_count++;
              }   
           }
           if(j==0||j==1)
           {
               if(blackleft_0[j]!=0&&blackright_0[j]!=79&&abs_int(blackright_0[j]-blackleft_0[j])<9&&abs_int(blackright_0[j]-blackleft_0[j])>1&&white_count>60)           
               {
                 qianduan_count++;
                 zhongdian=(blackright_0[j]+blackleft_0[j])/2;
               }
               if(qianduan_count==2)
                  qianduan_flag=1;    
           }
          
           if(j==2||j==3)
           {
              if(abs_int(blackright_0[j]-blackleft_0[j])<9&&abs_int(blackright_0[j]-blackleft_0[j])>1&&white_count>50)
                     zhongduan_count++; 
              if(zhongduan_count==2)
                     zhongduan_flag=1;
           }
          
           if(j==4||j==5)
           {
              if(abs_int(blackright_0[j]-blackleft_0[j])<9&&abs_int(blackright_0[j]-blackleft_0[j])>1&&white_count>50)
                     houduan_count++; 
              if(houduan_count==2)
                     houduan_flag=1;
           } 
           if(j==4||j==5&&houduan_flag==0)
	   {
	       if(abs_int(blackright_0[j]-blackleft_0[j])>50)     //此时一定不是双线        可能为单线末端和十字
	       {
		    for(i=0;i<79;i++)                                            //只扫描近处30行的图像数据
                    { 
                       if(img[sample_row_1][i]==white)
                       {
                             white_count++;
                       }   
                    }
		    if(white_count>60)
		    {
		      flag=1;
		     }
	       }
	      if(flag)                                                       //进一步判断是十字还是单线末端
	      {
		   if(qianduan_flag==1)
		   {
                       houduan_flag=1;						
		   }
		   if(qianduan_flag==0)
		   {
		       houduan_flag=0;
		   }	
	      }				
	   }
          white_count=0;
       }
         if(!qianduan_flag&&!zhongduan_flag&&!houduan_flag)
         {
              SX_flag=1;
             last_pic=2;
         }
         else if(qianduan_flag&&zhongduan_flag&&houduan_flag)
         {
              DX_flag=1; 
             last_pic=3;
         } 
        else if(!qianduan_flag&&houduan_flag)
         {
             SD_flag=1;
             last_pic=4;
         }
        else if(qianduan_flag&&!houduan_flag)
         {
             DS_flag=1;
             last_pic=5;
         }
        else
        {
            if(last_pic==2)        
            {  
                SX_flag=1;
            }
            else if(last_pic==3)   
            {              
                DX_flag=1;
            }
            else if(last_pic==4) 
            { 
                SD_flag=1;
            }
            else if(last_pic==5)  
            {
                DS_flag=1;
            }
        }
}


int8  Z_Z_Flag=0,Y_Y_Flag=0 ,M=0, L=0, Z_COUNT=0, Y_COUNT=0,last_flag_Z=0,last_flag_Y=0;  //障碍识别
int8  Z_hang=0 ,Y_hang=0;
void  SX_deal()     //提取黑线
{
    uint8 i,j,center_row_1,center_row_2,sample_row_1,find_right,find_left;

     Z_Z_Flag=0;
     Y_Y_Flag=0;
    for(i=0;i<52;i++)       //只扫描近处30行的图像数据
    {
        find_right=0;
        find_left=0;
        if(i==0)
        {
          center_row_1=40;
          center_row_2=40;
        }
        sample_row_1=sample_row[i];
        if(center_row_1>r)
           center_row_1=r;
        if(center_row_1<z)  
           center_row_1=z;
        if(center_row_2>r)
           center_row_2=r;
        if(center_row_2<z)  
           center_row_2=z; 
        for(j=center_row_1;j>1;j--)  //100
        {
          if(img[sample_row_1][j]==1&&img[sample_row_1][j-1]==0&&img[sample_row_1][j-2]==0)
          {
                 blackleft[i]=j;    //找到左边黑点
                 find_left=1;
                 center_row_1=blackleft[i]+8;
                 break;
          }
        }
        for(j=center_row_2;j<78;j++)  //100
        {
          if(img[sample_row_1][j]==1&&img[sample_row_1][j+1]==0&&img[sample_row_1][j+2]==0)
          {
                  blackright[i]=j;    //找到右边黑点
                  find_right=1;
                  center_row_2=blackright[i]-8;
                  break;
          }      
        }  
/*************函数功能******************** 
          障碍识别函数
        
          可优化
*************6月7号  验证通过（1） ***********/ 
        if(find_right&&find_left&&(Z_Z_Flag==0&&Y_Y_Flag==0))
        { 
             if(!img[sample_row_1][blackleft[i]+Fxiankuan[i]]&&img[sample_row_1][blackright[i]-Fxiankuan[i]])    //左为黑0，右边为白1；
             {
                L=L+1;                 //默认全局0
                if(last_flag_Z==1||L==1)  //全局置1    //第一次      第二次      第三次  第四次没有
                {                           //        1           2           3         0      
                   Z_COUNT=Z_COUNT+1;                 
                } 
                last_flag_Z=1;
             }
             else
             {
                  last_flag_Z=0;         //清零
                  Z_COUNT=0;             //清零
                  L=0;                   //清零
             }
 
             if(img[sample_row_1][blackleft[i]+Fxiankuan[i]]&& !img[sample_row_1][blackright[i]-Fxiankuan[i]])    //左为黑0，右边为白1；
             {
                 M=M+1;                 //默认全局0
                 if(last_flag_Y==1||M==1)  //全局置1    //第一次      第二次      第三次  第四次没有
                 {  
                    Y_COUNT=Y_COUNT+1;  
                 }
                 last_flag_Y=1;
             }
             else
             {
                  last_flag_Y=0;         //清零
                  Y_COUNT=0;             //清零
                  M=0;                   //清零
             }
             if(Z_COUNT>2)        //
             {
                Z_Z_Flag=1;
                Z_hang=i;
                
             }
            
             if(Y_COUNT>2)        //
             {
                Y_Y_Flag=1;
                Y_hang=i;
  
             }
        }
/*************函数功能******************** 
          障碍识别函数
         
          可优化
*************6月7号  验证通过（0） ***********/ 
    }      
}
void SD_deal()
{
    uint8 i,j,sample_row_1,find_left=0,find_right=0;
    for(i=0;i<52;i++)       //只扫描近处30行的图像数据
    {
        sample_row_1=sample_row[i];
        find_left=0;find_right=0;
        for(j=0;j<78;j++)       //只扫描近处30行的图像数据
        {
                if(img[sample_row_1][j]==1&&img[sample_row_1][j+1]==0&&img[sample_row_1][j+2]==0)
                {
                      blackright[i]=j+1;
		      find_right=1;
                      break;
                }
        }
        for(j=79;j>1;j--)       //只扫描近处30行的图像数据
        {
                if(img[sample_row_1][j]==1&&img[sample_row_1][j-1]==0&&img[sample_row_1][j-2]==0)
                {
                      blackleft[i]=j-1;
		      find_left=1;
                      break;
                }
        }
        if(find_left&&!find_right&&i>=2)
	{
	    blackright[i]=2*blackright[i-1]-blackright[i-2];
	}
	if(find_right&&!find_left&&i>=2)
	{
            blackleft[i]=2*blackleft[i-1]-blackleft[i-2];
	}
        center[i]=(blackright[i]+blackleft[i])/2;
    }
}
/*
      单线 采用10*1开窗口 改进的跟踪边沿检测法   不依赖背景

      输入：  qishi_hang         起始行
              center_point       起始中点
      终止行：52

      5月29号                    调用即可
                                                              */
uint8  jiezhi_hang;
void DX_deal(uint8 qishi_hang,uint8 center_point)
{
        uint8 i,j,k,sample_row_1;
       // uint8 center_point=(_blackright+_blackleft)/2;
        int8  start=center_point-10;
        int8  end=center_point+10;
        int8  jiezi_flag=0;
        for(j=qishi_hang;j<50;j++)
        {
          sample_row_1=sample_row[j];
          for(i=start;i<end;i++)
          {
            if(img[sample_row_1][i]==1&&img[sample_row_1][i+1]==1&&img[sample_row_1][i+2]==0)
            {
              blackleft[j]=i+1;
              for(k=i;k<end;k++)
              {
                  if(img[sample_row_1][k]==0&&img[sample_row_1][k+1]==1&&img[sample_row_1][k+2]==1)
                  {
                             blackright[j]=k+1 ;
                             center[j]=(blackright[j]+blackleft[j])/2;
                             start=center[j]-12;
                             end=  center[j]+12;
                             break;
                  }
              }
              if(abs_int(blackright[j]-blackleft[j])>8||abs_int(blackright[j]-blackleft[j])==0)
              {
                 jiezi_flag=1;
		 jiezhi_hang=i;
	         valid_row_=jiezhi_hang;
                 break;
              }
            }
          }
          center[i]=(blackright[i]+blackleft[i])/2;
        }
}
void DS_deal()
{
    uint8 i,j,k,sample_row_1,find_left=0,find_right=0;
    for(i=0;i<52;i++)       //只扫描近处30行的图像数据
    {
        sample_row_1=sample_row[i];
        find_left=0;find_right=0;
        for(j=1;j<77;j++)       //只扫描近处30行的图像数据
        {
                if(img[sample_row_1][j]==1&&img[sample_row_1][j+1]==0&&img[sample_row_1][j+2]==0)
                {
                      blackright[i]=j+1;
		      find_right=1;
                      break;
                }
        }
        for(j=77;j>0;j--)       //只扫描近处30行的图像数据
        {
                if(img[sample_row_1][j]==1&&img[sample_row_1][j-1]==0&&img[sample_row_1][j-2]==0)
                {
                      blackleft[i]=j+1;
		      find_left=1;
                      break;
                }
        }
        
	if(find_left&&!find_right)
	{
            blackright[i]=blackleft[i]+2*bxiankuan[i]-4;
	}
	if(find_right&&!find_left)
	{
	     blackleft[i]=blackright[i]-2*bxiankuan[i]-4;
	}

        center[i]=(blackright[i]+blackleft[i])/2;
    }
}
void  MOHU_Deal()
{ 
   uint8  i,GD_1,GD_2,GD1_flag=0,GD2_flag=0;
   if(DS_flag||SD_flag)
   {
      for(i=2;i<52;i++)
      {
          if(GD1_flag==0&&abs_int(center[i+1]-center[i])>5)             
          {
              GD_1=i;
              GD1_flag=1;  
          }
          if(GD1_flag)
          {
              if(abs_int(center[i+1]-center[i])>5)             
              {
                  GD_2=i;
                  GD2_flag=1;  
		  break;
              }
          }
      }
   }
   if(GD2_flag)
   {
      for(i=GD_1-1;i<=GD_2+1;i++)       
      {
         center[i]=center[GD_1-1];
      } 
   }
}
int8  bad_flag=0;
void Centerline()
{
    uint8 i;
    ShiBie_DX();
    for(i=0;i<52;i++)
    {
       center[i]=0;   
       center_2[i]=0;  
       blackleft[i]=0;
       blackright[i]=79;
    }  
    if(DX_flag)
    {
        DX_deal(0,zhongdian);
    }
    else if(SX_flag)
    {
        SX_deal();
    }
    else if(DS_flag)
    { 
        DS_deal();
    }
    else if(SD_flag)
    {
        SD_deal();
    }
    else
    {

    }
    MOHU_Deal();
    if(valid_row_<8)         //坏帧              1
    {
         bad_flag=1;
    }
}

/*************函数功能********************        
               弯入黑线 
               补线处理			   
	             
	          只处理弯入黑线的情况
			        判断左右
			     忽略直道入黑线
	 
*************6月9号   ********************/  

void HX_ZJ()              
{
	int8  YP_HX_flag=0,flag_1=0,count_y=0,count_z=0,HX_flag=0;
	uint8 i,k,m,JD_Hang;
	for(i=0;i<40;i++)
	{
	    sample_row_1=sample_row[i];
        if(YP_HX_flag==0&&abs_int(blackleft[i]-blackright[i])<5)
		{
			YP_HX_flag=1;
			JD_Hang=i;
			
		}
        if(YP_HX_flag)		
		{
		   for(k=JD_Hang;k>=JD_Hang-5;k--)             //调整系数可以选择判断 黑线倾斜的角度
		   {
			   if(abs_int(blackleft[i]-blackright[i])<bxiankuan[i]*2-10)
			   {
				   flag_1++;
			   }
		   }
           if(YP_HX_flag&&flag_1>4)
		   {
			   HX_flag=1;
			   break;
		   }
		   if(HX_flag)
		   {
			   for(m=blackleft[JD_Hang];m>0;m--)
			   {
				   if(img[sample[JD_Hang]][m]==1)
					{
						count_y++;
					}
			   }
			   for(m=blackright[JD_Hang];m<79;m++)
			   {
				    if(img[sample[JD_Hang]][m]==1)
					{
						count_z++;
					}
			   }
			   if(count_y>count_z)
			   {
				   Y_HX_flag=1;
			   }
			   else if(count_y<count_z)
			   {
				   Z_HX_flag=1;
			   }
			   else 
			   {
				   //预留
			   }
		   }
		   YP_HX_flag=0;       //预判黑线标志位清零
		}		   
	}
}




/*************函数功能********************        
      给出入十字弯和部分斜十字补线
          可优化斜十字处理
          可优化十字上端重新补线
*************3月13号  验证通过 ***********/  
int8 shizhi_flag=0; 
uint8  qishi_flag=0,jiezhi_flag=0;  
uint8 qishi=0,jiezhi=0;
int8  zx=0,yx=0,zs=0,ys=0; 

uint8 h=0;

uint8 xieshizi_zuoflag=0,xieshizi_youflag=0,youxiaozuoxieshizi=0,youxiaoyouxieshizi=0,xieyx=0,xieys=0,xiezs=0,xiezx=0;

uint8  zuoqishi;
void shizi_handle()
{   
    zx=0;yx=0;zs=0;ys=0; 
    qishi_flag=0;
    jiezhi_flag=0; 
    qishi=0;
    jiezhi=0;
    uint8  sample_row_1,sample_row_2,sample_row_3;
    
    uint8  jinru_flag=0,z_flag=0, x_flag=0,zx_flag=0,yx_flag=0;
    int16  aa,bb,cc,dd;
    int8   i;
    int8   zs_flag=0, ys_flag=0;
    
    shizhi_flag=0;  
    for(i=0;i<50;i++)                                    //1
    {
       sample_row_1=sample_row[i];
       sample_row_2=sample_row[i+1];
       sample_row_3=sample_row[i+2];
       if(qishi_flag==0&& (blackleft[i]==z)&&(blackright[i]==r)&&
                   (blackleft[i+1]==z)&&(blackright[i+1]==r)&&
                   (blackleft[i+2]==z)&&(blackright[i+2]==r)&&
                   img[sample_row_1][z]==white&&img[sample_row_1][r]==white&&
                   img[sample_row_2][z]==white&&img[sample_row_2][r]==white)
       {
         qishi=i;                    //数组十字开始行
         qishi_flag=1;
       }
       if(  jiezhi_flag==0&&qishi_flag!=0&&
              blackleft[i]==z &&blackright[i]==r&&
            blackleft[i+1]==z &&blackright[i+1]==r&&
           img[sample_row_1][z]==white&&img[sample_row_1][r]==white&&
           img[sample_row_1][z]==white&&img[sample_row_2][r]==white&&
          (img[sample_row_3][z]==black||img[sample_row_3][r]==black||
            blackright[i+2]<r||blackleft[i+2]>z))
         {                                                                         //其实前面两条就够了，后面这两条为了保险才加的
            jiezhi=i+1;                  //数组十字结束行
            jiezhi_flag=1;
            break;//跳出，省时间
         }   
     }
     if(jiezhi-qishi>3)  shizhi_flag=1;          // 2
     if( shizhi_flag)                            //只有十字才能进入
     {
        for(i=0;i<=qishi+2;i++)                     //找下端拐点
        {         
            aa=blackleft[i+2]-blackleft[i];
            bb=blackleft[i+2]-blackleft[i+4];
            cc=blackright[i+2]-blackright[i];
            dd=blackright[i+2]-blackright[i+4];
            if(aa>=0&&bb>0&&abs_int(bb)>3&&blackleft[i+2]>z)
                zx=i+2;                       //左下
            if(cc<=0&&dd<0&&abs_int(dd)>3&&blackright[i+2]<r)
                yx=i+2;                         //右下     
         }
        //4月29 重新搜线
         if(shizhi_flag)
         {
             uint8 b=0;
              h=0;
             uint8 a=0;
             uint8 m=0;
             for(m=15;m<65;m++)
             {
                   for(uint8 k=qishi;k<52;k++)
                   {
                       uint8 sample_row_4=sample_row[k];
                       uint8 sample_row_5=sample_row[k+1];
                       if(img[sample_row_4][m]==1&&img[sample_row_5][m]==1)
                       {
                            a=k+1;
                       }
                       else break;
                   }
                   if(a>b)
                   {
                              b=a;
                              h=m;
                   }
             }
             for(uint8 k=qishi;k<52;k++)
             {
                uint8 sample_row_4=sample_row[k];
                for(uint8 l=h;l<79;l++)  
                {
                   if(img[sample_row_4][l]==1&&img[sample_row_4][l+1]==0)
                   {
                     blackright[k]=l;
                     break;
                   }
                }
                for(uint8 l=h;l>0 ;l--)
                {
                   if(img[sample_row_4][l]==1&&img[sample_row_4][l-1]==0)
                   {
                     blackleft[k]=l;
                     break;
                   }
                }
                if(blackleft[k]>0&&blackright[k]<79)
                   h=(blackleft[k]+ blackright[k])/2;   
             }
         }
        //
         for(i=jiezhi-2;i<52;i++)                     //找下端拐点
         {  
            if(i<=49)
            { 
              aa=blackleft[i]-blackleft[i-2];
              bb=blackleft[i]-blackleft[i+2];
              cc=blackright[i]-blackright[i-2];
              dd=blackright[i]-blackright[i+2];  
              if(aa>0&&bb<=0&&abs_int(aa)>3&&abs_int(bb)<=2&&blackleft[i]>z)
              { zs=i;zs_flag=1;}                       //左上       
              if(cc<0&&dd>=0&&abs_int(cc)>3&&abs_int(dd)<=2&&blackright[i]<r)
              { ys=i;ys_flag=1;}                      //右上
            }
            if(zs_flag==0&&i>49&&blackleft[49]!=0&&blackleft[50]!=0&&blackleft[51]!=0) //特殊情况
            {
                  zs=max(blackleft[49],blackleft[50],blackleft[51]);          
            }
            if(ys_flag==0&&i>49&&blackright[49]!=r&&blackright[50]!=r&&blackright[51]!=r)
            {
                  ys=max(blackright[49],blackright[50],blackright[51]);              
            }
         }
         if(zx==0&&yx==0&&ys!=0&&zs!=0&&zs>jiezhi&&ys>jiezhi)
                jinru_flag=1;                 //进入十字
         if(zx!=0&&yx!=0&&ys!=0&&zs!=0&&zs>jiezhi&&ys>jiezhi&&zx<qishi&&yx<qishi)
                 z_flag=1;                    //正十字
         if(zx!=0&&yx!=0&&(ys!=0||zs!=0))     //斜十字  
                 x_flag=1;  
         if(zx!=0&&zs!=0&&yx==0)
             zx_flag=1;
         if(ys!=0&&yx!=0&&zx==0)
             yx_flag=1;
     }
     if(zx_flag)
     {
         for(i=zs;i>zx-1;i--)
         {
           blackleft[i-1]=((blackleft[zs]-blackleft[zx-1])*(zs-i))/(zx-1-zs)+blackleft[zs];//注意顺序，上面拐点在左还是在右 
           center[i-1]= blackleft[i-1]+bxiankuan[i-1];       
         }
     }
     if(yx_flag)
     {
         for(i=ys+1;i>yx-1;i--)
         {
           blackright[i-1]=(blackright[yx-1]-blackright[ys+1])*(ys+1-i)/(ys+1-yx+1)+blackright[ys+1];//注意顺序，上面拐点在左还是在右 
           center[i-1]= blackright[i-1]-bxiankuan[i-1];
         } 
     }
     if(z_flag||x_flag)                       //正十字补线   
     {   
         for(i=zs;i>zx-1;i--)
         {
           blackleft[i-1]=((blackleft[zs]-blackleft[zx-1])*(zs-i))/(zx-1-zs)+blackleft[zs];//注意顺序，上面拐点在左还是在右 
         }
         for(i=ys+1;i>yx-1;i--)
         {
           blackright[i-1]=(blackright[yx-1]-blackright[ys+1])*(ys+1-i)/(ys+1-yx+1)+blackright[ys+1];//注意顺序，上面拐点在左还是在右 
         } 
     }
     if(jinru_flag)                           //进入十字补线      
     {
         for(i=zs+1;i>=0;i--)
         {
          // blackleft[i-1]=-blackleft[zs+1]*(zs+1-i)/(zs+1)+blackleft[zs+1];
             blackleft[i-1]=blackleft[zs+1];
         }
         for(i=ys+1;i>=0;i--)
         {
          // blackright[i-1]=(r-blackright[ys+1])*(ys+1-i)/(ys+1)+blackright[ys+1];
             blackright[i-1]=blackright[ys+1];
         } 
     } 
}   //十字补线结束


/*************新识别黑线函数**************************        
       识别黑线
       补线
       减速
*************4月3号  验证通过 ***************/  
uint16  GD_1,GD_2;
int8  y_90_flag=0,Cont=0,Trg=0,ZJ_Process_flag=0,ReadData=0, flag_0=0,flag_0_0;
int8  count_flag_0=0,flag_1=0,flag_2=0,  y_flag=0 , z_flag=0;
void _90_ZJ()
{

//舵机中值：   1482
//左打死  :    1700
//右打死：     1265
   uint8 i,y,k;
   int8  jiezhi_flag=0,jiezhi_hang;
                                             //没有    第一次检测   第二次检测    第三次检测     没有   第二次没有
   ReadData=0x01^heixian_flag_0;   // 0       1            1             1              0      0
   Trg=ReadData&(ReadData^Cont);   // 0       1            0             0              0      0 
   Cont=ReadData;                      // 0       1            1             1              0      0
   if(Trg==1)
   {
        ZJ_Process_flag=0x01^ZJ_Process_flag;         //反转标志位   
   }
   if(ZJ_Process_flag)
   {
     yupan_zy();
     for(i=0;i<50;i++)
     {    
	   sample_row_1	=sample_row[i];
	   sample_row_2	=sample_row[i+1];
       if(y_zj_flag&&(blackleft[i]!=0&&blackright[i]==79&&
          blackleft[i+1]!=0&&blackright[i+1]==79&&
          blackleft[i+2]!=0&&blackright[i+2]==79))      //右直角
       {
            y=i;  
            y_flag=1;                    //判断左右  ，加预判函数  
          if(y==0||y==1||y==2||y==3||y==4)                       //左线存在
          {
              if( jiezhi_flag==0&&img[sample_row_1][79]==1&&img[sample_row_2][79]==0)
			  {
				         jiezhi_hang=i;
						 jiezhi_flag=1;
			  }
          }
		  for(k=y-1;k<jiezhi_hang-5;k++)                             //改变系数即可改变路径
		  {
                center[k]=blackleft[k]+bxiankuan[k]+0;         //改变系数即可改变路径
		  }
       }
     } 
     
     //左直角
     for(i=0;i<50;i++)
     {     
		  sample_row_1	=sample_row[i];
	      sample_row_2	=sample_row[i+1];
          if(z_zj_flag&&(blackleft[i]==0&&blackright[i]!=79&&
            blackleft[i+1]==0&&blackright[i+1]!=79&&
            blackleft[i+2]==0&&blackright[i+2]!=79)）   //直角
          {
               y=i; 
               z_flag=1;                //判断左右  ，加预判函数 
             if(y==0||y==1||y==2||y==3||y==4)       
             {
                if( jiezhi_flag==0&&img[sample_row_1][0]==1&&img[sample_row_2][0]==0)
			    {
				         jiezhi_hang=i;
						 jiezhi_flag=1;
			    }
             }
		     for(k=y-1;k<jiezhi_hang-5;k++)                             //改变系数即可改变路径
		     {
                  center[k]=blackright[k]-bxiankuan[k]-0;         //改变系数即可改变路径
		     }
          }   
       }
     
   }
   if(ZJ_Process_flag&&)
   {
         for(i=0;i<52-2;i++)
		 {

        }
   }
}

void _lvbo()
{
   uint8 i;
   for(i=0;i<52-2;i++)
   {
     blackleft[i]=MiddleFilter (blackleft[i],blackleft[i+1],blackleft[i+2]);
     blackright[i]=MiddleFilter( blackright[i], blackright[i+1], blackright[i+2]);
   } 
}


/*************函数功能**************************        
       主要对90度镜头一侧完全丢线进行补线，
           对远弯进行补线
       忽略近段丢线
       可优化赛道识别
*************3月13号  验证通过 ***************/  
//uint8  z_yi=0,z_zi=0,y_yi=0,y_zi=0;
uint8  youxiaozuobian=0,youxiaoyoubian=0;
int8   jinduan_Y=0,jinduan_Z=0;
uint8  wandao_flag,inwandao_flag,s_zhidao_flag, zong,valid_row;
void  nihe_center()                    //中线拟合
{
    uint8  i,j,m;
    uint8  sample_row_1, sample_row_2, sample_row_3;
    uint8   youbian_Y,zoubian_Z;
       
    jinduan_Y=0;
    jinduan_Z=0;

    youxiaozuobian=0;
    youxiaoyoubian=0;
    uint8  z_yi=0,z_zi=0,y_yi=0,y_zi=0;

    s_zhidao_flag=0;
    inwandao_flag=0;
    wandao_flag=0;
    valid_row=0;
    
    for(i=0;i<52-1;i++)
    {
      sample_row_1=sample_row[i];
      if(center[i]==0)
      {
        if(blackleft[i]>0&&blackright[i]<r&&center[i]==0)
        {
          center[i]=(blackleft[i]+blackright[i])/2;
        }
        else if(blackleft[i]!=0&&blackright[i]>r-2&&center[i]==0)  //右边丢线
        {
           if(y_yi==0)
               y_yi=i;
           if(y_zi==0)
           {
             for(j=y_yi;j<52;j++)
             {
                sample_row_2=sample_row[j];
                sample_row_3=sample_row[j+1];
                if((blackright[j]!=r&&blackright[j+1]!=r&&blackright[j+2]!=r)||
                (img[sample_row_2][blackright[j]-1]==1&&img[sample_row_3][blackright[j+1]-1]==0))
                {
                  y_zi=j;
                  break;
                }
                else
                {
                  y_zi=j;
                }
             }
           }
           if(youxiaozuobian==0)
           {
               for(m=y_yi;m<y_zi+5;m++)//判断左线连续性，并确定不连续行
               {
                  if((blackleft[m+1]-blackleft[m])<0||blackleft[m]>=(r-1))
                  {//本来左线是递增向右的，当遇到左线递减了或跑到右边沿了就进入，
                      youxiaozuobian=m;
                      break;
                  }
                   else if((blackleft[m+1]-blackleft[m])>=0||blackleft[m]<(r-1))
                       youxiaozuobian=m;//递增时一直更新此值
               }
           }
           if(y_yi==0||y_yi==1||(y_yi==2))//如果靠近车，应该是近端丢线，先判断下
           {
             if(y_yi<3&&jinduan_Y==0)
             {
               for(int8 as=y_zi+2;as<52;as++)
               {
                  if((blackright[as+1]-blackright[as])<=0&&blackright[as]<r)
                  {//向上递减 
                     youbian_Y=as;
                     if(youbian_Y>6&&(youbian_Y-y_zi)>6)//验证是否确实是近端丢线  此值应离丢线处不远
                     {
                        for(int8 as=y_zi;as>=0;as--)//能进来，可以确定确实近端丢线了
                        {
                          if(center[as]==0)
                          {
                             jinduan_Y=1;//置位标志  //根据前一行的中线求出该行的中线
                             center[as]=(blackleft[as+1]+blackright[as+1])/2-blackleft[as+1]+blackleft[as]+0;
                             if(center[as]>=r) center[as]=r;
                          }
                        }
                     }
                      if(jinduan_Y==1)  break;//补线完成跳出程序
                  }
                  else break;//若为进入急弯，则会跳出
                }
              }
               if(jinduan_Y==0&&y_yi<=3&&y_zi>=8&&(youxiaozuobian>=y_yi))//如果不是近端丢线，而是进入急弯  不
               {//建议摄像头的镜头在120度以上，90度的容易进入此处
                 inwandao_flag=1;
                 for(int as=0;as<y_zi;as++)
                 {
                    valid_row=y_zi; 
                    if(blackleft[as]!=0)
                    {
                       center[as]=blackleft[as]+bxiankuan[as]+4;//改变系数1的大小，可改善进入急弯的转向能力
                       if(center[as]>=r)
                       { 
                          center[as]=r;
                          valid_row=as;     
                          break;
                       }
                    }
                 }
               } 
           }
            else  if(center[i]==0&&blackleft[i]!=0)//远弯  弯道丢线时 
            {
              center[i]=center[i-2]-blackleft[i-2]+blackleft[i]+1;//改变系数4的大小，可改善进入急弯的转向能力
              if(center[i]>=r)  center[i]=r;
            }
        }
       
        else if(blackleft[i]<2&&blackright[i]!=r&&center[i]==0)  //左边丢线
        {
           if(z_yi==0)
              z_yi=i;
           if(z_zi==0)
           {
             for(j=z_yi;j<52-1;j++)
             {
                sample_row_2=sample_row[j];
                sample_row_3=sample_row[j+1];
                if((blackleft[j]!=0&&blackleft[j+1]!=0&&blackleft[j+2]!=0)||
                img[sample_row_2][blackleft[j]+1]==1&&img[sample_row_3][blackleft[j+1]+1]==0)
                {
                  z_zi=j;
                  break;
                }
                else
                {
                   z_zi=j;
                }
             }
           }
           if(youxiaoyoubian==0)
           {
               for(m=z_yi;m<z_zi+5;m++)//判断左线连续性，并确定不连续行
               {
                  if((blackright[m+1]-blackright[m])>0||blackright[m]<=0)
                  {//本来左线是递增向右的，当遇到左线递减了或跑到右边沿了就进入，
                      youxiaoyoubian=m;
                      break;
                  }
                   else if((blackright[m+1]-blackright[m])<=0||blackright[m]>0)
                       youxiaoyoubian=m;//递增时一直更新此值
               }
           }
           if(z_yi==0||z_yi==1||(z_yi==2))//如果靠近车，应该是近端丢线，先判断下
           {
             if(z_yi<3&&jinduan_Z==0)
             {
               for(uint8 as=z_zi+2;as<52;as++)
               {
                  if((blackleft[as+1]-blackleft[as])>=0&&blackleft[as]>0)
                  {//向上递减 
                     zoubian_Z=as;
                     if(zoubian_Z>6&&(zoubian_Z-z_zi)>6)//验证是否确实是近端丢线  此值应离丢线处不远
                     {
                        for(int as=z_zi;as>=0;as--)//能进来，可以确定确实近端丢线了
                        {
                          if(center[as]==0)
                          {
                             jinduan_Z=1;//置位标志  //根据前一行的中线求出该行的中线
                             center[as]=-(blackright[as+1]-(blackleft[as+1]+blackright[as+1])/2)+blackright[as];
                             if(center[as]<0)  center[as]=0;
                          }
                        }
                     }
                      if(jinduan_Z==1)  break;//补线完成跳出程序
                  }
                  else break;//若为进入急弯，则会跳出
                }
               }
               if(jinduan_Z==0&&z_yi<3&&z_zi>8&&youxiaoyoubian>=z_yi)//如果不是近端丢线，而是进入急弯  不
               {//建议摄像头的镜头在120度以上，90度的容易进入此处
                 inwandao_flag=1;
                 for(int as=0;as<z_zi;as++)
                 {
                    valid_row=z_zi; 
                    if(blackright[as]!=r)
                    {
                       center[as]=blackright[as]-bxiankuan[as]-4;//改变系数1的大小，可改善进入急弯的转向能力
                       if(center[as]<0)
                       {
                          valid_row=as; 
                          center[as]=0;
                          break;
                       }  
                    }
                 }
               } 
            }
            else  if(center[i]==0&&blackright[i]!=r)//远弯  弯道丢线时 
            { 
              center[i]=-(blackright[i-2]-center[i-2])+blackright[i]-1;//改变系数4的大小，可改善进入急弯的转向能力
              if(center[i]<0)  center[i]=0;
            }
        }
       else if(center[i]==0)//其他情况，直接取中值
       {
         center[i]=(blackright[i]+blackleft[i])/2;  
       }
     }
   }
}


/*************函数功能********************        
          有效行的计算还有问题
          
          可优化 
*************3月15号  验证通过 ***********/  

int16 error_3;
void youxiao_hang()
{
  int8  flag_1=0,flag_2=0;
  uint8 i;
  uint8 zuowan,youwan,row ;
  error_3=0;
  bad_flag=0;
  s_zhidao_flag=0;
  
  /*
  for(i=0;i<52-1;i++)
  {
    //if(abs_int(center[i+1]-center[i])>4||center[i+1]==0||center[i+1]==79)
       if(abs_int(center[i+1]-center[i])>5&&abs_int(center[i+2]-center[i])>5)   
       {
          if(center[i+1]<=2||center[i+1]>=78)  
          {
            valid_row_=i-1;
            break;
          }
          valid_row_=i;
          break;
       }
       else valid_row_=i;   
  }
  */
  //*****************************

   //  for(i=0;i<52-2;i++)
    //     center[i]=MiddleFilter(center[i],center[i+1],center[i+2]);
     
       uint8 you_flag=0,zuo_flag=0,j;
       if(center[5]-center[0]>1)
       {
             you_flag=1;
       }
       else  if(center[5]-center[0]<-1)
       {
             zuo_flag=1;
       }
       else  
       {
            for(i=0;i<52-1;i++)
            {
                        //if(abs_int(center[i+1]-center[i])>4||center[i+1]==0||center[i+1]==79)
                 if(abs_int(center[i+1]-center[i])>5&&abs_int(center[i+2]-center[i])>5)   
                 {
                        if(center[i+1]<=2||center[i+1]>=78)  
                        {
                           valid_row_=i-1;
                           break;
                         }
                         valid_row_=i;
                         break;
                  }
                  else valid_row_=i;   
            }
             
       }
       if(you_flag)
       {
           for(j=0;j<52-4;j++)
           {
                
               if(center[j+2]-center[j]>=0)   
               {       
                     valid_row_=j+1;
               }
               else    break;
           }
         
       }
       else if(zuo_flag)   
       {
           for(j=0;j<52-2;j++)
           {
                
               if(center[j+2]-center[j]<=0)   
               {       
                     valid_row_=j+1; 
               }
               else    break ;
           } 
       } 
       else
       {
           //预留
       }
       
  //********************************
  
  error_3=abs_int(center[valid_row_-1]-center[1]);
  if(valid_row_>=45)
  {
     s_zhidao_flag=1;
  }
  if(inwandao_flag)
  {
     valid_row_=valid_row;
  }
  if(valid_row_<8)
  {
     bad_flag=1;
  }
  
  
/*********障碍判断 6月8号*************/
  if(Z_hang>valid_row_)
  {
       Z_Z_Flag=0;
  }
  if(Y_hang>valid_row_)
  {
       Y_Y_Flag=0;
  }
/*********障碍判断 6月8号*************/ 
}

/*********障碍处理 6月8号************* 
       


*********障碍处理 6月8号*************/ 
void  Z_A_deal()
{ 
     int8 i,j,h,m;
     if(Z_Z_Flag)    //左障碍   右线肯定存在
     {
        for(i=blackright[Z_hang]-5;i>0;i--)
        {
          if(img[sample_row[Z_hang]][i]==1&&img[sample_row[Z_hang]][i-1]==0)
          {
            if(blackright[Z_hang]-i<2*bxiankuan[Z_hang]-3)
            {
              blackleft[Z_hang]=i;
              break;
            }
          }
        }
        for(h=0;h<valid_row_;h++)
        {
          center[h]=(blackleft[Z_hang]+blackright[Z_hang])/2+1;  //
        }
     }
     if(Y_Y_Flag)    //右障碍   左线肯定存在
     {
        for(j=blackleft[Y_hang]+5;j<78;j++)
        {
          if(img[sample_row[Y_hang]][j]==1&&img[sample_row[Y_hang]][j+1]==0)
          {
            if(j-blackleft[Y_hang]<2*bxiankuan[Y_hang]-3)
            {
              blackright[Y_hang]=j;
              break;
            }
          }
        }
        for(m=0;m<valid_row_;m++)
        {
          center[m]=(blackleft[Y_hang]+blackright[Y_hang])/2-1;
        }    
     }
}


/////////////////////////////////////////////////////////////////////
void pinghua()
{
  uint8 i;
  for(i=0;i<52-2;i++)
     center[i]=MiddleFilter(center[i],center[i+1],center[i+2]);
}

  float  sum=0.0;
  float CenterEq=0.0;
  float  error=0;
  float error1=0;
  volatile float error_s=0;
void angle_pd()
{
   sum=0.0;
  // CenterEq=0.0;
   int16 xianzhong=0;
     error_s=0;
   uint8  i,j=0,center_value=40;
   for(i=0;i<valid_row_;i++)//滤波后进行加权平均，
   {
        sum+=(center[i]-center_value)*quanzhi[i];
        xianzhong+=quanzhi[i];                  //线重
   }
   error=((center[valid_row_-1]+center[valid_row_-2])/2.0-(center[1]+center[2])/2.0)/(valid_row_*1.0-2.0);    //最远和最近的偏差
 
   CenterEq=sum/xianzhong;                      //得到平均k值  
   error_s=(CenterEq-error1)*10.0;
   error1=CenterEq; 
   if(ZJ_Process_flag)
   {
       angle_pwm=(uint32)(DJ_cen -CenterEq*ZP_1-error*ZD_1- error_s*ZS_1);
   }
   else  if(inwandao_flag)
   {
      angle_pwm=(uint32)(DJ_cen -CenterEq*P_1-error*D_1- error_s*S_1);
   }
   else if (s_zhidao_flag)
   {
      angle_pwm=(uint32)(DJ_cen-CenterEq*P_2-error*D_2- error_s*S_2);
   }
   else if(bad_flag)                   //
   {
      angle_pwm=last_angle;
   }
   else 
   {
      angle_pwm=(uint32)(DJ_cen-CenterEq*P_3-error*D_3- error_s*S_3);
   } 
   last_angle=angle_pwm;                //
}

/*************新画线函数********************        
          画中线
          注意不能全屏显示 ，只能显示分辨率大小的图像中线
          可优化 
          shuzu:要显示的数组
         point_num:要显示点的数量
         rgb565：红色
******************************************************/

void LCD_points (int16 *shuzu,uint32 point_num, uint16 rgb565)                
{
     uint8 i;
     Site_t site[52];
     for(i=0;i<point_num;i++)
     {
        // site[i].x=sample_row[i];
        // site[i].y=shuzu[i];
           site[i].y=sample_row[i];
           site[i].x=shuzu[i];
     }
    while(point_num--)
    {
        LCD_point(site[point_num],rgb565) ;             
    }

}
void LCD_point_s ()          //边线          
{
     uint8 i,point_num=60;
     Site_t site[60];
     for(i=0;i<60;i++)
     {
           site[i].y=i;
           site[i].x=79;
     }
    while(point_num--)
    {
        LCD_point(site[point_num],BLUE) ;             
    }

}
void LCD_point_s_s ()       //边线         
{
     uint8 i,point_num=80;
     Site_t site[80];
     for(i=0;i<80;i++)
     {
           site[i].y=59;
           site[i].x=i;
     }
    while(point_num--)
    {
        LCD_point(site[point_num],BLUE) ;             
    }

}
void DISPLAY()
{
  
    uint32 point_num=52;
                              //写内存
    LCD_point_s ();
    LCD_point_s_s ();
    LCD_points (center, point_num,WHITE);
    LCD_points (blackleft, point_num,BLUE);
    LCD_points (blackright, point_num,BLUE);
   

}
void DISPLAY_1()
{
    uint32 point_num=52;
    LCD_points (center, point_num,RED);        //清屏
    LCD_points (blackleft, point_num,RED);     //清屏
    LCD_points (blackright, point_num,RED);    //清屏
   
      Site_t site = {0,110};   //x = 10 ,y = 20
      LCD_str(site,"y:", BLUE,RED);
      Site_t site_1 = {25,110};   //x = 10 ,y = 20
      LCD_num_BC(site_1,Y_Y_Flag,5, BLUE,RED);
      
      Site_t site_2 = {0,90};  
      LCD_str(site_2,"z:",  BLUE,RED);
      
      Site_t site_2_1 = {25,90};   //x = 10 ,y = 20
      LCD_num_BC(site_2_1,Z_Z_Flag,5,  BLUE,RED);
      
      Site_t site_3 = {0,70};  
      LCD_str(site_3,"H",  BLUE,RED);
      
      Site_t site_3_1 = {25,70};   //x = 10 ,y = 20
      LCD_num_BC(site_3_1,valid_row_,2, BLUE,RED);
 
      Site_t site_4 = {50,90};  
      LCD_str(site_4,"sx:",  BLUE,RED);
      
      Site_t site_4_1 = {75,90};  
      LCD_num_BC(site_4_1,SX_flag,2,  BLUE,RED);

      
      Site_t site_5 = {50,70};  
      LCD_str(site_5,"sd:", BLUE,RED);
      
       Site_t site_5_1 = {75,70};
        LCD_num_BC(site_5_1,SD_flag,2,BLUE,RED);
 
        Site_t site_6 = {50,110};  
        LCD_str(site_6,"ds:",BLUE,RED);
      
        Site_t site_6_1 = {75,110};
        LCD_num_BC(site_6_1,DS_flag,2,BLUE,RED);

}
/**********************************************            
                  主程序



 

************************************************/
void  PROCESS()
{
    DISPLAY_1();                            //清屏
    
    Centerline();
	 
	 HX_ZJ();
    
    if(SX_flag==1&&!ZJ_Process_flag)           shizi_handle();  
    
    _lvbo();
    
    if(SX_flag==1)                                    nihe_center();
    
    _90_ZJ();
    
    pinghua();
     
     
     youxiao_hang();  
      
     Z_A_deal();
    
     DISPLAY();
    
   // if(!ZJ_Process_flag)                      angle_pd();
     angle_pd();
 
}