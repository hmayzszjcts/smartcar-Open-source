#include "common.h"
int danxian_flag_zuo,danxian_flag_you;
int last_danxian_zhongjian,danxian_zuobian,danxian_youbian,danxian_zhongjian;
int danxian_piancha;
int last_youbian8[21],last_zuobian8[21],last_danxian_zuobian_flag,last_danxian_youbian_flag,last_danxian_zuobian,last_danxian_youbian;
int zhijiao_you_lianxu,zhijiao_zuo_lianxu,zhijiaopanding_count,you_cnt,zuo_cnt,zhijiao_cnt_youbian,zhijiao_cnt_zuobian;


int baiheibai_25_flag_zuoyou,baiheibai_25_flag_youzuo=0,baiheibai_30_flag_zuoyou,baiheibai_30_flag_youzuo=0;
int baiheibai_40_flag_zuo,baiheibai_40_flag_you=0,baiheibai_45_flag_zuo,baiheibai_45_flag_you=0;
int baiheibai_40_zuo,baiheibai_40_you,baiheibai_45_zuo,baiheibai_45_you;
int flag_danxian_start;

int heidiangeshu25,heidiangeshu30;


void panduandanshuang(void)
{
int i;
baiheibai_25_flag_zuoyou=0;
baiheibai_25_flag_youzuo=0;
baiheibai_30_flag_zuoyou=0;
baiheibai_30_flag_youzuo=0;

heidiangeshu25=0;
heidiangeshu30=0;

for(i=0;i<79;i++)
{
if(imgyiwei[25][i]==0)
heidiangeshu25++;

if(imgyiwei[30][i]==0)
heidiangeshu30++;
}

for(i=1;i<75;i++)
{
  if((imgyiwei[25][i]-imgyiwei[25][i+1])>0&&imgyiwei[25][i+1]-imgyiwei[25][i+4]<0)
  {
    baiheibai_25_flag_zuoyou=1;
  break;
  }

}
for(i=78;i>5;i--)
{
  if((imgyiwei[25][i]-imgyiwei[25][i-1])>0&&imgyiwei[25][i-1]-imgyiwei[25][i-4]<0)
  {
    baiheibai_25_flag_youzuo=1;
  break;
  }
}
for(i=1;i<75;i++)
{
  if((imgyiwei[30][i]-imgyiwei[30][i+1])>0&&imgyiwei[30][i+1]-imgyiwei[30][i+4]<0)
  {
    baiheibai_30_flag_zuoyou=1;
  break;
  }

}
for(i=78;i>5;i--)
{
  if((imgyiwei[30][i]-imgyiwei[30][i-1])>0&&imgyiwei[30][i-1]-imgyiwei[30][i-4]<0)
  {
    baiheibai_30_flag_youzuo=1;
  break;
  }
}

if(baiheibai_25_flag_zuoyou&&baiheibai_25_flag_youzuo&&baiheibai_30_flag_zuoyou&&baiheibai_30_flag_youzuo&&heidiangeshu25<4&&heidiangeshu30<4)
{flag_danxian_start=1;}
}



void panduanshuangdan(void)
{
  int i;
baiheibai_40_flag_zuo=0;
baiheibai_40_flag_you=0;
baiheibai_45_flag_zuo=0;
baiheibai_45_flag_you=0;
for(i=2;i<78;i++)
{
  if(imgyiwei[40][i]-imgyiwei[40][i+1]<0)
  {
   baiheibai_40_zuo=i;
   baiheibai_40_flag_zuo=1;
   break;
  }
}
if(baiheibai_40_flag_zuo==0)
{
baiheibai_40_zuo=79;
}

 for(i=78;i>2;i--)
{
  if(imgyiwei[40][i]-imgyiwei[40][i-1]<0)
  {
   baiheibai_40_you=i;
   baiheibai_40_flag_you=1;
  break;
  }
}
if(baiheibai_40_flag_you==0)
{
baiheibai_40_you=0;
}


for(i=2;i<78;i++)
{
  if(imgyiwei[45][i]-imgyiwei[45][i+1]<0)
  {
   baiheibai_45_zuo=i;
   baiheibai_45_flag_zuo=1;
   break;
  }
}
if(baiheibai_45_flag_zuo==0)
{
baiheibai_45_zuo=79;
}

 for(i=78;i>2;i--)
{
  if(imgyiwei[45][i]-imgyiwei[45][i-1]<0)
  {
   baiheibai_45_you=i;
   baiheibai_45_flag_you=1;
  break;
  }
}
if(baiheibai_45_flag_you==0)
{
baiheibai_45_you=0;
}


if((baiheibai_40_you-baiheibai_40_zuo)>0&&(baiheibai_45_you-baiheibai_45_zuo)>0)
{
  flag_danxian_start=0;
}





}






