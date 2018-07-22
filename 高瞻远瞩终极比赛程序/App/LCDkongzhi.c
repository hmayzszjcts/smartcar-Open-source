#include "common.h"
#include "include.h"
/**********************LCD功能标志位******************************/
uint8 flag_LCD=1;                         //LCD界面标志
uint8 flag_LCD_revise=0;                  //resive参数修改选择
uint8 flag_LCD_queren=0;                  //确认标志位

uint8 flag_LCD_first=1;                      //first界面
unsigned char select_tm[10]={               //液晶10个图标选择标志
0,0,0,0,0,0,0,0,0,0,}; 
unsigned char flag_tubiao_viewm[10]={     //液晶10个图标显示标志
0,0,0,0,0,0,0,0,0,0,};
unsigned int flag_gundongs[10]={          //液晶10个图标滚动显示标志
1,1,1,1,1,1,1,1,1,1,};
uint8 flag_selectm=0;                     //液晶选择标志
uint8 flag_gundongm=1;                    //液晶返回滚动标志

uint flag_blueteethopen=0;                  //蓝牙小图标显示标志
uint8 flag_stop=1;                           //停车小图标标志，包括停车是否控制标志


int keyceshi;
int DCsignal[4];
s16 piancha_lcd;

//uint8 imgErwei[CAMERA_W][CAMERA_H];
//uint8 img[CAMERA_W*CAMERA_H]; 
//uint8 imgyiwei[CAMERA_W][CAMERA_H]; 
uint8 imgyiwei[60][80]; 
CCDxianshi CCD_LIVE_WATCH;   
//int speed_level;
char speed_level_lcd[4];

struct                                     //第九届液晶显示各个参数结构体
{
long int zhidao_speed;
long int xiaowan_speed;
long int xiaowan_chasu;
long int jiwan_speed;
long int jiwan_chasu;
long int podao_speed;

long int stop_miaoshu;
long int zhangai_panduan;

long int ZA_flag;
long int DUOJI_kp;

long int houpai_flag;

 char zhidao_speed_lcd[5];
 char xiaowan_speed_lcd[5];
 char xiaowan_chasu_lcd[5];
 char jiwan_speed_lcd[5];
 char jiwan_chasu_lcd[5];
 char podao_speed_lcd[5];
 
 char stop_miaoshu_lcd[5];
 char zhangai_panduan_lcd[5];
 char ZA_flag_lcd[5];
 char DUOJI_kp_lcd[5];
 char houpai_flag_lcd[5];
}dijiujiecanshu;


struct                                     //第十一届液晶显示各个参数结构体
{
int QWspeed;
char QWspeed_lcd[5];

int MMA7361_ZZ;
int GYRO_X_ZZ;
int GYRO_Y_ZZ;

char MMA7361_ZZ_lcd[5];
char GYRO_X_ZZ_lcd[5];
char GYRO_Y_ZZ_lcd[5];

}DSYJcanshu;

  
int MMA7361_sj;
int GYRO_X;
int GYRO_Y;

char MMA7361_sj_lcd[5];
char GYRO_X_lcd[5];
char GYRO_Y_lcd[5];

char user_p0[5];
char user_p1[5];
char user_p2[5];
char user_p3[5];
char user_p4[5];
char user_p5[5];
char user_p6[5];
char user_p7[5];
char user_p8[5];
char user_p9[5];
char user_p10[5];
char user_p11[5];



unsigned char F8x16tbm[]={
0x00,0x00,0x04,0x02,0xFF,0x02,0x04,0x00,0x00,0x20,0x40,0xFF,0x40,0x20,0x00,0x00,/*"未命名文件",0*/
0x00,0x08,0x1C,0x2A,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x2A,0x1C,0x08,0x00,/*"未命名文件",1*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"clear",2*/

};
/*************************END************************/



signed char position[]="    ";        //OLED液晶显示 偏差 电感0，1，2，3动态数据显示字符缓存
signed char dcxhsz0[]="    ";
signed char dcxhsz1[]="    ";
signed char dcxhsz2[]="    ";
signed char dcxhsz3[]="    ";
signed char leftmoto_lcd[]="    ";
signed char rightmoto_lcd[]="    ";

signed char youxiaohangmoduan[]="    ";
signed char youxiaohangxiayihangzhongxian[]="    ";
signed char youxiaohangxiayihan_zuobian[]="    ";
signed char youxiaohangxiayihan_youbian[]="    ";
signed char junzhipiancha_lcd[]="    ";
signed char zuo52hang[]="    ";
signed char you52hang[]="    ";
signed char zhong52hang[]="    ";
signed char last52zuo[]="    ";
signed char last52you[]="    ";

