/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       main.c
 * @brief      山外K60 平台主程序
 * @author     山外科技
 * @version    v5.3
 * @date       2015-04-07
 *///旧的*///

#include "common.h"
#include "include.h"
#include "math.h"
#include "saomiao.h"
int yunsuflag;
uint8 imgbuff[CAMERA_SIZE];     
 int currentzhongjian[60];
int quanjuCount,xielv_dian;
int OutData[4],maichongshu;
extern int dawan_speed,duojijiaodu,dian1,dian2,dian3,kp,zhankongbi,mubiao_speed,dian,dianyuan,zhidao_flag,qvlv_quanju,control_point2,qulv_jinduan,qulv_yuandaun,my_piancha,xielv,xielv_yuan,right_heixian[60],left_heixian[60],control_point,control_point1,qvlv_quanju_right,qvlv_quanju_left;
//int kp=22,kd=10,right_heixian[60],lastpiancha_1,duojijiaodu,left_heixian[60],my_lastzhongjian=40,currentzhongjianzhi=40,,leftheixian_flag,xielv;
//函数声明
void PORTA_IRQHandler();
void DMA0_IRQHandler();
void PIT0_IRQHandler();
void Priority_Set();
extern int regression(int startline,int endline);
char time,star_lineflag,star_line,go,L,R;
extern char xielv_flag,qulv_point,zhangai_right,zhangai_left,podao_flag;
extern  void speedcontrol();
extern  void servo();
extern  void myself_key();
//void PIT1_IRQHandler();
/*!
 *  @brief      main函数
 *  @since      v5.3
 *  @note       山外 DMA 采集摄像头 实验
                注意，此例程 bus频率设为100MHz(50MHz bus频率会太慢而导致没法及时采集图像)

                此例程使用的上位机为【山外多功能调试助手】
                具体资料请参考：山外多功能调试助手资料专辑
                                http://www.vcan123.com/forum.php?mod=collection&action=view&ctid=27
 */
void hecheng()//虚拟中线函数
{
unsigned int i;
for(i=0;i<=59;i++)
      {
       imgyiwei[i][currentzhongjian[i]]=0 ;
       //imgyiwei[i][right_heixian[i]]=0 ;
       // imgyiwei[i][left_heixian[i]]=0 ;
      }
}
void sendimg(uint8 *imgaddr,uint32 imgsize)
{
img_extract((u8 *)imgyiwei, imgbuff,CAMERA_SIZE);      //解压图像
congzhongjianwangliangbian();
uint8 cmd[4] = {0, 255, 1, 0 };    //yy_摄像头串口调试 使用的命令
hecheng();
uart_putbuff(VCAN_PORT, cmd, sizeof(cmd));    //先发送命令
uart_putbuff(VCAN_PORT, imgaddr, imgsize); //再发送图像
}
//**************************************************************************
/*
*  功能说明：SCI示波器CRC校验
内部调用函数
*  参数说明： 无
*  函数返回：无符号结果值
*  修改时间：2013-2-10
*/
//**************************************************************************
static unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT)
{
    unsigned short CRC_Temp;
    unsigned char i,j;
    CRC_Temp = 0xffff;

    for (i=0;i<CRC_CNT; i++){
        CRC_Temp ^= Buf[i];
        for (j=0;j<8;j++) {
            if (CRC_Temp & 0x01)
                CRC_Temp = (CRC_Temp >>1 ) ^ 0xa001;
            else
                CRC_Temp = CRC_Temp >> 1;
        }
    }
    return(CRC_Temp);
}

//************************************************
//
/*
*  功能说明：SCI示波器发送函数

*  参数说明：
OutData[]  需要发送的数值赋予该数组
*  函数返回：无符号结果值
*  修改时间：2013-2-10
*/
//****************************************************
void OutPut_Data(void)
{   
    int temp[4] = {0};
    unsigned int temp1[4] = {0};
    unsigned char databuf[10] = {0};
    unsigned char i;
    unsigned short CRC16 = 0;
    for(i=0;i<4;i++)
    {

        temp[i]  = (int)OutData[i];
        temp1[i] = (unsigned int)temp[i];

    }

    for(i=0;i<4;i++)
    {
        databuf[i*2]   = (unsigned char)(temp1[i]%256);
        databuf[i*2+1] = (unsigned char)(temp1[i]/256);
    }

    CRC16 = CRC_CHECK(databuf,8);
    databuf[8] = CRC16%256;
    databuf[9] = CRC16/256;

    for(i=0;i<10;i++)
    {
        uart_putchar (UART0,(char)databuf[i]);
    }
}

