#include "include.h"
//OLED页面计数默认为页面零暂时设为3个页面
uint8 Page_Index=2,Para_Index=1,Light_Tower_Index=0,Para_Checked=0,Para_Choice=0,OLED_Refresh=0;
uint8 Race_Index=0;
uint8 Debug_Index=0;
uint8 Para_Index_Limit=7;       //一页最多有7个变量序号
uint8 Uart_Send=true;
uint8 Debug_Motor_ON;
uint8 Display_Edge=1;
uint8 CCD_Draw_Buff[150];
float reserve=0;

char Debug_Mode[4][12]={"Normal  \0","UpRight\0","NoSpeed\0","NoDirec\0"};  //调试模式 正常 直立 没有速度 ，没有方向

char Light_Tower_Para[4][11]={"OFF      \0","Normal   \0","StartOnly\0","StopOnly \0 "};   //灯塔模式

uint8 light_tower_detected=0;

uint8 SendPara=0;
uint8 SendSD=0;
uint8 SendCCD=0;

//对应不同的页面
char Para_Name[7][12]={"PID_ANGLE.P\0","PID_ANGLE.D\0","PID_SPEED.P\0",
"PID_SPEED.I\0","PID_SPEED.D\0","PID_DIREC.P\0","PID_DIREC.D\0"};

char Para_Name1[7][12]={"Fuzzy_Kp\0","Fuzzy_Kd\0","Hill_Slow\0",
"STurnAngle\0","STurnSpeed\0","BTurnAngle\0","BTurnSpeed\0"};

char Car_Set_Name[7][12]={"Set_Speed\0","Set_Angle\0","Acc_Offset\0","Debug_Mode\0","Light_Tower\0","Protect\0","Threshold\0"};

char Type_Switch[7][15]={"DoubleTurn\0","TurnToSingle\0","TurnToCross\0","TurnToAngle\0","StraightSlow","TurnToBarri\0","HillAngle\0"};


uint8 OverWrite=true;                        //数据重写
uint8 Stop=true,CarStopedJustNow=false;
uint8 Starting=true;                //正在启动
int   Start_Cnt=0;


static float Voltage=0;
static float Voltagetemp;

float Step[6]={0.0001,0.001,0.01,0.1,1.0,10.0};   //默认调节步长为0.01
unsigned char Step_Index=2;
#define Frame_Len 4  //帧长为4


//将PID变量的地址加到指针数组里
float *Control_Para[16]={&PID_ANGLE.P,&PID_ANGLE.D,&PID_SPEED.P,&PID_SPEED.I,&PID_SPEED.D,&PID_TURN.P,&PID_TURN.D,&SetSpeed,
                         &Fuzzy_Kp,&Fuzzy_Kd,&Delta_P,&Delta_D};
float *Variable[20]={&Angle,&Angle_Speed,&Car_Angle,&Middle_Err,&Delt_error,&Turn_Speed,&CarSpeed,&Voltage,&PID_ANGLE.OUT,
                      &PID_SPEED.OUT,&PID_TURN.OUT,&RightMotorOut,&LeftMotorOut,&ControlSpeed,&Distance,&RunTime,&reserve,&reserve,&reserve,&reserve};

uint8 Data_Buf[Frame_Len];

