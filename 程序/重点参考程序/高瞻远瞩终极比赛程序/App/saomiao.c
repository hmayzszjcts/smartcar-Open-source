#include "saomiao.h"
#include "common.h"
#include "include.h"
#include "math.h"
int dian_1=27,dongdi;
int shiziduandian=0,shizi_distance=0,shizi_piancha,youshi_kuan_flag,pl,lo,kp=19,dian,my_piancha,kd=60,right_heixian[60],left_heixian[60],xielv,control_point,control_point1;
int currentzhongjian_ave,shizikuandu,dian1,dian2,dian3,jiao,control_point2,xielv_yuan,x,y,right=79,left=0,shizi_count,shizi_flag,angle_sure,angle_maybe,zhongjianpingjun;
extern u8 imgyiwei[60][80];
extern char L,R;
extern int zhidao_flag,currentzhongjian[60],qvlv_quanju,qulv_jinduan,qulv_yuandaun;
int cishu,angle_wan=0,half_zhi[60],dianyuan,lastpiancha_5,lastpiancha_4,lastpiancha_3,lastpiancha_2,lastpiancha_1,duojijiaodu,B,agle_state,half_width=37,qulv_jinduan_right,qulv_jinduan_left,qulv_yuandaun_right,qulv_yuandaun_left,qvlv_quanju_right,qvlv_quanju_left;
char podao_flag,huaxianflag,doudiu_flag,xielv_flag,qulv_point,no_qulv_jinduan,no_qulv_yuandaun,no_qvlv_quanju,zhangai_right,zhangai_left;
static int half_width_group[60]={12,13,13,14,14,15,15,16,16,17, 
17,18,18,19,19,20,20,21,21,22,
22,23,23,24,24,24,25,25,26,26,
27,27,28,28,28,29,29,29,30,30,
31,31,32,32,32,32,33,33,33,33,
34,34,34,35,35,35,36,36,36,36};
/*{
  10,10,11,11,11,12,12,13,14,14,15,15,16,16,17,17,
18,18,19,19,20,20,21,21,22,22,23,24,24,24,25,25,
26,26,27,27,28,28,29,29,30,30,30,31,31,32,32,
33,33,33,34,34,34,35,35,35,36,36,37,37};*/
/*{12 13 13 14 14 15 15 16 16 17 
17 18 18 19 19 20 20 21 21 22 
22 23 23 24 24 24 25 25 26 26 
27 27 28 28 28 29 29 29 30 30 
31 31 31 32 32 32 33 33 33 33 
34 34 34 35 35 35 36 36 36 36};*/
//**********************************************画线函数，用于连接两点间直线***************************************//
void huaxian(int x1,int y1,int x2,int y2)
{
  int i,max,a1,a2,hy;
  a1=x1;
  a2=x2;
  if(a1>a2)
  {max=a1;
  a1=a2;
  a2=max;}
for(i=a1;i<a2;i++)
{

  hy=(i-x1)*(y2-y1)/(x2-x1)+y1;
    currentzhongjian[i]=hy;
}
}
void zhixian_yanshen(int Y1,int X1,int Y2,int X2,int Y3)
{
  int U,HY;
for(U=Y1;U>=Y3;U--)
{
  HY=(U-Y1)*(X2-X1)/(Y2-Y1)+X1;
    currentzhongjian[U]=HY;
}
}
//*****************************************线性回归方程计算斜率函数*********************************************
int regression(int startline,int endline)         
{
  if(endline>55)
    endline=55;
  int i;
  int sumX=0,sumY=0,avrX=0,avrY=0 ;
   int num=0,B_up1=0,B_up2=0,B_up,B_down;
   for(i=startline;i<=endline;i++)
   {
            num++;
            sumX+=i;
            sumY+=currentzhongjian[i];
   }
         avrX=sumX/num;
         avrY=sumY/num;
         B_up=0;
         B_down=0;
         for(i=startline;i<=endline;i++)
        {
         
            B_up1=(int)(currentzhongjian[i]-avrY);
            B_up2=i-avrX;
            B_up+=(int)(10*(B_up1*B_up2));
            B_up=B_up/100*100;
            B_down+=(int)(10*((i-avrX)*(i-avrX)));
         }
   if(B_down==0) 
   B=0;
   else 
   B=B_up*16/B_down;//16
   return B;
}

//****************************************************舵机控制*********************************************//
void servo()//舵机控制函数
{
jiao=(int)(kp*duojijiaodu-kd*(lastpiancha_4-duojijiaodu));
if(jiao>=1000)
jiao=1000;
if(jiao<=-1000)
jiao=-1000;
 ftm_pwm_duty(FTM1,FTM_CH0,4210+jiao); //20
} 