void SendHex(unsigned char hex) {
  unsigned char temp;
  temp = hex >> 4;
  if(temp < 10) {
    uart_putchar(UART0,temp + '0');
  } else {
    uart_putchar(UART0,temp - 10 + 'A');
  }
  temp = hex & 0x0F;
  if(temp < 10) {
    uart_putchar(UART0,temp + '0');
  } else {
   uart_putchar(UART0,temp - 10 + 'A');
  }
}

void SendImageData(unsigned char ImageData[][80]) 
{
    int lll1,lll2;
    unsigned char crc = 0;

    /* Send Data */
    uart_putchar(UART0,'*');
    uart_putchar(UART0,'L');
    uart_putchar(UART0,'D');

    SendHex(0);
    SendHex(0);
    SendHex(0);
    SendHex(0);
    
  //  imgyiwei[60][80]; 

    for(lll2=0;lll2<80;lll2++)
    {
      
      for(lll1=0;lll1<60; lll1++)
        SendHex(ImageData[lll1][lll2]);
    }
    

    SendHex(crc);
    uart_putchar(UART0,'#');
}
//********************************************************起跑线检测***********************************************************
void star_line_judg()//起跑线检测
{
 int kk,left_baihei_flag,right_baihei_flag,baihei_flag,left_heibai_flag,right_heibai_flag;;
  for(kk=58;kk>48;kk--)
  {
    if(!right_baihei_flag&&(imgyiwei[kk][currentzhongjian[58]+14]-imgyiwei[kk+1][currentzhongjian[58]+14]<0))//右跳变
    right_baihei_flag=1;
    if(!left_baihei_flag&&(imgyiwei[kk][currentzhongjian[58]-14]-imgyiwei[kk+1][currentzhongjian[58]-14]<0))//左跳变
      left_baihei_flag=1;
    if(right_baihei_flag&&left_baihei_flag)
      baihei_flag=1;
    if(baihei_flag&&!right_heibai_flag&&(imgyiwei[kk][currentzhongjian[58]+14]-imgyiwei[kk+1][currentzhongjian[58]+14]>0))//右跳变
      right_heibai_flag=1;
    if(baihei_flag&&!left_heibai_flag&&(imgyiwei[kk][currentzhongjian[58]-14]-imgyiwei[kk+1][currentzhongjian[58]-14]>0))//左跳变
      left_heibai_flag=1;
    if(right_heibai_flag&&left_heibai_flag)
    {
          star_lineflag=1;
          break;
    }
  }
}



//*****************************************************main函数*****************************************************
void  main(void)
{
    DisableInterrupts; 
    //初始化摄像头
   ftm_quad_init(FTM2);
    ftm_pwm_init(FTM0,FTM_CH5,10000,0);  //电机初始化
ftm_pwm_init(FTM0,FTM_CH4,10000,0); 
 ftm_pwm_init(FTM1,FTM_CH0,300,4230);//舵机初始化
    Priority_Set();
    camera_init(imgbuff);                                  //这里设定  imgbuff 为采集缓冲区！！！！！！
    pit_init_ms(PIT0,5);
    set_vector_handler(PIT0_VECTORn,PIT0_IRQHandler);   // 设置中断服务函数到中断向量表里
    enable_irq(PIT0_IRQn);                         // 使能PIT中断
    set_vector_handler(PORTA_VECTORn,PORTA_IRQHandler);    //设置PORTA的中断服务函数为 PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn,DMA0_IRQHandler);      //设置DMA0的中断服务函数为 DMA0_IRQHandler
  DMX_chengxuchushihua();
    EnableInterrupts;	   
  //ftm_pwm_duty(FTM1,FTM_CH0,770);
    while(1)
    {  
  // OutData[0]=mubiao_speed;
    //OutData[1]=maichongshu;
   //  OutData[2]=zhankongbi;
     //OutPut_Data();
      camera_get_img();
      //led(LED1, LED_OFF);
 // if(flag_LCD)
 //   LCD_view();
 //   else
 //   {
      if(quanjuCount>200&&quanjuCount<208)
        go=1;
      if(quanjuCount>2000&&quanjuCount<2005)
      star_line=1;
     if(star_line)
   star_line_judg();//起跑线检测
  //  }
 //sendimg((u8 *)imgyiwei, CAMERA_W * CAMERA_H);
   
 
    }
}