void OLED_Draw_UI()  //画出界面
{ 
   uint8 i;
   if(Page_Index!=4)                                     //第三页显示CCD图像
   {

     Voltage=adc_once(ADC1_SE14,ADC_12bit);//adc = PTB10
     Voltagetemp = Voltage;
     Voltage=Voltage*Voltage_Ratio; //转换为实际电压
     OLED_P6x8Str(0,0,"Voltage=");                          //显示电池电压
     OLED_PrintValueF(48, 0,Voltage,2);                     
     OLED_PrintValueF(72, 0,Step[Step_Index],5);            //显示调节步进值
     if((Page_Index!=3)&&Para_Index==7)
     {
        reverse=1; 
         OLED_P6x8Str(116,0,"EE"); 
        reverse=0;
     }
     else  
     {
        OLED_P6x8Str(116,0,"EE"); 
     }
     OLED_Set_Pos(122,7);
     OLED_P6x8Char(Page_Index+48);                         //写出页面序号
   }
  /////////////////////////////////////////////////////////第0页  PID调节
  if(Page_Index==0)                
  {
    for(i=0;i<7;i++)
    {
      if(i==Para_Index&&Para_Choice==true&&Para_Checked==false)
      {
       reverse=1;
       OLED_P6x8Str(0,i+1,Para_Name[i]);   //将参量名反转显示
       reverse=0;
      }
      else OLED_P6x8Str(0,i+1,Para_Name[i]);

      
      
      if(i==(Para_Index)&&Para_Checked)
      {
        reverse=1;
        OLED_PrintValueF(72, i+1,*Control_Para[i],5);
        reverse=0;
      }
      else  OLED_PrintValueF(72, i+1,*Control_Para[i],5);
      
      OLED_Set_Pos(116,i+1);

    }
  }
  /////////////////////////////////////////////////////////第1页  PID调节
    else if(Page_Index==1)
  {  
     for(i=0;i<7;i++)
    {
     if(i==Para_Index&&Para_Choice==true&&Para_Checked==false)
      {
       reverse=1;
       OLED_P6x8Str(0,i+1,Para_Name1[i]);   //将参量名反转显示
       reverse=0;
      }
      else OLED_P6x8Str(0,i+1,Para_Name1[i]);
    }
      OLED_Set_Pos(116,i+1);
    
       if(Para_Index==0&&Para_Checked)
      {
        reverse=1;
        OLED_PrintValueF(72, 1,Fuzzy_Kp,5);
        reverse=0;
      }
      else 
        OLED_PrintValueF(72, 1,Fuzzy_Kp,5);
       
      if(Para_Index==1&&Para_Checked)
      {
        reverse=1;
        OLED_PrintValueF(72, 2,Fuzzy_Kd,5);
        reverse=0;
      }
      else 
        OLED_PrintValueF(72, 2,Fuzzy_Kd,5);
      
       if(Para_Index==2&&Para_Checked)
      {
        reverse=1;
        OLED_PrintValueF(72, 3,Hill_Slow_Ratio,3);
        reverse=0;
      }
      else 
        OLED_PrintValueF(72, 3,Hill_Slow_Ratio,3);

  

  }
  /////////////////////////////////////////////////////////第2页            查看车身信息
  else if(Page_Index==2)
  {   
    for(i=0;i<7;i++)
    {
      if(i==Para_Index&&Para_Choice==true&&Para_Checked==false)
      {
       reverse=1;
       OLED_P6x8Str(0,i+1,Car_Set_Name[i]);   //将参量名反转显示
       reverse=0;
      }
      else OLED_P6x8Str(0,i+1,Car_Set_Name[i]);

      OLED_Set_Pos(116,i+1);
  
    }  
     //参数的反转显示
      if((0==Para_Index)&&Para_Checked)    //参数0    设定速度
      {
        reverse=1;
        OLED_PrintValueF(72, 1,SetSpeed,3);
        reverse=0;
      }
      else    OLED_PrintValueF(72,1,SetSpeed,3);
    
       if((1==Para_Index)&&Para_Checked)  //参数1     加速前倾角度
      {
        reverse=1;
        OLED_PrintValueI(72,2,Set_Angle);
        reverse=0;
      }
      else    OLED_PrintValueI(72,2,Set_Angle);
         
      if((2==Para_Index)&&Para_Checked)  //参数2    加速度计中值
      {
        reverse=1;
       OLED_PrintValueI(72,3,Acc_Offset);
        reverse=0;
      }
      else    OLED_PrintValueI(72,3,Acc_Offset);
      
      
       if((3==Para_Index)&&Para_Checked)  //参数2    调试模式
      {
        reverse=1;
       OLED_P6x8Str(72, 4,Debug_Mode[Debug_Index]);
        reverse=0;
      }
      else     OLED_P6x8Str(72, 4,Debug_Mode[Debug_Index]);
    
        if((4==Para_Index)&&Para_Checked)  //参数3    灯塔模式
      { 
        reverse=1;
             OLED_P6x8Str(72, 5, Light_Tower_Para[Light_Tower_Index]);  
        reverse=0;
      }
      else  OLED_P6x8Str(72, 5, Light_Tower_Para[Light_Tower_Index]);
      
       if((5==Para_Index)&&Para_Checked)  //参数3  是否保护
      {
        reverse=1;
        if(!Protect)
         OLED_P6x8Str(72, 6,"FALSE");
        else
         OLED_P6x8Str(72, 6,"TRUE ");
        reverse=0;
      }
      else 
      {  
        if(!Protect)
        OLED_P6x8Str(72, 6,"FALSE");
        else
        OLED_P6x8Str(72, 6,"TRUE ");
      }
      

  }
    /////////////////////////////////////////////////////////第3页
  else if(Page_Index==3)
  {
    OLED_P6x8Str(0,1,"CarAngle");
    OLED_PrintValueF(72,1,Car_Angle,3);
    OLED_P6x8Str(0,2,"GyroSpeed");
    OLED_PrintValueF(72, 2,Angle_Speed,3);
    OLED_P6x8Str(0,3,"CarSpeed");
    OLED_PrintValueF(72, 3,CarSpeed,3);
    OLED_P6x8Str(0,4,"Distance");
    OLED_PrintValueF(72, 4,Distance,4);
    OLED_P6x8Str(0,5,"RunTime");
    OLED_PrintValueF(72, 5,RunTime,4);
    OLED_P6x8Str(0,6,"Average_Spd");
    OLED_PrintValueF(72, 6,AverageSpeed,4);    
    
    
    if(Para_Choice==true&&Para_Checked==false)   reverse=1;
    OLED_P6x8Str(0,7,"Encoder_Dis");
    reverse=0;
     
  } 
      /////////////////////////////////////////////////////////第4页
  else if(Page_Index==4)
  {
    OLED_P6x8Str(0,1,"MidError");
    OLED_PrintValueF(72,1,Middle_Err,3);
    OLED_P6x8Str(0,2,"left_adc");
    OLED_PrintValueF(72,2,left_adc,3);
    OLED_P6x8Str(0,3,"right_adc");
    OLED_PrintValueF(72,3,right_adc,3);
    OLED_P6x8Str(0,4,"RoadType");
    OLED_PrintValueF(72,4,RoadType,3);
  }
  else if(Page_Index==5)
  {
     for(i=0;i<7;i++)
    {
      if(i==Para_Index&&Para_Choice==true&&Para_Checked==false)
      {
       reverse=1;
       OLED_P6x8Str(0,i+1,Type_Switch[i]);   //将参量名反转显示
       reverse=0;
      }
      else OLED_P6x8Str(0,i+1,Type_Switch[i]); 
      
      
    
    
    
    } 
  }
  
}