void DMX_chengxuchushihua()
{
    speed_level=2;
    OLED_Init();
    key_Init();
    ZNC_DATA_Init();
    LCDcanshuchushihua();
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//第一步 把你想要写进液晶屏幕的参数 在这里赋值初始化
void LCDcanshuchushihua()//掉到按键
{
dawan_speed=User_parameter[0];
kp=User_parameter[1];//初始化时候 把
dian_1=User_parameter[2];
xielv_dian=User_parameter[3];
longs=User_parameter[4];
yunsuflag=User_parameter[5];
//DSYJcanshu.MMA7361_ZZ=MMA7361_vertical;
//DSYJcanshu.GYRO_X_ZZ=GYRO_VAL;
//DSYJcanshu.GYRO_Y_ZZ=ZX_MID_G;
}
void DSYJ_canshugengxin()//按键返回
{
User_parameter[0]=dawan_speed;
User_parameter[1]=kp;
User_parameter[2]=dian_1;
User_parameter[3]=xielv_dian;
User_parameter[4]=longs;
User_parameter[5]=yunsuflag;
//MMA7361_vertical=DSYJcanshu.MMA7361_ZZ;
//GYRO_VAL=DSYJcanshu.GYRO_X_ZZ;
//ZX_MID_G=DSYJcanshu.GYRO_Y_ZZ;
}


void LCD_view(void)
{
         if(flag_LCD_first) 
            {
              zhujiemian();
               
               //ceshi2(); 
            }
         else if(flag_tubiao_viewm[0])         //check
            {
               //ceshi_check();
               //dijiujie_speed_tiaoshi();
               
              
              dijiujie_speed_tiaoshi();
              
              ZNC_DATA_Update();
              
               flag_gundongs[0]=gundong_128(flag_gundongs[0]);
               flag_tubiao_viewm[0]=Backtom(flag_tubiao_viewm[0],0);
            }
         else if(flag_tubiao_viewm[1])        //修改参数后期想修改什么参数就修改什么参数
            {
               //ceshi_revise();
              
              dijiujie_speed_tiaoshi2();
              
              ZNC_DATA_Update();
              
               flag_gundongs[1]=gundong_128(flag_gundongs[1]);
               flag_tubiao_viewm[1]=Backtom(flag_tubiao_viewm[1],1); 
            }
         else if(flag_tubiao_viewm[2])          //锁屏
            {
                //ceshi_lock();
                xiaoche_lock(); 
                //flag_LCD=0;
                flag_gundongs[2]=gundong_128(flag_gundongs[2]);
                flag_tubiao_viewm[2]=Backtom(flag_tubiao_viewm[2],2); 
                
                
                
                //参数赋给数组
               
                // while(flash_erase_sector(200)!=0);
	  	//while(flash_write_buf(200,  0,sizeof(Flash_canshu),(u8 *)Flash_canshu)!=0);
           //     SetMototVoltage(0,0);
              //  delayms(2000);
            }
         else if(flag_tubiao_viewm[3])            //AD波形显示
            {
                //ceshi_boxing();
                //ceshi_tuxiangxianshi();
            //  DSYJzhilishujuxianshi();
              //  VIEW_CCD_dongtai(Pixel);
                flag_gundongs[3]=gundong_128(flag_gundongs[3]);
                flag_tubiao_viewm[3]=Backtom(flag_tubiao_viewm[3],3); 
            }
         else if(flag_tubiao_viewm[4])               //偏差拟合显示
            {
   //             ceshi_piancha();
              //  VIEW_CCD_dongtai(CCD_erzhihua);
                flag_gundongs[4]=gundong_128(flag_gundongs[4]);
                flag_tubiao_viewm[4]=Backtom(flag_tubiao_viewm[4],4); 
             }
         else if(flag_tubiao_viewm[5])                //AD直方图显示
            {
                //ceshi_zhifangtu();
                flag_gundongs[5]=gundong_128(flag_gundongs[5]);
                flag_tubiao_viewm[5]=Backtom(flag_tubiao_viewm[5],5);  
             }
         else if(flag_tubiao_viewm[6])                  //蓝牙开启关闭
             {       
                //ceshi_blueteeth();
                flag_gundongs[6]=gundong_128(flag_gundongs[6]);
                flag_tubiao_viewm[6]=Backtom(flag_tubiao_viewm[6],6);
             }
         else if(flag_tubiao_viewm[7])                  //事实脉冲反馈
             {
                // ceshi_speedpulse();
                 flag_gundongs[7]=gundong_128(flag_gundongs[7]);
                 flag_tubiao_viewm[7]=Backtom(flag_tubiao_viewm[7],7); 
             }
         else if(flag_tubiao_viewm[8])                    //速度档位
             {
               // ceshi_speedlevel();
                ceshi_speedlevel();
                flag_gundongs[8]=gundong_128(flag_gundongs[8]);
                flag_tubiao_viewm[8]=Backtom(flag_tubiao_viewm[8],8);  
             }
         else if(flag_tubiao_viewm[9])                     //加停车标志
             {
                ceshi_stop();
                flag_gundongs[9]=gundong_128(flag_gundongs[9]); 
                flag_tubiao_viewm[9]=Backtom(flag_tubiao_viewm[9],9);
             }
        // flash_canshu_shuaxin();//flash刷新
        // speed_max=DSYJcanshu.QWspeed;
         DSYJ_canshugengxin();
         
}





void ceshi_tuxiangxianshi(void)
{
 long int ll1;
 unsigned char ll0;
  int x_img,y_img;
clear_drawpottempm();
//Draw_potL(1,1,1,1);   //二维图像显示
//Draw_potL(128,64,0,1);

for(ll1=0;ll1<60;ll1++)
 {
  for(ll0=0;ll0<80;ll0++)
  {
   if(imgyiwei[ll1][ll0]==0)
      Draw_potL(ll0,ll1,0,1);
    else
      Draw_potL(ll0,ll1,1,1);
  }
 }

#if 1 
for(ll0=0;ll0<60;ll0++)
   {
//      Draw_potL((SXTyoubian[ll0]-2),ll0,0,1);//有一个1的整体偏移 多一个-1
//      Draw_potL((SXTzuobian[ll0]+4),ll0,0,1);//有一个1的整体偏移 多一个-1
   //   Draw_potL((SXTzhongjian[ll0]),ll0,0,1);
      
    
     // Draw_potL(39,ll0,0,1);//期望中线
      
   }
for(ll0=0;ll0<80;ll0++)
{
//Draw_potL(ll0,diyitiao_tiaobianhang,0,1);
//Draw_potL(ll0,shuangxianbiandanxian,0,1);

//Draw_potL(ll0,danxianjinshuangxian_diyihang,0,1);
//Draw_potL(ll0,diyitiao_tiaobianhang-5,0,1);
//Draw_potL(ll0,diyitiao_tiaobianhang-8,0,1);
}

#endif


  //      countprint(g_nLeftMotorPulse,youxiaohangmoduan);
    LCD_6x8StrtoBMPL(97,1,youxiaohangmoduan,1);
    
 //       countprint(g_nRightMotorPulse,youxiaohangxiayihangzhongxian);
    LCD_6x8StrtoBMPL(97,9,youxiaohangxiayihangzhongxian,1);
    
    
 //       countprint(youxiaohangxiayihang_zuobian,youxiaohangxiayihan_zuobian);
    LCD_6x8StrtoBMPL(97,17,youxiaohangxiayihan_zuobian,1);
    
 ///       countprint(youxiaohangxiayihang_youbian,youxiaohangxiayihan_zuobian);
    LCD_6x8StrtoBMPL(97,25,youxiaohangxiayihan_zuobian,1);
    

LCD_PXXxYYCHtoBMPL(128,64,DRAWPOTtempm,0,0,0,0,1);
}

/*void ceshi_tuxiangxianshi(void)
{
 long int ll1;
 unsigned char ll0;
  int x_img,y_img;
clear_drawpottempm();
//Draw_potL(1,1,1,1);   //二维图像显示
//Draw_potL(128,64,0,1);

for(ll1=0;ll1<60;ll1++)
 {
  for(ll0=0;ll0<80;ll0++)
  {
   if(imgyiwei[ll0][ll1]==0)
      Draw_potL(ll0,ll1,0,1);
    else
      Draw_potL(ll0,ll1,1,1);
  }
 }
  /*for(ll1=0;ll1<4800;ll1++)
   {
  
   x_img=((ll1)%80)+1;
   y_img=((ll1)/80)+1;
   if(img[ll1]==0)
      Draw_potL(x_img,y_img,0,1);//黑色是0
    else
      Draw_potL(x_img,y_img,1,1);
  
   }*/
 /*for(ll0=0;ll0<60;ll0++)
   {
      Draw_potL((SXTyoubian[ll0]-2),ll0,0,1);//有一个1的整体偏移 多一个-1
      Draw_potL((SXTzuobian[ll0]+4),ll0,0,1);//有一个1的整体偏移 多一个-1
      Draw_potL((SXTzhongjian[ll0]),ll0,0,1);
    
     // Draw_potL(39,ll0,0,1);//期望中线
      
   }

    countprint(zhongxiantingzhihang,youxiaohangmoduan);
    LCD_6x8StrtoBMPL(97,1,youxiaohangmoduan,1);
    
        countprint(youxiaohangxiayihang_zhongxian,youxiaohangxiayihangzhongxian);
    LCD_6x8StrtoBMPL(97,9,youxiaohangxiayihangzhongxian,1);
    
       countprint(youxiaohangxiayihang_zuobian,youxiaohangxiayihan_zuobian);
    LCD_6x8StrtoBMPL(97,17,youxiaohangxiayihan_zuobian,1);
    
        countprint(youxiaohangxiayihang_youbian,youxiaohangxiayihan_youbian);
    LCD_6x8StrtoBMPL(97,25,youxiaohangxiayihan_youbian,1);



LCD_PXXxYYCHtoBMPL(128,64,DRAWPOTtempm,0,0,0,0,1);
}*/

void zhujiemian(void) 

{
int ds=0;
//key2=gpio_get(PORTB,2);
//key3=gpio_get(PORTA,29);



keyup_=keyUp;

if(!keyup_) //key2   up
             {
               systick_delay_ms(10);
                keyup_=keyUp;
                if(!keyup_)
                {  keyup_=keyUp;  
                   while(!keyup_)
                   {keyup_=keyUp;}; //key2
                    if(flag_selectm>0)
                         flag_selectm--;
                    else 
                        flag_selectm=9;
                    
                }
            }
keydown_=keyDown;

if(!keydown_) //down  key3
             {
                systick_delay_ms(10);
                keydown_=keyDown;
                if(!keydown_)
                {   keydown_=keyDown; 
                    while(!keydown_)
                    {keydown_=keyDown;};
                    if(flag_selectm<9)
                         flag_selectm++;
                    else 
                         flag_selectm=0;
                    
                }
            }

for(ds=0;ds<10;ds++) 
{
if(flag_selectm==ds)
select_tm[ds]=1;
else
select_tm[ds]=0;
}

clear_drawpottempm();


diyihangview();


LCD_PXXxYYCHtoBMPL(24,24,F24x24,2,16,0,select_tm[0],flag_gundongm);
LCD_PXXxYYCHtoBMPL(24,24,F24x24,27,16,1,select_tm[1],flag_gundongm);
LCD_PXXxYYCHtoBMPL(24,24,F24x24,52,16,2,select_tm[2],flag_gundongm);
LCD_PXXxYYCHtoBMPL(24,24,F24x24,77,16,3,select_tm[3],flag_gundongm);
LCD_PXXxYYCHtoBMPL(24,24,F24x24,102,16,4,select_tm[4],flag_gundongm);

LCD_PXXxYYCHtoBMPL(24,24,F24x24,2,40,5,select_tm[5],flag_gundongm);
LCD_PXXxYYCHtoBMPL(24,24,F24x24,27,40,6,select_tm[6],flag_gundongm);
LCD_PXXxYYCHtoBMPL(24,24,F24x24,52,40,7,select_tm[7],flag_gundongm);
LCD_PXXxYYCHtoBMPL(24,24,F24x24,77,40,8,select_tm[8],flag_gundongm);
LCD_PXXxYYCHtoBMPL(24,24,F24x24,102,40,9,select_tm[9],flag_gundongm);

flag_gundongm=gundong_128(flag_gundongm);

keyenter_=keyEnter;
if(!keyenter_) 
             {
                systick_delay_ms(20);
                keyenter_=keyEnter;
                if(!keyenter_)
                {   keyenter_=keyEnter; 
                    while(!keyenter_)
                    {keyenter_=keyEnter;};
                             flag_LCD_first=0;
                    switch(flag_selectm) 
                    {
                      case 0:flag_tubiao_viewm[0]=1;break;
                      case 1:flag_tubiao_viewm[1]=1;break;
                      case 2:flag_tubiao_viewm[2]=1;break;
                      case 3:flag_tubiao_viewm[3]=1;break;
                      case 4:flag_tubiao_viewm[4]=1;break;
                      case 5:flag_tubiao_viewm[5]=1;break;
                      case 6:flag_tubiao_viewm[6]=1;break;
                      case 7:flag_tubiao_viewm[7]=1;break;
                      case 8:flag_tubiao_viewm[8]=1;break;
                      case 9:flag_tubiao_viewm[9]=1;break;
                    }
                 }
                    
              }
 


  
 
}


void DSYJzhilishujuxianshi()
{
clear_drawpottempm();
diyihangview();
/*
int MMA7361_sj;
int GYRO_X;
int GYRO_Y;
*/
countprint(MMA7361_sj,MMA7361_sj_lcd);
countprint(GYRO_X,GYRO_X_lcd);
countprint(GYRO_Y,GYRO_Y_lcd);

LCD_6x8StrtoBMPL(1*16+12*6,2*8,MMA7361_sj_lcd,1);
LCD_6x8StrtoBMPL(1*16+12*6,3*8,GYRO_X_lcd,1);
LCD_6x8StrtoBMPL(1*16+12*6,4*8,GYRO_Y_lcd,1);


LCD_6x8StrtoBMPL(1*16,2*8,"MMA7361:",1);
LCD_6x8StrtoBMPL(1*16,3*8,"GYRO_X:",1);
LCD_6x8StrtoBMPL(1*16,4*8,"GYRO_Y:",1);
//LCD_6x8StrtoBMPL(1*16,5*8,"jiwanspeed:",1);
//LCD_6x8StrtoBMPL(1*16,6*8,"jiwanchasu:",1);
//LCD_6x8StrtoBMPL(1*16,7*8,"podaospeed:",1);





}



void ceshi_piancha(void)
{
    s16 i;
  //  piancha_lcd=-current_deviation;
    clear_drawpottempm();

    diyihangview();

    DrawLineX_fL(6,122,40,1);



    DrawLineY_fL(28,37,6+58+piancha_lcd,1);
    Draw_potL(6+57+piancha_lcd,36,1,1);
    Draw_potL(6+59+piancha_lcd,36,1,1);
    Draw_potL(6+57+piancha_lcd,35,1,1);
    Draw_potL(6+59+piancha_lcd,35,1,1);
    Draw_potL(6+56+piancha_lcd,35,1,1);
    Draw_potL(6+60+piancha_lcd,35,1,1);




    DrawLineY_fL(41,45,6+58,1);
    DrawLineY_fL(41,43,6+58+29,1);
    DrawLineY_fL(41,43,6+29,1);
    DrawLineY_fL(41,45,6,1);
    DrawLineY_fL(41,45,6+116,1);







    countprint(DCsignal[0],dcxhsz0);
   // countprint(DCsignal[1],dcxhsz1);
   // countprint(DCsignal[2],dcxhsz2);
    countprint(DCsignal[3],dcxhsz3);
//        countprint(g_nLeftMotorPulseSigma+2000,leftmoto_lcd);
   // countprint(DCsignal[1],dcxhsz1);
   // countprint(DCsignal[2],dcxhsz2);
//    countprint(g_nRightMotorPulseSigma+2000,rightmoto_lcd);
    
    countprint(piancha_lcd,position);

    
    
    
if(flag_gundongs[4]) 
  {
    LCD_6x8StrtoBMPL(6,16,dcxhsz0,1);
  //  LCD_6x8StrtoBMPL(30,16,dcxhsz1,1);
  //  LCD_6x8StrtoBMPL(79,16,dcxhsz2,1);
    LCD_6x8StrtoBMPL(103,16,dcxhsz3,1);
    
LCD_6x8StrtoBMPL(6,16+8+32,rightmoto_lcd,1);
LCD_6x8StrtoBMPL(103,16+8+32,leftmoto_lcd,1);
    
    
    
    LCD_P8x16StrtoBMPL(52,48,position,1);



    for(i=0;i<128;i++) 
      {
         Draw_potL(i,28,0,1);
         Draw_potL(i,29,0,1);
         Draw_potL(i,30,0,1);
         Draw_potL(i,31,0,1);
         Draw_potL(i,32,0,1);
         Draw_potL(i,33,0,1);
         Draw_potL(i,34,0,1);
         Draw_potL(i,35,0,1);
         Draw_potL(i,36,0,1);
         Draw_potL(i,37,0,1);
       }
     DrawLineY_fL(28,37,6+58+piancha_lcd,1); //箭头
     Draw_potL(6+57+piancha_lcd,36,1,1);
     Draw_potL(6+59+piancha_lcd,36,1,1);
     Draw_potL(6+57+piancha_lcd,35,1,1);
     Draw_potL(6+59+piancha_lcd,35,1,1);
     Draw_potL(6+56+piancha_lcd,35,1,1);
     Draw_potL(6+60+piancha_lcd,35,1,1);
   }
else 
   {
     LCD_6x8StrtoBMPL(6,16,dcxhsz0,0);
    // LCD_6x8StrtoBMPL(30,16,dcxhsz1,0);
   //  LCD_6x8StrtoBMPL(79,16,dcxhsz2,0);
     LCD_6x8StrtoBMPL(103,16,dcxhsz3,0);
     
     LCD_6x8StrtoBMPL(6,16+8+32,rightmoto_lcd,0);
LCD_6x8StrtoBMPL(103,16+8+32,leftmoto_lcd,0);

     LCD_P8x16StrtoBMPL(52,48,position,0);


     for(i=0;i<128;i++) 
     {
       Draw_potL(i,28,0,0);
       Draw_potL(i,29,0,0);
       Draw_potL(i,30,0,0);
       Draw_potL(i,31,0,0);
       Draw_potL(i,32,0,0);
       Draw_potL(i,33,0,0);
       Draw_potL(i,34,0,0);
       Draw_potL(i,35,0,0);
       Draw_potL(i,36,0,0);
       Draw_potL(i,37,0,0);
     }
     DrawLineY_fL(28,37,6+58+piancha_lcd,0); //箭头
     Draw_potL(6+57+piancha_lcd,36,1,0);
     Draw_potL(6+59+piancha_lcd,36,1,0);
     Draw_potL(6+57+piancha_lcd,35,1,0);
     Draw_potL(6+59+piancha_lcd,35,1,0);
     Draw_potL(6+56+piancha_lcd,35,1,0);
     Draw_potL(6+60+piancha_lcd,35,1,0);
  }
  LCD_6x8StrtoBMPL(0,48,"-30cm",1);
  LCD_6x8StrtoBMPL(103,48,"30cm",1);
}

void xiaoche_lock(void) 
{
 flag_LCD=0;
 clear_drawpottempm();
 
 LCD_PXXxYYCHtoBMPL(128,64,BMP3,0,0,0,0,1);
  //Draw_BMP(0,0,128,8,BMP3);
 
 //LCD_P8x16StrtoBMPL(1*16,2*8,"Screen lock!",1);     //字符大小XX*YY,对应大小的数组， x坐标，y坐标，第几个数或者字符，阴文/阳文,写入哪个GRAM
 //LCD_P8x16StrtoBMPL(1*16,4*8,"The car run!",1);
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void diyihangview(void) 
{
if(flag_blueteethopen)
LCD_PXXxYYCHtoBMPL(16,16,F16x16tbm,0,0,0,0,1);
else
LCD_PXXxYYCHtoBMPL(16,16,F16x16tbm,0,0,4,0,1);


 if(flag_stop)
LCD_PXXxYYCHtoBMPL(16,16,F16x16tbm,16*7,0,7,0,1);
else
LCD_PXXxYYCHtoBMPL(16,16,F16x16tbm,16*7,0,2,0,1);

LCD_PXXxYYCHtoBMPL(16,16,F16x16tbm,16*2,0,8,0,1);
LCD_PXXxYYCHtoBMPL(16,16,F16x16tbm,16*3,0,9,0,1);
LCD_PXXxYYCHtoBMPL(16,16,F16x16tbm,16*4,0,10,0,1);
LCD_PXXxYYCHtoBMPL(16,16,F16x16tbm,16*5,0,11,0,1);

LCD_PXXxYYCHtoBMPL(16,16,F16x16tbm,16*1,0,12,0,1);
LCD_PXXxYYCHtoBMPL(16,16,F16x16tbm,16*6,0,13,0,1);

//LCD_PXXxYYCHtoBMPL(16,16,F16x16tbm,16*2,0,3,0,1);
//LCD_PXXxYYCHtoBMPL(16,16,F16x16tbm,16*7,0,1,0,1);
}



void clear_drawpotm_k60(void) 
{
int k;
for(k=0;k<128*8;k++)
// DRAWPOTm[k]=0x00;
DRAWPOTtempm[k]=0x00;
}

void ceshi_speedlevel(void)
{
clear_drawpotm_k60();
diyihangview();
DrawLineX_fL(6,122,18,1);
DrawLineX_fL(6,122,54,1);       
DrawLineY_fL(18,54,6,1);
DrawLineY_fL(18,54,122,1);

DrawLineX_fL(4,124,16,1);
DrawLineX_fL(4,124,56,1);       
DrawLineY_fL(16,56,4,1);
DrawLineY_fL(16,56,124,1);
LCD_6x8StrtoBMPL(7+8*2,24+8,"SPEED LEVEL:",1);
countprint(speed_level,speed_level_lcd);
LCD_6x8StrtoBMPL(7+8*4,24+8+8,speed_level_lcd,1);
keyup_=keyUp;
if(!keyup_) 
             {
                systick_delay_ms(20);
                  keyup_=keyUp;
                if(!keyup_)
                {      keyup_=keyUp;
                    while(!keyup_)
                    {  keyup_=keyUp;};
                    if(speed_level>1)
                    speed_level--;
                   else 
                    speed_level=5;
                    
                }
            }
keydown_=keyDown;
if(!keydown_) 
             {
                systick_delay_ms(20);
                keydown_=keyDown;
                if(!keydown_)
                {    keydown_=keyDown;
                    while(!keydown_)
                    {keydown_=keyDown;};
                    if(speed_level<5)
                    speed_level++;
                   else 
                    speed_level=1;
                    
                }
            }

keyenter_=keyEnter;
if(!keyenter_) 
             {
                systick_delay_ms(20);
                keyenter_=keyEnter;
                if(!keyenter_)
                {    keyenter_=keyEnter;
                    while(!keydown_)
                    {keyenter_=keyEnter;};
                    
                    switch(speed_level)
                    {
                      case 1:write_speed_gear1();break;
                      case 2:write_speed_gear2();break;
                      case 3:write_speed_gear3();break;
                      case 4:write_speed_gear4();break;
                      case 5:write_speed_gear5();break;
                    }
                    flash_erase_memories();
                    flash_write_memories();
                    flash_read_memories();
                    read_parameter();
                    OLED_speed_set();
                    
                    LCDcanshuchushihua();//更新屏幕内参数
                }
            }





}

void ceshi_stop(void) 

{
clear_drawpotm_k60();
diyihangview();

DrawLineX_fL(6,122,18,1);
DrawLineX_fL(6,122,54,1);       
DrawLineY_fL(18,54,6,1);
DrawLineY_fL(18,54,122,1);

DrawLineX_fL(4,124,16,1);
DrawLineX_fL(4,124,56,1);       
DrawLineY_fL(16,56,4,1);
DrawLineY_fL(16,56,124,1);

LCD_6x8StrtoBMPL(7,24,"add the startline?",1);
LCD_6x8StrtoBMPL(16*2,40,"yes",1);
LCD_6x8StrtoBMPL(16*5,40,"no",1);

  keyup_=keyUp;
if(!keyup_) 
             {
                systick_delay_ms(20);
                  keyup_=keyUp;
                if(!keyup_)
                {      keyup_=keyUp;
                    while(!keyup_)
                    {  keyup_=keyUp;};
                    if(flag_stop==1)
                    flag_stop--;
                   else 
                    flag_stop=1;
                    
                }
            }
keydown_=keyDown;
if(!keydown_) 
             {
                systick_delay_ms(20);
                keydown_=keyDown;
                if(!keydown_)
                {    keydown_=keyDown;
                    while(!keydown_)
                    {keydown_=keyDown;};
                    if(flag_stop==0)
                    flag_stop++;
                   else 
                    flag_stop=0;
                    
                }
            }
 if(flag_gundongs[9]){
 
 if(flag_stop)
{
LCD_6x8StrtoBMPL(16*2-6,40,">",1); 
LCD_6x8StrtoBMPL(16*2+18,40,"<",1);
} else 
{
LCD_6x8StrtoBMPL(16*5-6,40,">",1); 
LCD_6x8StrtoBMPL(16*5+12,40,"<",1);
}
 } 
 
 else
 {
 
            
            
            
if(flag_stop){
              
LCD_6x8StrtoBMPL(16*2-6,40,">",0); 
LCD_6x8StrtoBMPL(16*2+18,40,"<",0);
LCD_6x8StrtoBMPL(16*5-6,40," ",0); 
LCD_6x8StrtoBMPL(16*5+12,40," ",0);
LCD_PXXxYYCHtoBMPL(16,16,F16x16tbm,16*7,0,7,0,0);


            } else 
            {
LCD_PXXxYYCHtoBMPL(16,16,F16x16tbm,16*7,0,2,0,0);
LCD_6x8StrtoBMPL(16*2-6,40," ",0); 
LCD_6x8StrtoBMPL(16*2+18,40," ",0);
LCD_6x8StrtoBMPL(16*5-6,40,">",0); 
LCD_6x8StrtoBMPL(16*5+12,40,"<",0);
            }

}


}



void dijiujie_speed_tiaoshi(void)
{
int m;
clear_drawpottempm();

diyihangview();

//---------------------参数修改界面名称---------------------//
LCD_6x8StrtoBMPL(1*16,2*8,"dawan_s:",1);//dawanspeed
LCD_6x8StrtoBMPL(1*16,3*8,"KP:",1);
LCD_6x8StrtoBMPL(1*16,4*8,"qianzhan:",1);
LCD_6x8StrtoBMPL(1*16,5*8,"xl_dian:",1);
LCD_6x8StrtoBMPL(1*16,6*8,"zhidao_s:",1);
LCD_6x8StrtoBMPL(1*16,7*8,"yun_su?:",1);

if(flag_LCD_queren==0) 
{
  keyup_=keyUp;
  if(!keyup_) 
             {
                systick_delay_ms(20);
                keyup_=keyUp;
                if(!keyup_)
                {   keyup_=keyUp; 
                    while(!keyup_)
                    {keyup_=keyUp;};
                    if(flag_LCD_revise>0)
                    flag_LCD_revise--;
                   else 
                    flag_LCD_revise=5;
                    
                }
            }
  keydown_=keyDown;
if(!keydown_) 
             {
                keydown_=keyDown;
                systick_delay_ms(20);
                if(!keydown_)
                {   keydown_=keyDown;
                    while(!keydown_)
                    {keydown_=keyDown;};
                    if(flag_LCD_revise<5)
                    flag_LCD_revise++;
                   else 
                    flag_LCD_revise=0;
                    
                }
            }


}
keyenter_=keyEnter;
if(!keyenter_) 
             {
                systick_delay_ms(20);
                keyenter_=keyEnter;
                if(!keyenter_)
                {   keyenter_=keyEnter; 
                    while(!keyenter_)
                    {keyenter_=keyEnter;};
                    if(flag_LCD_queren==0)
                    flag_LCD_queren++;
                   else 
                    flag_LCD_queren=0;
                    
                }
            }
 /*
struct                                     //第九届液晶显示各个参数结构体
{
 int zhidao_speed;
 int xiaowan_speed;
 int xiaowan_chasu;
 int jiwan_speed;
 int jiwan_chasu;
 int podao_speed;
 
 char zhidao_speed_lcd[4];
 char xiaowan_speed_lcd[4];
 char xiaowan_chasu_lcd[4];
 char jiwan_speed_lcd[4];
 char jiwan_chasu_lcd[4];
 char podao_speed_lcd[4];
 
int MMA7361_ZZ;
int GYRO_X_ZZ;
int GYRO_Y_ZZ;

}dijiujiecanshu;   */
            
if(flag_LCD_queren)
{
 switch(flag_LCD_revise) 
 {
  case 0:
 //+++++++++++++++++++++++++++++++++++++++++++++++++最后这里要改++++++++++++++++++++++++++++++++++       
         dawan_speed=Parameterset(dawan_speed);
              
         break;
  case 1:
         kp=Parameterset(kp);     
         break;
  case 2:dian_1=Parameterset(dian_1);     
         break;
  case 3:xielv_dian=Parameterset(xielv_dian);     
         break;
 
  case 4:longs=Parameterset(longs);     
          
         break;
  case 5:yunsuflag=Parameterset(yunsuflag);     
         
         break;
 }
}
/*countprint(dijiujiecanshu.zhidao_speed,dijiujiecanshu.zhidao_speed_lcd); 
countprint(dijiujiecanshu.xiaowan_speed,dijiujiecanshu.xiaowan_speed_lcd);
countprint(dijiujiecanshu.xiaowan_chasu,dijiujiecanshu.xiaowan_chasu_lcd);
countprint(dijiujiecanshu.jiwan_speed,dijiujiecanshu.jiwan_speed_lcd); 
countprint(dijiujiecanshu.jiwan_chasu,dijiujiecanshu.jiwan_chasu_lcd);
countprint(dijiujiecanshu.podao_speed,dijiujiecanshu.podao_speed_lcd);*/
//+++++++++++++++++++++++++++++++++++++++++++++++++再最后+++++++++++++++++++++++++++++++++++++++++++++  
countprint(dawan_speed,user_p0);  
countprint(kp,user_p1);
countprint(dian_1,user_p2);
countprint(xielv_dian,user_p3); 
countprint(longs,user_p4);
countprint(yunsuflag,user_p5); 




 if(flag_gundongs[0]) 
     { 
      LCD_6x8StrtoBMPL(1*16+12*6,2*8,user_p0,1);
      LCD_6x8StrtoBMPL(1*16+12*6,3*8,user_p1,1);
      LCD_6x8StrtoBMPL(1*16+12*6,4*8,user_p2,1);
      LCD_6x8StrtoBMPL(1*16+12*6,5*8,user_p3,1);    //添加之后如果多处显示就换参数就OK了
      LCD_6x8StrtoBMPL(1*16+12*6,6*8,user_p4,1);
      LCD_6x8StrtoBMPL(1*16+12*6,7*8,user_p5,1);
       /*LCD_6x8StrtoBMPL(1*16+12*6,2*8,dijiujiecanshu.zhidao_speed_lcd,1);
       LCD_6x8StrtoBMPL(1*16+12*6,3*8,dijiujiecanshu.xiaowan_speed_lcd,1);
       LCD_6x8StrtoBMPL(1*16+12*6,4*8,dijiujiecanshu.xiaowan_chasu_lcd,1);
       LCD_6x8StrtoBMPL(1*16+12*6,5*8,dijiujiecanshu.jiwan_speed_lcd,1);    //添加之后如果多处显示就换参数就OK了
       LCD_6x8StrtoBMPL(1*16+12*6,6*8,dijiujiecanshu.jiwan_chasu_lcd,1);
       LCD_6x8StrtoBMPL(1*16+12*6,7*8,dijiujiecanshu.podao_speed_lcd,1); */
       if(!flag_LCD_queren) 
           {
              for(m=0;m<6;m++)
                   LCD_PXXxYYCHtoBMPL(16,8,F8x16tbm,0,16+8*m,2,0,1); 
                   LCD_PXXxYYCHtoBMPL(16,8,F8x16tbm,0,16+8*flag_LCD_revise,0,0,1);   
           }
       else
             LCD_PXXxYYCHtoBMPL(16,8,F8x16tbm,0,16+8*flag_LCD_revise,1,0,1);
    }

 else 
  {   
LCD_6x8StrtoBMPL(1*16+12*6,2*8,user_p0,0);
LCD_6x8StrtoBMPL(1*16+12*6,3*8,user_p1,0);
LCD_6x8StrtoBMPL(1*16+12*6,4*8,user_p2,0);
LCD_6x8StrtoBMPL(1*16+12*6,5*8,user_p3,0);
LCD_6x8StrtoBMPL(1*16+12*6,6*8,user_p4,0);
LCD_6x8StrtoBMPL(1*16+12*6,7*8,user_p5,0);
     /* LCD_6x8StrtoBMPL(1*16+12*6,2*8,dijiujiecanshu.zhidao_speed_lcd,0);
      LCD_6x8StrtoBMPL(1*16+12*6,3*8,dijiujiecanshu.xiaowan_speed_lcd,0);
      LCD_6x8StrtoBMPL(1*16+12*6,4*8,dijiujiecanshu.xiaowan_chasu_lcd,0);
      LCD_6x8StrtoBMPL(1*16+12*6,5*8,dijiujiecanshu.jiwan_speed_lcd,0);
      LCD_6x8StrtoBMPL(1*16+12*6,6*8,dijiujiecanshu.jiwan_chasu_lcd,0);
      LCD_6x8StrtoBMPL(1*16+12*6,7*8,dijiujiecanshu.podao_speed_lcd,0);  */
      if(!flag_LCD_queren)
           {
              for(m=0;m<6;m++)
                  for(m=0;m<6;m++)
                      LCD_PXXxYYCHtoBMPL(16,8,F8x16tbm,0,16+8*m,2,0,0); 
                      LCD_PXXxYYCHtoBMPL(16,8,F8x16tbm,0,16+8*flag_LCD_revise,0,0,0);
           }
      else
        LCD_PXXxYYCHtoBMPL(16,8,F8x16tbm,0,16+8*flag_LCD_revise,1,0,0);
  }
  
  
}            



void dijiujie_speed_tiaoshi2(void)
{
int m;
clear_drawpottempm();

diyihangview();


LCD_6x8StrtoBMPL(1*16,2*8,"QWspeed:",1);
LCD_6x8StrtoBMPL(1*16,3*8,"MMA7361:",1);
LCD_6x8StrtoBMPL(1*16,4*8,"GYRO_X:",1);
LCD_6x8StrtoBMPL(1*16,5*8,"GYRO_Y:",1);

LCD_6x8StrtoBMPL(1*16,6*8,"jiwanchasu:",1);
LCD_6x8StrtoBMPL(1*16,7*8,"podaospeed:",1);

if(flag_LCD_queren==0) {
  keyup_=keyUp;
  if(!keyup_) 
             {
                systick_delay_ms(20);
                keyup_=keyUp;
                if(!keyup_)
                {   keyup_=keyUp; 
                    while(!keyup_)
                    {keyup_=keyUp;};
                    if(flag_LCD_revise>0)
                    flag_LCD_revise--;
                   else 
                    flag_LCD_revise=5;
                    
                }
            }
  keydown_=keyDown;
if(!keydown_) 
             {
                keydown_=keyDown;
                systick_delay_ms(20);
                if(!keydown_)
                {   keydown_=keyDown;
                    while(!keydown_)
                    {keydown_=keyDown;};
                    if(flag_LCD_revise<5)
                    flag_LCD_revise++;
                   else 
                    flag_LCD_revise=0;
                }
            }


}
keyenter_=keyEnter;
if(!keyenter_) 
             {
                systick_delay_ms(20);
                keyenter_=keyEnter;
                if(!keyenter_)
                {   keyenter_=keyEnter; 
                    while(!keyenter_)
                    {keyenter_=keyEnter;};
                    if(flag_LCD_queren==0)
                    flag_LCD_queren++;
                   else 
                    flag_LCD_queren=0;
                    
                }
            }
 /*
struct                                     //第九届液晶显示各个参数结构体
{
 int zhidao_speed;
 int xiaowan_speed;
 int xiaowan_chasu;
 int jiwan_speed;
 int jiwan_chasu;
 int podao_speed;
 
 char zhidao_speed_lcd[4];
 char xiaowan_speed_lcd[4];
 char xiaowan_chasu_lcd[4];
 char jiwan_speed_lcd[4];
 char jiwan_chasu_lcd[4];
 char podao_speed_lcd[4];
 
int MMA7361_ZZ;
int GYRO_X_ZZ;
int GYRO_Y_ZZ;

}dijiujiecanshu;   */
            
if(flag_LCD_queren){
  
 switch(flag_LCD_revise) 
 {
  case 0:
         
         DSYJcanshu.QWspeed=Parameterset20( DSYJcanshu.QWspeed);
              
         break;
  case 1:
         DSYJcanshu.MMA7361_ZZ=Parameterset(DSYJcanshu.MMA7361_ZZ);     
         break;
  case 2:DSYJcanshu.GYRO_X_ZZ=Parameterset(DSYJcanshu.GYRO_X_ZZ);     
         break;
  case 3:DSYJcanshu.GYRO_Y_ZZ=Parameterset(DSYJcanshu.GYRO_Y_ZZ);     
         break;
 
  case 4:dijiujiecanshu.jiwan_chasu=Parameterset(dijiujiecanshu.jiwan_chasu);     
          
         break;
  case 5:dijiujiecanshu.podao_speed=Parameterset(dijiujiecanshu.podao_speed);     
         
         break;
 }
}
/*countprint(dijiujiecanshu.zhidao_speed,dijiujiecanshu.zhidao_speed_lcd); 
countprint(dijiujiecanshu.xiaowan_speed,dijiujiecanshu.xiaowan_speed_lcd);
countprint(dijiujiecanshu.xiaowan_chasu,dijiujiecanshu.xiaowan_chasu_lcd);
countprint(dijiujiecanshu.jiwan_speed,dijiujiecanshu.jiwan_speed_lcd); 
countprint(dijiujiecanshu.jiwan_chasu,dijiujiecanshu.jiwan_chasu_lcd);
countprint(dijiujiecanshu.podao_speed,dijiujiecanshu.podao_speed_lcd);*/
countprint(DSYJcanshu.QWspeed, DSYJcanshu.QWspeed_lcd);  
countprint(DSYJcanshu.MMA7361_ZZ,DSYJcanshu.MMA7361_ZZ_lcd);
countprint(DSYJcanshu.GYRO_X_ZZ,DSYJcanshu.GYRO_X_ZZ_lcd);
countprint(DSYJcanshu.GYRO_Y_ZZ,DSYJcanshu.GYRO_Y_ZZ_lcd); 

countprint(dijiujiecanshu.jiwan_chasu,dijiujiecanshu.jiwan_chasu_lcd);
countprint(dijiujiecanshu.podao_speed,dijiujiecanshu.podao_speed_lcd); 




 if(flag_gundongs[0]) 
     { 
      LCD_6x8StrtoBMPL(1*16+12*6,2*8, DSYJcanshu.QWspeed_lcd,1);
      LCD_6x8StrtoBMPL(1*16+12*6,3*8,DSYJcanshu.MMA7361_ZZ_lcd,1);
      LCD_6x8StrtoBMPL(1*16+12*6,4*8,DSYJcanshu.GYRO_X_ZZ_lcd,1);
      LCD_6x8StrtoBMPL(1*16+12*6,5*8,DSYJcanshu.GYRO_Y_ZZ_lcd,1);    //添加之后如果多处显示就换参数就OK了
      LCD_6x8StrtoBMPL(1*16+12*6,6*8,dijiujiecanshu.jiwan_chasu_lcd,1);
      LCD_6x8StrtoBMPL(1*16+12*6,7*8,dijiujiecanshu.podao_speed_lcd,1);
       /*LCD_6x8StrtoBMPL(1*16+12*6,2*8,dijiujiecanshu.zhidao_speed_lcd,1);
       LCD_6x8StrtoBMPL(1*16+12*6,3*8,dijiujiecanshu.xiaowan_speed_lcd,1);
       LCD_6x8StrtoBMPL(1*16+12*6,4*8,dijiujiecanshu.xiaowan_chasu_lcd,1);
       LCD_6x8StrtoBMPL(1*16+12*6,5*8,dijiujiecanshu.jiwan_speed_lcd,1);    //添加之后如果多处显示就换参数就OK了
       LCD_6x8StrtoBMPL(1*16+12*6,6*8,dijiujiecanshu.jiwan_chasu_lcd,1);
       LCD_6x8StrtoBMPL(1*16+12*6,7*8,dijiujiecanshu.podao_speed_lcd,1); */
       if(!flag_LCD_queren) 
           {
              for(m=0;m<6;m++)
                   LCD_PXXxYYCHtoBMPL(16,8,F8x16tbm,0,16+8*m,2,0,1); 
                   LCD_PXXxYYCHtoBMPL(16,8,F8x16tbm,0,16+8*flag_LCD_revise,0,0,1);   
           }
       else
             LCD_PXXxYYCHtoBMPL(16,8,F8x16tbm,0,16+8*flag_LCD_revise,1,0,1);
    }

 else 
  {   LCD_6x8StrtoBMPL(1*16+12*6,2*8,DSYJcanshu.QWspeed_lcd,0);
LCD_6x8StrtoBMPL(1*16+12*6,3*8,DSYJcanshu.MMA7361_ZZ_lcd,0);
LCD_6x8StrtoBMPL(1*16+12*6,4*8,DSYJcanshu.GYRO_X_ZZ_lcd,0);
LCD_6x8StrtoBMPL(1*16+12*6,5*8,DSYJcanshu.GYRO_Y_ZZ_lcd,0);
LCD_6x8StrtoBMPL(1*16+12*6,6*8,dijiujiecanshu.jiwan_chasu_lcd,0);
LCD_6x8StrtoBMPL(1*16+12*6,7*8,dijiujiecanshu.podao_speed_lcd,0);
     /* LCD_6x8StrtoBMPL(1*16+12*6,2*8,dijiujiecanshu.zhidao_speed_lcd,0);
      LCD_6x8StrtoBMPL(1*16+12*6,3*8,dijiujiecanshu.xiaowan_speed_lcd,0);
      LCD_6x8StrtoBMPL(1*16+12*6,4*8,dijiujiecanshu.xiaowan_chasu_lcd,0);
      LCD_6x8StrtoBMPL(1*16+12*6,5*8,dijiujiecanshu.jiwan_speed_lcd,0);
      LCD_6x8StrtoBMPL(1*16+12*6,6*8,dijiujiecanshu.jiwan_chasu_lcd,0);
      LCD_6x8StrtoBMPL(1*16+12*6,7*8,dijiujiecanshu.podao_speed_lcd,0);  */
      if(!flag_LCD_queren)
           {
              for(m=0;m<6;m++)
                  for(m=0;m<6;m++)
                      LCD_PXXxYYCHtoBMPL(16,8,F8x16tbm,0,16+8*m,2,0,0); 
                      LCD_PXXxYYCHtoBMPL(16,8,F8x16tbm,0,16+8*flag_LCD_revise,0,0,0);
           }
      else
        LCD_PXXxYYCHtoBMPL(16,8,F8x16tbm,0,16+8*flag_LCD_revise,1,0,0);
  }
  
  
}




int Parameterset(int p)         //参数set

{
keyup_=keyUp;
if(!keyup_) 
            {
                systick_delay_ms(20);
                keyup_=keyUp;
                if(!keyup_)
                { keyup_=keyUp;   
                  while(!keyup_)
                  {keyup_=keyUp;};
                  p++;  
                    
                }
            }
keydown_=keyDown;
 if(!keydown_) 
            {
                systick_delay_ms(20);
                keydown_=keyDown;
                if(!keydown_)
                {  keydown_=keyDown;  
                  while(!keydown_)
                  {keydown_=keyDown;};
                  p--;  
                    
                }
            }
       return p;
}



int Parameterset20(int p)         //参数set

{
keyup_=keyUp;
if(!keyup_) 
            {
                systick_delay_ms(20);
                keyup_=keyUp;
                if(!keyup_)
                { keyup_=keyUp;   
                  while(!keyup_)
                  {keyup_=keyUp;};
                  p+=10;  
                    
                }
            }
keydown_=keyDown;
 if(!keydown_) 
            {
                systick_delay_ms(20);
                keydown_=keyDown;
                if(!keydown_)
                {  keydown_=keyDown;  
                  while(!keydown_)
                  {keydown_=keyDown;};
                  p-=10;  
                    
                }
            }
       return p;
}


int Backtom(int flag,int k) 
{
    keyleft_=keyLeft;
      if(!keyleft_) 
            {
                systick_delay_ms(20);
                keyleft_=keyLeft;
                if(!keyleft_)
                {   keyleft_=keyLeft; 
                    while(!keyleft_)
                    {keyleft_=keyLeft;};
                    flag_LCD_first=1;
                    flag=0;
                    flag_gundongs[k]=1;
                    flag_gundongm=1;
                    //LCD_CLS();
                    
                }
            }
            return flag;  
}

/*int Backtom(int flag,int k) 
{
      if(!GPIO_Get(PORTE,8)) 
            {
                systick_delay_ms(20);
                if(!GPIO_Get(PORTE,8))
                {    
                    while(!GPIO_Get(PORTE,8));
                    flag_LCD_first=1;
                    flag=0;
                    flag_gundongs[k]=1;
                    flag_gundongm=1;
                    //LCD_CLS();
                    
                }
            }
            return flag;  
}
*/
void OLED_starting_car()//flash写入显示
{
clear_drawpotm_k60();

DrawLineX_fL(6,122,18,1);
DrawLineX_fL(6,122,54,1);       
DrawLineY_fL(18,54,6,1);
DrawLineY_fL(18,54,122,1);

DrawLineX_fL(4,124,16,1);
DrawLineX_fL(4,124,56,1);       
DrawLineY_fL(16,56,4,1);
DrawLineY_fL(16,56,124,1);
LCD_6x8StrtoBMPL(7+16,24+8,"WINDOWS CARS",1);


Draw_BMP(0,0,128,8,DRAWPOTtempm);
}


void OLED_flash_memories()//flash写入显示
{
clear_drawpotm_k60();

DrawLineX_fL(6,122,18,1);
DrawLineX_fL(6,122,54,1);       
DrawLineY_fL(18,54,6,1);
DrawLineY_fL(18,54,122,1);

DrawLineX_fL(4,124,16,1);
DrawLineX_fL(4,124,56,1);       
DrawLineY_fL(16,56,4,1);
DrawLineY_fL(16,56,124,1);
LCD_6x8StrtoBMPL(7,24,"flash update...",1);
Draw_BMP(0,0,128,8,DRAWPOTtempm);
}

void OLED_flash_memories_queren()//flash写入显示
{
clear_drawpotm_k60();

DrawLineX_fL(6,122,18,1);
DrawLineX_fL(6,122,54,1);       
DrawLineY_fL(18,54,6,1);
DrawLineY_fL(18,54,122,1);

DrawLineX_fL(4,124,16,1);
DrawLineX_fL(4,124,56,1);       
DrawLineY_fL(16,56,4,1);
DrawLineY_fL(16,56,124,1);
LCD_6x8StrtoBMPL(7,24,"flash update...",1);
LCD_6x8StrtoBMPL(7,24+8,"update finish...",1);
Draw_BMP(0,0,128,8,DRAWPOTtempm);
}

void OLED_start_finish()//flash写入显示
{
clear_drawpotm_k60();

DrawLineX_fL(6,122,18,1);
DrawLineX_fL(6,122,54,1);       
DrawLineY_fL(18,54,6,1);
DrawLineY_fL(18,54,122,1);

DrawLineX_fL(4,124,16,1);
DrawLineX_fL(4,124,56,1);       
DrawLineY_fL(16,56,4,1);
DrawLineY_fL(16,56,124,1);
LCD_6x8StrtoBMPL(7,24,"Start finish...",1);
LCD_6x8StrtoBMPL(7,24+8,"I'm fine!!!",1);
Draw_BMP(0,0,128,8,DRAWPOTtempm);
}
void OLED_speed_set()//flash写入显示
{
clear_drawpotm_k60();

DrawLineX_fL(6,122,18,1);
DrawLineX_fL(6,122,54,1);       
DrawLineY_fL(18,54,6,1);
DrawLineY_fL(18,54,122,1);

DrawLineX_fL(4,124,16,1);
DrawLineX_fL(4,124,56,1);       
DrawLineY_fL(16,56,4,1);
DrawLineY_fL(16,56,124,1);
LCD_6x8StrtoBMPL(7,24,"set finish...",1);
LCD_6x8StrtoBMPL(7,24+8,"Press the left",1);
LCD_6x8StrtoBMPL(7,24+16,"button to exit!",1);
Draw_BMP(0,0,128,8,DRAWPOTtempm);
systick_delay_ms(500);
}

void VIEW_CCD_dongtai(u8 x[])
{
  u8 i;
  //clear_OLED_VIEW();
  clear_drawpottempm();
  
 /* OLED_DrawLine_Y(0,3,63,0); //  三个稍长的标号
  OLED_DrawLine_Y(0,3,31,0);
  OLED_DrawLine_Y(0,3,95,0);
  
  OLED_DrawLine_Y(28,31,63,0);
  OLED_DrawLine_Y(28,31,31,0);
  OLED_DrawLine_Y(28,31,95,0);
  
  
  OLED_DrawLine_Y(0,1,15,0); //四个稍短的标号
  OLED_DrawLine_Y(0,1,47,0);
  OLED_DrawLine_Y(0,1,79,0);
  OLED_DrawLine_Y(0,1,111,0);
  
  OLED_DrawLine_Y(30,31,15,0);
  OLED_DrawLine_Y(30,31,47,0);
  OLED_DrawLine_Y(30,31,79,0);
  OLED_DrawLine_Y(30,31,111,0);
  
  
  OLED_DrawLine_Y(0,31,0,0);//  两条竖着的边
  OLED_DrawLine_Y(0,31,127,0);
  
  OLED_DrawLine_X(0,127,31,0);//一条下底盘线
 
  OLED_DrawLine_X(0,3,15,0);  //两天 侧面的 标号
  OLED_DrawLine_X(124,127,15,0);*/
  
  DrawLineY_fL(0,3,63,1); //  三个稍长的标号
  DrawLineY_fL(0,3,31,1);
  DrawLineY_fL(0,3,95,1);
  
  DrawLineY_fL(28,31,63,1);
  DrawLineY_fL(28,31,31,1);
  DrawLineY_fL(28,31,95,1);
  
  
  DrawLineY_fL(0,1,15,1); //四个稍短的标号
  DrawLineY_fL(0,1,47,1);
  DrawLineY_fL(0,1,79,1);
  DrawLineY_fL(0,1,111,1);
  
  DrawLineY_fL(30,31,15,1);
  DrawLineY_fL(30,31,47,1);
  DrawLineY_fL(30,31,79,1);
  DrawLineY_fL(30,31,111,1);
  
  
  DrawLineY_fL(0,31,0,1);//  两条竖着的边
  DrawLineY_fL(0,31,127,1);
  
  DrawLineX_fL(0,127,31,1);//一条下底盘线
 
  DrawLineX_fL(0,3,15,1);  //两天 侧面的 标号
  DrawLineX_fL(124,127,15,1); 
  

  
//countprint(zuobian,CCD_LIVE_WATCH.CCD1_char); 
//OLED_write_8X16String(0*8,2*16,CCD_LIVE_WATCH.CCD1_char,0);
LCD_P8x16StrtoBMPL(0*8,2*16,CCD_LIVE_WATCH.CCD1_char,1);

//countprint(youbian,CCD_LIVE_WATCH.CCD2_char); 
//OLED_write_8X16String(12*8,2*16,CCD_LIVE_WATCH.CCD2_char,0);
 LCD_P8x16StrtoBMPL(12*8,2*16,CCD_LIVE_WATCH.CCD2_char,1); 
//countprint(piancha,CCD_LIVE_WATCH.CCD3_char); 
//OLED_write_8X16String(6*8,2*16,CCD_LIVE_WATCH.CCD3_char,0);
LCD_P8x16StrtoBMPL(6*8,2*16,CCD_LIVE_WATCH.CCD3_char,1);
//countprint(piancha1,CCD_LIVE_WATCH.CCD4_char); //youbian-zuobian
//OLED_write_8X16String(6*8,3*16,CCD_LIVE_WATCH.CCD4_char,0);
LCD_P8x16StrtoBMPL(6*8,3*16,CCD_LIVE_WATCH.CCD4_char,1);
//countprint(duoji,CCD_LIVE_WATCH.CCD4_char); 
//OLED_write_8X16String(0*8,3*16,CCD_LIVE_WATCH.CCD4_char,0);
LCD_P8x16StrtoBMPL(0*8,3*16,CCD_LIVE_WATCH.CCD4_char,1);


//countprint(DSYJ_CCD_junzhi,CCD_LIVE_WATCH.CCD6_char); 
//OLED_write_8X16String(0*8,3*16,CCD_LIVE_WATCH.CCD4_char,0);
LCD_P8x16StrtoBMPL(12*8,3*16,CCD_LIVE_WATCH.CCD6_char,1);
//countprint(g_fCarSpeednow,CCD_LIVE_WATCH.CCD5_char); 
//LCD_P8x16StrtoBMPL(12*8,3*16,CCD_LIVE_WATCH.CCD5_char,1); 
 // DrawLineY_fL(0,31,piancha-63,1);
 // DrawLineY_fL(0,15,zuobian,1);
 // DrawLineY_fL(0,15,youbian,1);
 // DrawLineY_fL(0,15,zhongjian,1);
//  DrawLineY_fL(0,15,g_fCarSpeednow,1);
  
  //DrawLineY_fL(16,15,speed_now,1);
  //OLED_Diamondhero_Draw_XXYY(16,16,F16x16tbm,16*7,16*3,14,0,0);
  //LCD_PXXxYYCHtoBMPL(16,16,F16x16tbm,16*7,16*3,14,0,1);
  //LCD_PXXxYYCHtoBMP
  
  for(i=0;i<128;i++)
  {
    //OLED_Draw_pot(i,(256-x[i])/8,1,0);
   Draw_potL(i,(256-x[i])/8,1,1);
  }
 // OLED_Draw_pot(127,255/8,1,0);
  //OLED_VIEW_OUT(0,0,128,8,OLED_VIEW); 
   LCD_PXXxYYCHtoBMPL(128,64,DRAWPOTtempm,0,0,0,0,1);
}