void congzhongjianwangliangbian()
{
  unsigned char diu_left=0,diu_right=0,diu_left1=0,diu_right1=0,pianyou=0,pianzuo=0,zhangai_flag=0,kuan_flag=0,rightbianjie_flag,leftbianjie_flag,rightheixian_flag=0,leftheixian_flag=0,shizi_left_count=0,shizi_right_count=0;
 int my_lastzhongjian=40,currentzhongjianzhi=39,podao_zhi=0,shizi_doudiu=0;
 left_heixian[59]=0;
 right_heixian[59]=79;
 if(currentzhongjian[58])
   my_lastzhongjian= currentzhongjian[59]=currentzhongjian[58];
 else
 currentzhongjian[59]=39;
 control_point=0;
 half_width_group[58]=37;
for(y=58;y>=0;y--)//数据越小越远，越大越近，从近行向远行扫描
{
for(x=my_lastzhongjian;x<=79;x++)//从中间向右扫描
{
if(imgyiwei[y][x]==0)
{
right_heixian[y]=x;
rightheixian_flag=1;
break;
}
}

for(x=my_lastzhongjian;x>=0;x--)//从中间向左扫描
{
if(imgyiwei[y][x]==0)
{
left_heixian[y]=x;
leftheixian_flag=1;
break;
}
}
if(leftheixian_flag&&!rightheixian_flag)//丢了右边
{
  if(y<49&&y>10)
  {
  shizi_right_count++;
  rightbianjie_flag=y;

  }
if(y>48)
 right_heixian[y]=left_heixian[y]+2*half_width_group[y];
else 
right_heixian[y]=right_heixian[y+1]+(abs)(left_heixian[y]-left_heixian[y+1])-1;
}

 if(!leftheixian_flag&&rightheixian_flag)//丢了左边
{
  if(y<49&&y>10)
  {
 shizi_left_count++;
    leftbianjie_flag=y;
  
  }

 if(y>48)
 left_heixian[y]=right_heixian[y]-2*half_width_group[y];
else
left_heixian[y]=left_heixian[y+1]-(abs)(right_heixian[y]-right_heixian[y+1])+1;
}



 if(!leftheixian_flag&&!rightheixian_flag)//都丢了
{
    if(y==40)
   diu_left=1;
     if(y==40)
diu_right=1;
if(y==46)
diu_left1=1;
if(y==46)
diu_right1=1;
    if(y<49&&y>10)
  {
    rightbianjie_flag=y;
    leftbianjie_flag=y;
  shizi_right_count++;
    shizi_left_count++;
    doudiu_flag++;
  }
  if(shizi_flag&&y<49&&y>shizi_doudiu+1)
    shizi_doudiu=y;
    
 left_heixian[y]=0;
  right_heixian[y]=79;
}
shizikuandu=(int)(right_heixian[y]-left_heixian[y]);
//******************************************************中线计算及处理*************************************************//
currentzhongjianzhi=(int)((right_heixian[y]+left_heixian[y])/2);
half_zhi[y]=(int)((right_heixian[y]-left_heixian[y])/2);//一半跑道宽度
currentzhongjian[y]=currentzhongjianzhi;
/******************************************************中线滤波**********************************************************/
if(qvlv_quanju>=10)
{
if(currentzhongjian[y]-currentzhongjian[y+1]>4&&y<42&&xielv>0)
currentzhongjian[y]=currentzhongjian[y+1]+1;
if(currentzhongjian[y]-currentzhongjian[y+1]<-4&&y<42&&xielv<0)
currentzhongjian[y]=currentzhongjian[y+1]-1;
}
/******************************************************十字断点搜索*************************************************/
if(shizi_flag&&y<50&&((right_heixian[y]-right_heixian[y+3])>12||(left_heixian[y]-left_heixian[y+3])<-12))
{
if(y>shiziduandian+1)
shiziduandian=y;
}
/******************************************************曲率计算*************************************************/
if(y>=35)//曲率近端判断
{
  if((currentzhongjian[y]-currentzhongjian[y+1])>0)
qulv_jinduan_right++;
else
if((currentzhongjian[y]-currentzhongjian[y+1])<0)
qulv_jinduan_left++;
no_qulv_jinduan=0;
}
else
if(qulv_point>30)
no_qulv_jinduan=1;

if(y<25&&y>=3)//曲率远端判断
{
  if((currentzhongjian[y]-currentzhongjian[y+1])>0)
qulv_yuandaun_right++;
else
if((currentzhongjian[y]-currentzhongjian[y+1])<0)
qulv_yuandaun_left++;
no_qulv_yuandaun=0;
}
else
if(qulv_point>10)
no_qulv_yuandaun=1;

if(y<55&&y>7)//曲率全局判断
{
  if((currentzhongjian[y]-currentzhongjian[y+1])>0)
qvlv_quanju_right++;
else
if((currentzhongjian[y]-currentzhongjian[y+1])<0)
qvlv_quanju_left++;
no_qvlv_quanju=0;
}
else
if(qulv_point>33)
no_qvlv_quanju=1;

if(currentzhongjian[y]>79)
currentzhongjian[y]=79;
if(currentzhongjian[y]<0)
currentzhongjian[y]=0;
my_lastzhongjian=currentzhongjian[y];
//****************************************************障碍判定****************************************************************//
if(y<52&&y>5&&qvlv_quanju<10&&qulv_point<7&&!shizi_flag&&qulv_jinduan<=4&&dian1<6)
{
   if(kuan_flag<=21&&zhangai_flag<2&&2*half_zhi[y]>2*half_width_group[y]-5)//宽赛道
   {
   kuan_flag++;
  pl+=currentzhongjian[y];
   }else
if(2*half_width_group[y]-2*half_zhi[y]>5&&kuan_flag>17)//赛道变窄
{
zhangai_flag++;
if(zhangai_flag==6)
lo=(currentzhongjian[y]+currentzhongjian[y+1])/2-(currentzhongjian[55]+currentzhongjian[54])/2;
}
 if(zhangai_flag>8&&kuan_flag>17&&2*half_zhi[y]>2*half_width_group[y]-7)//宽赛道
 {
   youshi_kuan_flag++;
 }
 if(youshi_kuan_flag>4)
 {
  //L=lo/zhangai_flag;
  //P=pl/kuan_flag;
  if(lo>2)
  {
    //led(LED0, LED_ON);
    //led(LED1, LED_OFF);
    zhangai_left=1;
    zhangai_right=0;
  }
  else
     if(lo<-2)
     {
      
      //led(LED0, LED_OFF);
    zhangai_right=1;
    zhangai_left=0;
     }
   youshi_kuan_flag=0;
 }
}
//****************************************************坡道判定****************************************************************//
if(!podao_flag&&y<55&&y>10&&qulv_point<20&&qvlv_quanju<7&&qulv_jinduan<6&&abs(my_piancha)<7&&!L&&!R&&pianyou<10&&pianzuo<10)
{
   currentzhongjian_ave=(currentzhongjian[55]+currentzhongjian[56]+currentzhongjian[57]+currentzhongjian[58])/4;
   if(shizikuandu-2*half_width_group[y]<-3)
     podao_zhi++;
//***************************对称性检测******************************//
if(currentzhongjian[y]-currentzhongjian_ave>=3)//偏右
pianyou++;
else
if(currentzhongjian[y]-currentzhongjian_ave<=-3)//偏左
pianzuo++;
}
//****************************************************防止出轨****************************************************************//
rightheixian_flag=0;
leftheixian_flag=0;
if(y<35&&((right_heixian[y+1]-currentzhongjian[y+1]<2&&right_heixian[y]-currentzhongjian[y]<2)||(abs(left_heixian[y+1]-currentzhongjian[y+1])<2&&abs(left_heixian[y]-currentzhongjian[y])<2)))
{
  qulv_point=y;
 if(y>7)
 {
 control_point2=y-7;
 if(control_point2>25)
    control_point=25;
 }            
  else
    control_point2=0;
  if(y>12)
  {
  control_point=y-12;
    if(control_point>35)
    control_point=35;
  }
  else
    control_point=0;
if(y>27)
  {
  control_point1=y-27;
    if(control_point1>25)
    control_point1=25;
  }
  else
    control_point1=0;
break;
}
else
{
  qulv_point=0;
control_point=0;
control_point1=0;
 control_point2=0;
}
}//行扫描结束位置括号行扫描结束位置括号行扫描结束位置括号
//******************************************************坡道确认***********************************************************//
if(!podao_flag&&abs(pianyou-pianzuo)<5&&podao_zhi>=7)//
{
      podao_flag=1;
      //led(LED1, LED_ON);
}
//******************************************************十字判定及补线***********************************************************//
shizi_flag=0;
if(shizi_right_count>=2&&shizi_left_count>=2&&(abs)(rightbianjie_flag-leftbianjie_flag)<9&&doudiu_flag>=2)//十字判定
{
  
doudiu_flag=0;
shizi_flag=1;//十字标志位
xielv_flag=0;
}
else
{
  shiziduandian=0;
cishu=0;
xielv_flag=1; 
//led(LED0, LED_OFF);
}
//*************************************************十字补线******************************************************//

/*if(shizi_flag)//十字补线
{
 // led(LED0, LED_ON);
  if(imgyiwei[9][0]>0&&imgyiwei[9][1]>0&&imgyiwei[9][79]>0&&imgyiwei[9][78]>0)//右白左白
   ;//zhixian_yanshen(57,(currentzhongjian[56]+currentzhongjian[57]+currentzhongjian[58])/3,46,currentzhongjian[46],1);
  else
  if(imgyiwei[9][79]>0&&imgyiwei[9][78]>0&&imgyiwei[9][0]==0&&imgyiwei[9][1]==0||(imgyiwei[9][79]==0&&imgyiwei[9][78]==0&&imgyiwei[9][0]==0&&imgyiwei[9][1]==0))//右白左黑或者左黑右黑
  {
    for(int i=3;i<=70;i++)//从左往右
if(imgyiwei[5][i]==0&&imgyiwei[9][i+1]==0&&imgyiwei[9][i+2]==0&&imgyiwei[9][i+3]>0&&imgyiwei[9][i+4]>0&&imgyiwei[9][i+5]>0&&imgyiwei[9][i+6]>0&&imgyiwei[9][i+7]>0&&imgyiwei[9][i+8]>0&&imgyiwei[9][i+9]>0)//
{
  huaxianflag=i+16;
//shizi_piancha+=currentzhongjian[i]-currentzhongjian[i+1];
  huaxian(9,huaxianflag,56,currentzhongjian[56]);
break;
}
  }else
if(imgyiwei[9][0]>0&&imgyiwei[9][1]>0&&imgyiwei[9][79]==0&&imgyiwei[9][78]==0)//右黑左白
{
for(int oo=79;oo>=10;oo--)//从右往左
if(imgyiwei[9][oo]==0&&imgyiwei[9][oo-1]==0&&imgyiwei[9][oo-2]>0&&imgyiwei[9][oo-3]>0&&imgyiwei[9][oo-4]>0&&imgyiwei[9][oo-5]>0&&imgyiwei[9][oo-6]>0&&imgyiwei[9][oo-7]>0&&imgyiwei[9][oo-8]>0)
{
  huaxianflag=oo-16;
  huaxian(9,huaxianflag,56,currentzhongjian[56]);
  break;
}
}
}*/
if(shizi_flag)//十字补线
{
  if((!diu_left||!diu_right)&&(!diu_left1||!diu_right1))
  //if(abs(currentzhongjian[50]-currentzhongjian[52])<3)
  {
zhixian_yanshen(55,(currentzhongjian[54]+currentzhongjian[55]+currentzhongjian[56])/3,49,currentzhongjian[49],1);
 //xu=currentzhongjian[3];
  }else// if(diu_left[48]&&diu_right[48])
    //if(cishu>=4)
huaxian(5,currentzhongjian[5],58,currentzhongjian[58]);
}
//************************************************曲率计算******************************************************//
shizi_distance=shizi_piancha=0;
qvlv_quanju=abs(qvlv_quanju_right-qvlv_quanju_left);//曲率全局计算
qulv_jinduan=abs(qulv_jinduan_right-qulv_jinduan_left);//曲率近端计算
qulv_yuandaun=abs(qulv_yuandaun_right-qulv_yuandaun_left);//曲率远端计算
qvlv_quanju_right=qvlv_quanju_left=qulv_jinduan_right=qulv_jinduan_left=qulv_yuandaun_right=qulv_yuandaun_left=0;
}
//*************************************************偏差处理******************************************************//
void pianchachuli()
{ 
  dian1=abs((currentzhongjian[48]+currentzhongjian[49]+currentzhongjian[50])/3-39);
  dian2=abs((currentzhongjian[40]+currentzhongjian[41]+currentzhongjian[42])/3-39);
  dian3=currentzhongjian[35]-39;
  dongdi=dian_1-0;
  dian=(1*currentzhongjian[dongdi+control_point1])-39;//+0.3*currentzhongjian[32+control_point1-3]+0.3*currentzhongjian[30+control_point1-1])-39;
//if(shizi_flag)
//dian=1.6*dian3;
  if(L)//左边有障碍
  {
dian+=6;
xielv=0;
  }
else
if(R)//右边有障碍
{
xielv=0;
dian-=6;
}
my_piancha=(int)(1*dian+0.9*xielv);//0.9  1  1.2   0.4
xielv=0;
lastpiancha_5=lastpiancha_4;
lastpiancha_4=lastpiancha_3;
lastpiancha_3=lastpiancha_2;
lastpiancha_2=lastpiancha_1;
lastpiancha_1=duojijiaodu;
duojijiaodu=my_piancha;
}