/*
 * 串口中断函数
 */


/*
 * 读拨码开关的值
 */
void Read_Switch() 
{
  if(PTB2_IN==0)  //拨码开关4功能
  {
    if(OLED_Refresh==false)
    {
     OLED_Init();
     Stop=true;
     OLED_Refresh=true;
    }
  }
  else           
  {
    if(OLED_Refresh==true)
    {
      OLED_Refresh=false;
      OLED_Fill(0x00);       
    }
  }

  if(PTB3_IN==0)   //拨码开关3功能
  {
   Uart_Send=true;  
  } 
  else 
  {
    Uart_Send=false;
  }
  
  if(PTB4_IN==0)  //拨码开关2功能
  {
     if(Debug_Motor_ON==false)
     {
       Debug_Motor_ON=true;
     }
      
  }
  else 
  {
     Debug_Motor_ON=false;
  }
 
  if(PTB5_IN==0) //拨码开关1功能
 { /*
   if(SaveData==false)
    {
     SaveData=true;    
    }
 }
  else   
  {
     SaveData=false;
 */ 
  }
} 

//检查启动按钮
void Check_StartPress()
{
   //按键 取消
     if(PTA12_IN==0)
   {
      //去抖
      DELAY_MS(100);
      if(PTA12_IN==0)
      { 
       if(Stop==true)
       { 
         Start_Cnt=3000;
         Starting=true;
         Stop=false; 
         ForceStop=false;
         //把所有速度控制变量清零
         SpeedControlOutOld=0;
         SpeedControlOutNew=0;
         SpeedControlIntegral=0;  
         PID_SPEED.OUT=0;
         Distance=0;
         RunTime=0;
         ControlSpeed=0;
     
       }
       else
       {
        Stop=true;
        CarStopedJustNow=true;     //小车刚停止
       }
      }
      while(PTA12_IN==0);  //直到按键松开再运行
   } 
}

/*
 * 检测按键是否按下
 */
