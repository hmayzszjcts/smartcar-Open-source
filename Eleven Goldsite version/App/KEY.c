#include "common.h"
#include "include.h" 

extern struct s3010 spid;
extern struct TSL1401 ccd1;
extern struct motor pidl;
extern struct motor pidr;
extern int16 speed,diff_speed;
extern uint16 STAR_NUMBER;         //开始不检测起跑线标记

/*****************************
      按键处理变量
*******************************/
void KEY_inter(void)
{
  LCD_clear(RED);//清屏
  
  uint8 a = 0;
  
  Site_t site   = { 0,0};   //显示参数标号
  Site_t sit    = {35,0};
  Site_t sitea  = { 0,20};  //显示第一个变量
  Site_t siteb  = { 0,40};  //第二个
  Site_t sitec  = { 0,60};  //第3
  //Site_t sited  = { 0,80};  //第3

  LCD_str(sitea,"1: CCD",RED,BLACK);
  LCD_str(siteb,"2: S3010",RED,BLACK);
  LCD_str(sitec,"3: Motor",RED,BLACK);
  //LCD_str(sited,"4: CCDF",RED,BLACK);
  
  while(key_check(KEY_B) == KEY_UP)
  {
    //上下选择目录，上加下减
    if(key_check(KEY_U) == 0)
    {    
      a++;
      if(a > 3)     //多于参数个数，跳回第一个参数
        a = 1;
    }
    if(key_check(KEY_D) == 0)
    {  
      a--;
      if(a < 1)     //小于0，跳到最后一个参数
        a = 3;
    } 
    DELAY_MS(180); //延时消抖
    //显示当前参量号
    LCD_str(site," a = ",RED,BLACK);
    LCD_num_BC(sit,a,2,RED,BLACK);
    LCD_str(sitea,"1: CCD1",RED,BLACK);
    LCD_str(siteb,"2: S3010",RED,BLACK);
    LCD_str(sitec,"3: Motor",RED,BLACK);
  }
  LCD_clear(RED);//清屏
  if(a == 1)
  {
    KEY_CCD();
  }
  else if(a == 2)
  {
    KEY_S3010();
  }
  else if(a == 3)
  {
    KEY_Motor();
  }
  else if(a == 0)
  {
    LCD_clear(RED);//清屏
    return;
  }
  
  LCD_clear(RED);//清屏
 
}
/********************************
    修改电机PID参数
*********************************/
void KEY_Motor(void)
{
  LCD_clear(RED);//清屏
  
  uint8 b;
  Site_t site   = { 0,0};   //显示参数标号
  Site_t sit    = {35,0};
  Site_t sitea  = { 0,18};  //显示第一个变量
  Site_t sitea1 = {55,18};
  Site_t siteb  = { 0,36};  //第二个
  Site_t siteb1 = {55,36};
  Site_t sitec  = { 0,54};  //第3
  Site_t sitec1 = {55,54};
  Site_t sited  = { 0,72};  //第4
  Site_t sited1 = {55,72};
  Site_t sitee  = { 0,90};  //第5
  Site_t sitee1 = {55,90};
  Site_t sitef  = { 0,108};  //第5
  Site_t sitef1 = {55,108};
  DELAY_MS(500);
  while(key_check(KEY_B) == 1)
  {
   //上下选择参数，上加下减
    if(key_check(KEY_U) == 0)
    {    
      b++;
      if(b > 7)     //多于参数个数，跳回第一个参数
        b = 1;
    }
    if(key_check(KEY_D) == 0)
    {  
      b--;
      if(b < 1)     //小于0，跳到最后一个参数
        b = 7;
    } 
    //左右修改参数，左加右减
    if(key_check(KEY_L) == 0)
    {       
      switch(b)
      {
        case 1: speed += 2;       break;
        case 2: pidl.I += 10;    break;
        case 3: pidl.P += 10;    break;  
        case 4: pidr.I += 10;    break;
        case 5: pidr.P += 10;    break;
        case 6: diff_speed += 1; break;
        case 7: KEY_inter();     break;
        default:break;
      }
    }
    if(key_check(KEY_R) == 0)
    {     
      switch(b)
      {
        case 1: speed -= 2;       break;
        case 2: pidl.I -= 10;    break;
        case 3: pidl.P -= 10;    break;  
        case 4: pidr.I -= 10;    break;
        case 5: pidr.P -= 10;    break;
        case 6: diff_speed -= 1; break;
        case 7: KEY_inter();     break;
        default:break;
      }
    }
    DELAY_MS(180); //延时消抖
    //显示当前参量号
    LCD_str(site," b =",RED,BLACK);
    LCD_num_BC(sit,b,2,RED,BLACK);
    //显示
    LCD_str(sitea,"1:Spd= ",RED,BLACK);
    LCD_num_BC(sitea1,speed,3,RED,BLACK);
    
    //显示
    LCD_str(siteb,"2:L I= ",RED,BLACK);
    LCD_num_BC(siteb1,pidl.I,4,RED,BLACK);
    //显示
    LCD_str(sitec,"3:L P= ",RED,BLACK);
    LCD_num_BC(sitec1,pidl.P,4,RED,BLACK);
    //显示
    LCD_str(sited,"4:R I= ",RED,BLACK);
    LCD_num_BC(sited1,pidr.I,4,RED,BLACK);
    //显示
    LCD_str(sitee,"5:R P= ",RED,BLACK);
    LCD_num_BC(sitee1,pidr.P,4,RED,BLACK);
    
    LCD_str(sitef,"6:dif+sp=",RED,BLACK);
    LCD_num_BC(sitef1,diff_speed,4,RED,BLACK);
  }
  
  LCD_clear(RED);//清屏
}
/********************************
      修改舵机PID参数
*********************************/
extern uint8 qqqqq;
void KEY_S3010(void)
{
  LCD_clear(RED);//清屏
  
  uint8 c;
  Site_t site   = { 0,0};   //显示参数标号
  Site_t sit    = {35,0};
  Site_t sitea  = { 0,20};  //显示第一个变量
  Site_t sitea1 = {55,20};
  Site_t siteb  = { 0,40};  //第二个
  Site_t siteb1 = {55,40};
  Site_t sitec  = { 0,60};  //第二个
  Site_t sitec1 = {55,60};
   Site_t sited  = { 0,80};  //第二个
  Site_t sited1 = {55,80};
  
  DELAY_MS(500);
  while(key_check(KEY_B) == 1)
  {
   //上下选择参数，上加下减
    if(key_check(KEY_U) == 0)
    {    
      c++;
      if(c > 5)     //多于参数个数，跳回第一个参数
        c = 1;
    }
    if(key_check(KEY_D) == 0)
    {  
      c--;
      if(c < 1)     //小于0，跳到最后一个参数
        c = 5;
    } 
    //左右修改参数，左加右减
    if(key_check(KEY_L) == 0)
    {       
      switch(c)
      {
        case 1: spid.P += 2; break;
        case 2: spid.D += 2; break;
        case 3: STAR_NUMBER += 100; break;
        case 4: qqqqq++; break;
        case 5: KEY_inter(); break;
        default: break;
      }
    }
    if(key_check(KEY_R) == 0)
    {     
      switch(c)
      {
        case 1: spid.P -= 2; break;
        case 2: spid.D -= 2; break;
        case 3: STAR_NUMBER -= 100; break;
        case 4: qqqqq--; break;
        case 5: KEY_inter(); break; 
        default: break;
      }
    }
    DELAY_MS(180); //延时消抖
    //显示当前参量号
    LCD_str(site," c =",RED,BLACK);
    LCD_num_BC(sit,c,2,RED,BLACK);
    //显示
    LCD_str(sitea,"1:S P= ",RED,BLACK);
    LCD_num_BC(sitea1,spid.P,3,RED,BLACK);
    //显示
    LCD_str(siteb,"2:S D= ",RED,BLACK);
    LCD_num_BC(siteb1,spid.D,3,RED,BLACK);
    //显示
    LCD_str(sitec,"3:Star=",RED,BLACK);
    LCD_num_BC(sitec1,STAR_NUMBER,4,RED,BLACK);
    
     //显示
    LCD_str(sited,"4:qqqq=",RED,BLACK);
    LCD_num_BC(sited1,qqqqq,2,RED,BLACK);
  }
  
  LCD_clear(RED);//清屏
}
/********************************
        修改CCD1参数
*********************************/
void KEY_CCD(void)
{
  LCD_clear(RED);//清屏
  
  uint8 d;
  Site_t site   = { 0,0};   //显示参数标号
  Site_t sit    = {35,0};
  Site_t sitea  = { 0,20};  //显示第一个变量
  Site_t sitea1 = {55,20};
  Site_t siteb  = { 0,40};  //第二个
  Site_t siteb1 = {55,40};
  DELAY_MS(500);
  while(key_check(KEY_B) == 1)
  {
   //上下选择参数，上加下减
    if(key_check(KEY_U) == 0)
    {    
      d++;
      if(d > 3)     //多于参数个数，跳回第一个参数
        d = 1;
    }
    if(key_check(KEY_D) == 0)
    {  
      d--;
      if(d < 1)     //小于0，跳到最后一个参数
        d = 3;
    } 
    //左右修改参数，左加右减
    if(key_check(KEY_L) == 0)
    {       
      switch(d)
      {
        case 1: ccd1.CCD_Offset++; break;
        case 2: ccd1.CROSS ++; break;
        case 3: KEY_inter(); break;
        default: break;
      }
    }
    if(key_check(KEY_R) == 0)
    {     
      switch(d)
      {
        case 1: ccd1.CCD_Offset --; break;
        case 2: ccd1.CROSS --; break;
        case 3: KEY_inter(); break;
        default: break;
      }
    }
    DELAY_MS(180); //延时消抖
    //显示当前参量号
    LCD_str(site," d =",RED,BLACK);
    LCD_num_BC(sit,d,2,RED,BLACK);
    //显示
    LCD_str(sitea,"1:off= ",RED,BLACK);
    LCD_num_BC(sitea1,ccd1.CCD_Offset,2,RED,BLACK);
    //显示
    LCD_str(siteb,"2:cros=",RED,BLACK);
    LCD_num_BC(siteb1,ccd1.CROSS,2,RED,BLACK);
  }
  
  LCD_clear(RED);//清屏
}