#include "include.h"
#include "common.h"
#include "math.h"
//#define dawan_speed 49
//dawan_speed ------User_parameter[0];
int dawan_speed=48;
int longs=63;
int dwucha,speed_wucha,zhidao_flag,lastspeed_wucha,lastspeed_wucha1,lastspeed_wucha2,lastspeed_wucha3,last_zhankongbi,mubiao_speed,qvlv_quanju,qulv_jinduan,qulv_yuandaun;
extern int kp,quanjuCount,my_piancha,maichongshu,qulv_jinduan_right,qulv_jinduan_left,qulv_yuandaun_right,qulv_yuandaun_left,qvlv_quanju_right,qvlv_quanju_left;
extern char podao_flag,time,star_lineflag,qulv_point,L,R;
char changzhidao_flag,zhidao_ruwan_flag,zhongwan_flag,dawan_flag=0,xiaowan_flag=0;


void zhengzhuan(int zheng)//正转
{
ftm_pwm_duty(FTM0,FTM_CH5,zheng);  //调制占空比
ftm_pwm_duty(FTM0,FTM_CH4,0); 
}

void fanzhuan(int fan)//反转
{
ftm_pwm_duty(FTM0,FTM_CH5,0);  //调制占空比
ftm_pwm_duty(FTM0,FTM_CH4,fan); 
}



/************************************路段判断***************************************************/
int speed_kp=65,speed_kd=4,zhankongbi,speed_ki=10;/////////////////速度PID参数


void luduan_panduan()//路段判断
{
 
    if(L||R)//障碍
   {
     mubiao_speed=dawan_speed-4;
     dawan_flag=0;
   }
 else
    if(podao_flag&&podao_flag<60)//坡道
{
  dawan_flag=0;
mubiao_speed=dawan_speed;
}
else
  if(zhidao_flag>30&&!podao_flag)//长直道
  { //led(LED0, LED_OFF);//提示关闭
    mubiao_speed=longs;       
//led(LED0, LED_ON);//提示
  }
else
 if(zhidao_flag>15&&zhidao_flag<30&&!podao_flag)//短直道
 {
   //led(LED1, LED_OFF);//提示关闭
    mubiao_speed=dawan_speed+5;
   // led(LED0, LED_ON);
 }
else
 if(qvlv_quanju>10&&qvlv_quanju<=25&&abs(my_piancha)<=18)//小弯
{
mubiao_speed=dawan_speed;
dawan_flag=0;
}
else
 if(qvlv_quanju>25||abs(my_piancha)>18||qulv_point>4)//大弯
{
  zhidao_ruwan_flag=0;
  dawan_flag=1;
mubiao_speed=dawan_speed;
//led(LED1, LED_OFF);//提示关闭
}else
 {
//led(LED0, LED_OFF);//提示关闭
   dawan_flag=0;
mubiao_speed=50;
}
 if(maichongshu>dawan_speed+4&&qulv_yuandaun>8&&qulv_jinduan<5)//高速直道入大弯
 { 
   dawan_flag=0;
   zhidao_ruwan_flag=1;
   mubiao_speed=dawan_speed-3;
 }
}




/***************************************速度控制**************************************************/
void speedcontrol(int speed_now)//速度控制                                                  
{
 int menkan,dut,piancha_speed;
 if(star_lineflag&&maichongshu>3)//起跑线
 {
   dut=4000;
   menkan=0;
kp=8;
 }else
  if(zhidao_ruwan_flag)//直道入弯
     {
      dut=3200;
      menkan=0;
     }
 else
   if(podao_flag)//坡道
   {
     dut=2000;
   menkan=3;
   }
 else
 if(dawan_flag)//大弯
   {
     //led(LED1, LED_OFF);//提示
     dut=1000;
   menkan=10;
   }
   else
   {
   menkan=6;
   dut=1000;
   }
  //lastspeed_wucha=speed_wucha;
 piancha_speed=maichongshu-speed_now;
  speed_wucha=speed_now-maichongshu;// bu
  dwucha=speed_wucha-lastspeed_wucha2;
  zhankongbi=last_zhankongbi+speed_kp*speed_wucha-speed_ki*lastspeed_wucha+speed_kd*dwucha;//pid调速
    if(zhankongbi>7000)
     zhankongbi=7000;
    if(zhankongbi<0)
     zhankongbi=0;
if(piancha_speed>menkan)
{
fanzhuan(dut);
}
else
zhengzhuan(zhankongbi);
  last_zhankongbi=zhankongbi;
  lastspeed_wucha3=lastspeed_wucha2;
  lastspeed_wucha2=lastspeed_wucha1;
  lastspeed_wucha1=lastspeed_wucha;
  lastspeed_wucha=speed_wucha;
}