void Check_BottonPress()
{
   //按键1 yes
   if(PTA9_IN==0) 
   {
     //去抖
      DELAY_MS(10);
      if(PTA9_IN==0)
     {    
       if(Para_Index==7) 
       { 
         //EEPROM_Save();
         Para_Index=0; 
       }
       else
       {
        if(Para_Choice==false)  Para_Choice=true;  //  选择参量   
        else
       {
         if(Para_Checked==false) Para_Checked=true;
         else Para_Checked=false;       
        }
       }
      }
      while(PTA9_IN==0); //直到按键松开再运行
   }
   //按键 取消
     if(PTA13_IN==0)
   {
      //去抖
      DELAY_MS(10);
      if(PTA13_IN==0)
      {    
       if(Para_Choice==true) Para_Choice=false; //不选择参量
       if(Para_Checked==true)Para_Checked=false;
       if(Page_Index==4)
       {
         if(Display_Edge) Display_Edge=0;
         else Display_Edge=1;
       }
      }
      while(PTA13_IN==0);  //直到按键松开再运行
   } 
   //按键2 Left_L
     if(PTA10_IN==0)
   {
      //去抖
      DELAY_MS(10);
      if(PTA10_IN==0)
      {
        if(Para_Checked) 
        {
          if(Step_Index==5) 
          Step_Index=5;   //最大的步长为10
          else Step_Index++;
        }
        else 
        { 
          Para_Index=0;
          if(Page_Index==0) Page_Index=5; //当参数没被选中的时候，按左右键翻页
          else Page_Index--;
          OLED_Fill(0);//清屏 
        }
      }
      while(PTA10_IN==0);//直到按键松开再运行
   } 
   //按键6 Right_L
     if(PTA14_IN==0)
   {
      //去抖
      DELAY_MS(10);
      if(PTA14_IN==0)
      {
        if(Para_Checked) 
        {
          if(Step_Index==0) 
           Step_Index=0;//最小的步长为0.0001
          else
          {
            Step_Index--;
          }
        }
        else 
        { 
          Para_Index=0;
          if(Page_Index==5) Page_Index=0;
          else Page_Index++;
         OLED_Fill(0);//清屏 
        }
      }
      while(PTA14_IN==0);      //直到按键松开再运行
   }
   //按键3 up
     if(PTA11_IN==0)
   {
      DELAY_MS(10);
      if(PTA11_IN==0)
      {
        if(Para_Choice==true)
        {
          if(Para_Checked==false)
          {
           if(Para_Index==0) Para_Index=Para_Index_Limit;
           else Para_Index-=1;
          }
          else
          {
              if(Page_Index==0&&Para_Index<=6)                    //修改第0页的参数
            {
              *Control_Para[Para_Index]+=Step[Step_Index];
            }
            
            if(Page_Index==1&&Para_Index<=6)                    //修改第1页的参数
            {
              if(Para_Index==0)Fuzzy_Kp+=Step[Step_Index];
              if(Para_Index==1)Fuzzy_Kd+=Step[Step_Index];
              if(Para_Index==2)Hill_Slow_Ratio+=Step[Step_Index];
            }
            
            if(Page_Index==2&&Para_Index<=6)                    //修改第2页的参数
            {
              if(Para_Index==0) SetSpeed+=Step[Step_Index];         //设定速度    
              if(Para_Index==1) Set_Angle+=(uint8)Step[Step_Index]; //加速角度
              if(Para_Index==2) Acc_Offset+=(int)Step[Step_Index];  //设定中点值            
       
              if(Para_Index==3)                                
              {
               if(Debug_Index==3)Debug_Index=0;
               else Debug_Index++;                   
              }
            
              if(Para_Index==5)
              {
               if(Protect) Protect=0;
               else Protect=1;
              }

            } 

 
                         
          }
        }
      }  
      
     
     
      while(PTA11_IN==0);//直到按键松开再运行  
   }
   //按键4 down
     if(PTA12_IN==0)
   {
      DELAY_MS(10);
      if(PTA12_IN==0)
      {
       if(Para_Choice==true)
        {
          if(Para_Checked==false)
          {             
            if(Para_Index==Para_Index_Limit)Para_Index=0;   //防止序号超出范围
            else  Para_Index+=1; 
          }
           else 
           {
              if(Page_Index==0&&Para_Index<=6)                    //修改第0页的参数
            {
              *Control_Para[Para_Index]-=Step[Step_Index];
            }
             
             if(Page_Index==1&&Para_Index<=6)                    //修改第0页的参数
            {
              if(Para_Index==0)Fuzzy_Kp-=Step[Step_Index];
              if(Para_Index==1)Fuzzy_Kd-=Step[Step_Index];
              if(Para_Index==2)Hill_Slow_Ratio-=Step[Step_Index];
          
             
            }
            
             if(Page_Index==2&&Para_Index<=6)                    //修改第1页的参数
            {
              if(Para_Index==0) SetSpeed-=Step[Step_Index];         //设定速度    
              if(Para_Index==1) Set_Angle-=(uint8)Step[Step_Index]; //加速角度
              if(Para_Index==2) Acc_Offset-=(int)Step[Step_Index];  //设定中点值            
              if(Para_Index==3)                                //调试模式
              {
               if(Debug_Index==0)Debug_Index=3;
               else Debug_Index--;                   
              }
               if(Para_Index==4)                             
              {
                 if(Light_Tower_Index==0)Light_Tower_Index=3;
               else Light_Tower_Index--;        
              }
              if(Para_Index==5)
              {
               if(Protect) Protect=0;
               else Protect=1;
              } 
            } 

             
   
          }
        }
      }

      while(PTA12_IN==0);  //直到按键松开再运行
   }
}