/*!
 *  @brief      PORTA中断服务函数
 *  @since      v5.0
 */
void PORTA_IRQHandler()
{
    uint8  n = 0;    //引脚号
    uint32 flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;                                   //清中断标志位

    n = 29;                                             //场中断
    if(flag & (1 << n))                                 //PTA29触发中断
    {
        camera_vsync();
    }
#if 0             //鹰眼直接全速采集，不需要行中断
    n = 28;
    if(flag & (1 << n))                                 //PTA28触发中断
    {
        camera_href();
    }
#endif
}

/*!
 *  @brief      DMA0中断服务函数
 *  @since      v5.0
 */
//void PIT1_IRQHandler()
//{
//shizi_count++;
//}
void DMA0_IRQHandler()
{
camera_dma();
img_extract((u8 *)imgyiwei, imgbuff,CAMERA_SIZE);      //解压图像
}

void PIT0_IRQHandler(void)
{ 
  PIT_Flag_Clear(PIT0);
  if(flag_LCD)
    LCD_view();
else
{
  quanjuCount++;

  if(quanjuCount==5000)
    quanjuCount=0;
  maichongshu=ftm_quad_get(FTM2);
  ftm_quad_clean(FTM2);
    if(star_lineflag&&star_lineflag<20)
      star_lineflag++;
    if(star_lineflag>20)
      star_lineflag=20;
/*****************************************直道判定***********************************************/
    if(abs(my_piancha)<=8&&qvlv_quanju<=10&&qulv_point<10)
      zhidao_flag++;
    else
      zhidao_flag=0;

/***************************************障碍处理***********************************************/ 
if(zhangai_left)
{
L=1;
zhangai_left=0;
}
else
if(zhangai_right)
{
R=1;
zhangai_right=0;
}


if(L&&L<45)
{
L++;
//duojijiaodu=5;
}else
if(R&&R<45)
{
R++;
//duojijiaodu=-5;
}else
L=R=0;
/***********************************图像处理及偏差提取*******************************************/  
 congzhongjianwangliangbian();//图像处理
if(xielv_flag)
{
xielv=-regression(12+control_point,xielv_dian+control_point);//斜率计算
}
if(xielv>18)
xielv=18;
if(xielv<-18)
xielv=-18;
pianchachuli();//偏差处理
/***************************************舵机控制*******************************************/ 
servo();
/***************************************坡道处理*******************************************/ 
if(podao_flag&&podao_flag<60)
podao_flag++;
else
podao_flag=0;
/***************************************速度控制*******************************************/ 
 if(!star_lineflag&&go)
 {
   if(!yunsuflag)
luduan_panduan();
else
if(yunsuflag)
mubiao_speed=dawan_speed;
 }
 else
   if(star_lineflag==20)
   mubiao_speed=0;
speedcontrol(mubiao_speed);

//if(flag_LCD)

}
}
void Priority_Set(void)
{
 // NVIC_SetPriorityGrouping(4);            //设置优先级分组,4bit 抢占优先级,没有亚优先级
  NVIC_SetPriority(PORTA_IRQn,0);         //配置优先级
  NVIC_SetPriority(DMA0_IRQn,1);          //配置优先级
  NVIC_SetPriority(PIT0_IRQn,2);          //配置优先级

}