void congliangbiansaomiao(void)
{
  int i;
  danxian_flag_zuo=0;
  danxian_flag_you=0;
  for(i=2;i<78;i++)
  {
    if(((imgyiwei[35][i-1]-imgyiwei[35][i])>0)&&((imgyiwei[35][i-2]-imgyiwei[35][i])>0))
    {
      danxian_zuobian=i;
      danxian_flag_zuo=1;
      break;
    }
    if(danxian_flag_zuo==0)
    {
    danxian_zuobian=79;
    }
  }
  for(i=77;i>2;i--)
  {
    if(((imgyiwei[35][i-1]-imgyiwei[35][i])<0)&&((imgyiwei[35][i-2]-imgyiwei[35][i])<0))
    {
      danxian_youbian=i;
      danxian_flag_you=1;
      break;
    }
    if(danxian_flag_you==0)
    {
    danxian_youbian=0;
    }
  
  }
  
  danxian_zhongjian=(int)(((danxian_zuobian+danxian_youbian)/2));
  danxian_piancha=danxian_zhongjian-39;
  
  
}


void danxianpianchachuli(void)
{
  int i;
  danxian_flag_zuo=0;
  danxian_flag_you=0;
  for(i=last_danxian_zhongjian;i<80;i++)
  {
    if(imgyiwei[35][i]==0)
    {
      danxian_youbian=i;
      danxian_flag_you=1;
      break;
    }
  }
  if(danxian_flag_you==0)
  {
    danxian_youbian=79;
  }
  
  for(i=last_danxian_zhongjian;i>0;i--)
  {
    if(imgyiwei[35][i]==0)
    {
      danxian_zuobian=i;
      danxian_flag_zuo=1;
      break;
    }
  }
  if(danxian_flag_zuo==0)
  {
    danxian_zuobian=0;
  }
  
  
  if(danxian_flag_zuo&&danxian_flag_you)
  {
  danxian_zhongjian=(int)(((danxian_zuobian+danxian_youbian)/2));
  danxian_piancha=danxian_zhongjian-39;
  
  last_danxian_zuobian_flag=1;
  last_danxian_youbian_flag=1;
  last_danxian_zuobian=danxian_zuobian;
  last_danxian_youbian=danxian_youbian;
  
  }
  else if(danxian_flag_zuo&&!danxian_flag_you)
  {
  youzhijiao();
  }
  else if(!danxian_flag_zuo&&danxian_flag_you)
  {
  zuozhijiao();
  }
  else if(!danxian_flag_zuo&&!danxian_flag_you)
  {
  danxian_piancha=0;
  last_danxian_zuobian_flag=0;
  last_danxian_youbian_flag=0;
  last_danxian_zuobian=danxian_zuobian;
  last_danxian_youbian=danxian_youbian;
  }
  
  

  last_danxian_zhongjian=danxian_zhongjian;
  zhijiaoshouwei();
}