//修改参数
void Modify_Para(uint8 *buff)
{
   uint8 i=0,addr=0;
   float temp;
   for(i=0;i<10;i++)
  {
       BYTE0(temp)=*(uint8*)(buff+addr);
       addr++;
       BYTE1(temp)=*(uint8*)(buff+addr);
       addr++;
       BYTE2(temp)=*(uint8*)(buff+addr);
       addr++;
       BYTE3(temp)=*(uint8*)(buff+addr);
       addr++;
       *Control_Para[i]=temp;
   }
}
//发送控制参数
void Send_Para()
{
  uint8 i=0,ch=0;
  float temp=0;
  uart_putchar(UART4,0x55);
  uart_putchar(UART4,0xaa);
  uart_putchar(UART4,0xaa);
  for(i=0;i<10;i++)
  {
    temp=*Control_Para[i];
    if(temp<0.00001&&temp>-0.00001)
      temp=0;
    else temp=temp+0.000001;
    ch=BYTE0(temp);
    uart_putchar(UART4,ch);
    ch=BYTE1(temp);
    uart_putchar(UART4,ch);
    ch=BYTE2(temp);
    uart_putchar(UART4,ch);
    ch=BYTE3(temp);
    uart_putchar(UART4,ch);
  }
    uart_putchar(UART4,0X0a);;//帧尾
}

void Send_Variable()
{
  uint8 i=0,ch=0;
  float temp=0;
   uint8 Variable_num=15;
  uart_putchar(UART4,0x55);
  uart_putchar(UART4,0xaa);
  uart_putchar(UART4,0xad);
   uart_putchar(UART4,Variable_num);
 for(i=0;i<Variable_num;i++)
  {
    temp=*Variable[i];
    ch=BYTE0(temp);
     uart_putchar(UART4,ch);
    ch=BYTE1(temp);
     uart_putchar(UART4,ch);
    ch=BYTE2(temp);
     uart_putchar(UART4,ch);
    ch=BYTE3(temp);
     uart_putchar(UART4,ch);
  }
    uart_putchar(UART4,0x0d);
}

void Send_Inductor_Adc()  //发送传感器信息
{
  uint8 i=0,ch=0;
  int temp=0;
  int Inductor_Num=2; //有几个电感就写几个，不过未注册版只能用两个
  uart_putchar(UART4,0x55);
  uart_putchar(UART4,0xaa);  //自行实现putc函数，即通过串口发送一个字节的数据
  uart_putchar(UART4,0xac);
  uart_putchar(UART4,Inductor_Num);
  
 for(i=0;i<Inductor_Num;i++)
  {
    temp=Inductor_Adc[i];
    ch=BYTE0(temp);
     uart_putchar(UART4,ch);
    ch=BYTE1(temp);
     uart_putchar(UART4,ch);
  }
 
   uart_putchar(UART4,0x0c);
}



//KEY