void zuozhijiao(void)
{
  //if((last_zuobian_flag==1&&last_youbian_flag==1))//&&(zuobian2>95&&zuobian2<115)) 
  
   if((danxian_youbian<last_youbian8[0]+6)&&(danxian_youbian>last_youbian8[0]-6)&&(last_danxian_zuobian_flag==1&&last_danxian_youbian_flag==1)&&((last_danxian_zuobian-danxian_zuobian)>4))//&&(zuobian2>95&&zuobian2<115)) 
           {
             if(zhijiao_you_lianxu>20&&zhijiaopanding_count>50)//速度越快 数值越小
             {
           //   zhijiao_flag_you=1;
             you_cnt=800;//判断确定直角后，打脚800次内不判断右直角   速度越快数值越小 
             zhijiao_cnt_youbian=80;//持续打脚多少次   //速度越快 数值越小
             danxian_piancha=-50;
             
             last_danxian_zuobian_flag=0;
             last_danxian_youbian_flag=1;
             }
             else
             {
             zhijiao_you_lianxu++;
             danxian_zhongjian=(int)(((danxian_zuobian+danxian_youbian)/2));
             if(danxian_zhongjian>79)
                 danxian_zhongjian=79;
             if(danxian_zhongjian<1)
                 danxian_zhongjian=1;          
              danxian_piancha=danxian_zhongjian-39;
          
             last_danxian_zuobian_flag=1;
             last_danxian_youbian_flag=1;
             
             
             }
           }
         
        
           else
           {
           
             
          zhijiao_you_lianxu=0;
           danxian_zhongjian=(int)(((danxian_zuobian+danxian_youbian)/2));
             if(danxian_zhongjian>79)
                 danxian_zhongjian=79;
             if(danxian_zhongjian<1)
                 danxian_zhongjian=1;          
              danxian_piancha=danxian_zhongjian-39;
          
           last_danxian_zuobian_flag=0;
           last_danxian_youbian_flag=1;
           last_danxian_zuobian=danxian_zuobian;
           last_danxian_youbian=danxian_youbian;
           }
  

}
void youzhijiao(void)
{
  if((danxian_zuobian<last_zuobian8[0]+6)&&(danxian_zuobian>last_zuobian8[0]-6)&&(last_danxian_zuobian_flag==1&&last_danxian_youbian_flag==1)&&((danxian_youbian-last_danxian_youbian)>4))//&&(zuobian2>20&&zuobian2<45)) 
           { 
             if(zhijiao_zuo_lianxu>20&&zhijiaopanding_count>50)
             {
           //  zhijiao_flag_zuo=1;
             zuo_cnt=800;
             zhijiao_cnt_zuobian=80;
             danxian_piancha=50;
             last_danxian_zuobian_flag=1;
             last_danxian_youbian_flag=0;
             
             
             }
             else
             {
             zhijiao_zuo_lianxu++;           
             danxian_zhongjian=(int)(((danxian_zuobian+danxian_youbian)/2));
             if(danxian_zhongjian>79)
                 danxian_zhongjian=79;
             if(danxian_zhongjian<1)
                 danxian_zhongjian=1;          
              danxian_piancha=danxian_zhongjian-39;
           
            last_danxian_zuobian_flag=1;
             last_danxian_youbian_flag=1;
             }
           }
         
        
            else{
              
           
              
              zhijiao_zuo_lianxu=0;
             danxian_zhongjian=(int)(((danxian_zuobian+danxian_youbian)/2));
             if(danxian_zhongjian>79)
                 danxian_zhongjian=79;
             if(danxian_zhongjian<1)
                 danxian_zhongjian=1;          
              danxian_piancha=danxian_zhongjian-39;
          
           last_danxian_zuobian_flag=1;
           last_danxian_youbian_flag=0;
           last_danxian_zuobian=danxian_zuobian;
           last_danxian_youbian=danxian_youbian;
            }
       

}

void zhijiaoshouwei(void)
{
       int i;
        if(zhijiao_cnt_zuobian>0&&you_cnt==0)
        {
             danxian_piancha=50;//打脚50次的初始偏差
             danxian_piancha--;//每次打脚越来越小
             if (danxian_piancha==0)//当偏差为0，偏差等于0
                 {
                      danxian_piancha=0;
                  }
              zhijiao_cnt_zuobian--;
        }
        if(zhijiao_cnt_youbian>0&&zuo_cnt==0)
        { 
             danxian_piancha=-50;
             danxian_piancha++;
             if (danxian_piancha==0)
             {
                    danxian_piancha=0;
             }
               zhijiao_cnt_youbian--;}
        
        if(you_cnt>0)
          you_cnt--;
        if(zuo_cnt>0)
          zuo_cnt--;
        
        
        
        last_zuobian8[20]=danxian_zuobian;
        last_youbian8[20]=danxian_youbian;
        for(i=0;i<20;i++)
        {
          last_zuobian8[i]=last_zuobian8[i+1];
          last_youbian8[i]=last_youbian8[i+1];
        }
        
        
        if(danxian_piancha>-6&&danxian_piancha<6)//直道持续判定。
      {
      zhijiaopanding_count++;
      if(zhijiaopanding_count>500)
        zhijiaopanding_count=500;
      }
      else
      {
      zhijiaopanding_count=0;
      }
        
}












