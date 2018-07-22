#include "common.h"
#include "include.h"

#define BIN_MAX 0x80   //限制的最大值

Size_t imgsize = {TSL1401_SIZE, 1}; //图像大小
Size_t size    = {TSL1401_SIZE,30}; //显示区域大小

Site_t site1   = { 0, 0};           //显示图像左上角位置
Site_t sitemax = {30,45};           //显示max值
Site_t sitemin = {60,45};           //显示min值
Site_t siteaver= {90,45};           //显示平均值
Site_t sitecnt = {90,30};           //显示黑点个数
Site_t sitel   = { 0,30};           //左边线位置
Site_t sitem   = {30,30};           //中线位置
Site_t siter   = {60,30};           //右边线位置
Site_t siteW   = { 0,45};           //赛道宽度
Site_t dis   = { 0,90};             //赛道宽度
      
void PIT0_IRQHandler(void);        //中断函数
void System_init(void);            //系统初始化
void changliang_init(void);        //常量初始化
void Rampway(void);                //坡道检测
void Stop(void);                   //停车检测

extern struct TSL1401 ccd1;        //ccd
extern struct motor pidl;          //电机pid
extern struct motor pidr;
uint32 dial_ccd,ramp_straight,speed_dial;//记录拨码开关数值
uint16 distance[3];                  //保存红外测距ADC转换结果

extern uint8 CCD_BUFF[TSL1401_MAX*3][TSL1401_SIZE]; //定义存储接收CCD图像的数组  
extern int16 CCD_Value[TSL1401_MAX][TSL1401_SIZE];  //存储差分值，用于巡线

uint16 star,STAR_NUMBER;             //开始不检测起跑线标记
uint8 stop_flag;                     //停标志
int16 a,b,c,d,g;        //速度设定参数、刹车系数（0，或者负）

/***************************************
  修改 PIT0 的定时时间即可修改曝光时间
****************************************/
void  main(void)
{
    System_init();        //系统初始化
    changliang_init();    //常量初始化
    
    led(LED_MAX,LED_ON);
    //不需要修改参数时，选择按键按下启动
    //while(key_check(KEY_A) == 1);//按键按下启动
    //需要修改参数时，选择进入修改参数菜单，修改后的参数只在本次运行中有效，重新上电后无效
    KEY_inter();                   //进入修改菜单，修改参数后启动
    led(LED_MAX,LED_OFF);
    
    uint8 time = 10;      //曝光时间
    tsl1401_set_addrs(TSL1401_MAX,(uint8 *)&CCD_BUFF[0]);//初始化 线性CCD
    tsl1401_init(time);                                  //配置 中断时间为 time
    set_vector_handler(PIT0_VECTORn,PIT0_IRQHandler);    //配置 中断函数 和 使能中断
    
    //读取编码器的值
    ramp_straight  = gpio_get(PTB0);//坡道是否打直
    dial_ccd       = gpio_get(PTB1);//小液晶观测CCD
    speed_dial     = gpio_get(PTB2) * 10 + gpio_get(PTB3);//速度控制
    
    if(speed_dial == 1)        //快速带刹车
    { 
      a=85;b=90;c=95;d=115; g = 20;
    }
    else if(speed_dial == 10) //快速不带刹车
    {
      a=85;b=90;c=95;d=110; g = 30;
    }
    else if(speed_dial == 11) //中速
    {
      a=85;b=90;c=95;d=100; g = 20;
    }
    else if(speed_dial == 0)  //低速
    {
      a=85;b=90;c=90;d=90;  g = 20;
    }
    
    uart_init (UART4, 115200); //串口初始化
    
    DELAY_MS(2000);            //延时发车
    enable_irq(PIT0_IRQn); 
    
    while(1)
    {
      tsl1401_get_img();       //采集 线性CCD 图像 ，函数内部带检测起跑线
      
      if(star < (STAR_NUMBER+50) )
      {
        star++; 
      }
      else 
      {
        star = STAR_NUMBER + 50;//小于倒计时+50 标记数+1
      }
      
      if(star < STAR_NUMBER )
      {
      }
      else 
      { 
        Stop();                 //小于倒计时不检测起跑线
      }
    }
}
/*********************************
        PIT0中断服务函数
 ********************************/  
extern uint8 ramp;
//uint8 ccnt;
void PIT0_IRQHandler(void)
{
  PIT_Flag_Clear(PIT0);  
  
  /*ccnt++;
  if(ccnt == 100)
  {
    //FENG = ~FENG;
    ccnt = 0;
  }*/
  
  tsl1401_time_isr();

  maxvar_a((uint8 *)&CCD_BUFF[0],TSL1401_SIZE,BIN_MAX,&ccd1);                         //限制最大值
  Filter((uint8 *)&CCD_BUFF[0],(uint8 *)&CCD_BUFF[1*TSL1401_MAX+0],TSL1401_SIZE);     //进行滤波
  findMax((uint8 *)&CCD_BUFF[1*TSL1401_MAX+0],&ccd1);                                 //求最大最小值，平均值  
  abs_diff((int16 *)&CCD_Value[0],(uint8 *)&CCD_BUFF[1*TSL1401_MAX+0],TSL1401_SIZE);  //求波形差分  
  
  searchEdge((int16 *)&CCD_Value[0],&ccd1);                                           //寻找边沿
  CaculateMid_Line(&ccd1);                                                            //计算中线
  
  Rampway();                                                                          //坡道检测，必须使用，国赛都有坡道
  //****************   关于障碍的检测   *************************//
  if(ramp <= 50) //如果处于平路时正常检测障碍，处于坡道时，不会有障碍，所以不用检测障碍
  {
    Obstacle_search((int16 *)&CCD_Value[0],&ccd1); //障碍检测
  }
  else
  {
    ccd1.obstacle = 0;    //障碍标志清零
  }
  //************  蜂鸣器示意状态   **************//
  if(ccd1.obstacle != 0 || ccd1.cross[0] == 1 || ramp > 50)
  {
    FENG = 0;
  }
  else
  {
    FENG = 1;
  }
  //***************   进入舵机控制环节   *****************//
  if(ccd1.obstacle == 0 || ramp > 50) //如果无障碍，或者处于坡道中，舵机正常控制
  {
    direction_Control();
  }
  else if(ccd1.obstacle == 01) //如果障碍在右边，加大偏差，偏差可调节
  {
      S3010AngleOB(&ccd1,-12);
  }
  else if(ccd1.obstacle == 10) //如果障碍在左边，加大偏差，偏差可调节
  {
      S3010AngleOB(&ccd1,12);
  }
  //*****************  进入电机控制环节   *****************//
  //如果停车标志位为0 ，即未到终点，正常速度控制
  if(stop_flag == 0) 
  {
    if(dial_ccd == 0)
    {    
      speed_Contorl();
    }
    else if(dial_ccd == 1)
    {
      LCD_Img_gray_Z(site1,size,(uint8 *)&CCD_BUFF[0],imgsize);         //显示图像
      LCD_wave_display(site1,size,(uint8 *)&CCD_BUFF[0],BIN_MAX,BLUE);  //显示滤波后的波形
      //LCD_num_BC(sitemax,ccd1.max,3,RED,BLACK);                       //最大值
      //LCD_num_BC(sitemin,ccd1.min,3,RED,BLACK);                       //最小值
      LCD_num_BC(siteaver,ccd1.aver,3,RED,BLACK);                       //平均值
      //LCD_num_BC(sitecnt,ccd1.obstacle,2,RED,BLACK);                  //障碍
      LCD_num_BC(sitel,ccd1.left_Edge[0],3,RED,BLACK);                  //左边线
      LCD_num_BC(sitem,ccd1.mid_Line[0],3,RED,BLACK);                   //中线
      LCD_num_BC(siter,ccd1.right_Edge[0],3,RED,BLACK);                 //右边线
      LCD_num_BC(siteW,ccd1.width,2,RED,BLACK);                         //赛道宽度
      LCD_num_BC(dis,distance[0],5,RED,BLACK);                          //红外测距数据
      
      M1(0);
      M2(0);
      M3(0);
      M4(0);
    }
    else
    { 
      speed_Contorl();
    }
  }
  //如果停车标志位置位，即到达终点，采取停车策略，
  //理论上PWM最大为0，最小为-9999，但此处实际处理应为：PWM通道交换并且取绝对值  
  else  
  {
    //FENG = 0;
    pidl.setPoint = 0; //速度预期设为0，即停止
    pidr.setPoint = 0;
    Getpluse();        //获取编码器值
    motorPID111();     //电机PID计算,带反转
    
    if(pidl.PWM >= 0)
    {
      M1(0);
      M2(0);
    }
    else
    {
      M2( ABS(pidl.PWM) );
      M1(0);
    }
    
    if(pidr.PWM >=0 )
    {
      M3(0);
      M4(0);
    }
    else
    {
      M4( ABS(pidr.PWM) );
      M3(0);
    }
  }
}
/******************************
    初始化系统
******************************/
void System_init(void)
{  
  gpio_init(PTA26,GPO,1);     //蜂鸣器端口
  //检测起跑线端口初始化
  gpio_init(PTA28,GPI,0);
  gpio_init(PTA29,GPI,0);   
  port_init_NoALT(PTA28,PULLUP);
  port_init_NoALT(PTA29,PULLUP);
  //电机以及舵机初始化
  ftm_pwm_init(MOTOR_FTM, MOTOR1_PWM,MOTOR_HZ,0);      //初始化 电机 PWM
  ftm_pwm_init(MOTOR_FTM, MOTOR2_PWM,MOTOR_HZ,0);      //初始化 电机 PWM
  ftm_pwm_init(MOTOR_FTM, MOTOR3_PWM,MOTOR_HZ,0);      //初始化 电机 PWM
  ftm_pwm_init(MOTOR_FTM, MOTOR4_PWM,MOTOR_HZ,0);      //初始化 电机 PWM
  
  ftm_pwm_init(S3010_FTM, S3010_CH,S3010_HZ,725);      //初始化 舵机 PWM
  //正交编码，已经外部上拉
  ftm_quad_init(FTM1);                                 //FTM1 正交解码初始化（所用的管脚可查 port_cfg.h ）
  ftm_quad_init(FTM2);                                 //FTM2 正交解码初始化（所用的管脚可查 port_cfg.h ）
  
  adc_init(infrared); //红外测距引脚初始化
  
  LCD_init();  
  key_init(KEY_MAX);
  led_init(LED_MAX);
  
  //拨码开关初始化
  gpio_init(PTB0,GPI,0);
  gpio_init(PTB1,GPI,0);
  gpio_init(PTB2,GPI,0);
  gpio_init(PTB3,GPI,0);   
  port_init_NoALT(PTB0,PULLUP);
  port_init_NoALT(PTB1,PULLUP);
  port_init_NoALT(PTB2,PULLUP);
  port_init_NoALT(PTB3,PULLUP);
  
  gpio_init(PTE8,GPI,0);
  gpio_init(PTE9,GPI,0);
  gpio_init(PTE10,GPI,0);
  gpio_init(PTE12,GPI,0);   
  port_init_NoALT(PTE8,PULLUP);
  port_init_NoALT(PTE9,PULLUP);
  port_init_NoALT(PTE10,PULLUP);
  port_init_NoALT(PTE12,PULLUP);
}
/***************************************
  
***************************************/
void changliang_init(void)
{
  STAR_NUMBER = 1600;  //起跑线检测倒计时
  CCD_init();
  PID_init();
}
/**************************************
        坡 道 检 测
***************************************/
uint8 ramp;
void Rampway(void)
{
  distance[0] = adc_once(infrared,ADC_10bit);             //读取红外测距
  
  for(uint8 i=2;i>0;i--)   distance[i] = distance[i-1];  //保存3次红外测距数据
  
  if(ramp == 0)
  {
    if(distance[0]>540 && distance[1]>540 && distance[2]>540 && ccd1.width>50) //连续三次小于阈值
    {
      ramp = 100;    //速度处理倒计时，这个值可以适当调节
      
      if(star> (STAR_NUMBER-100) ) star = STAR_NUMBER-100;//坡道时不检测起跑线，防止误测，这个值要和主程序中值结合
    }
    else
    {
      ramp = 0;
    }
  }
  else
  {
    ramp--;
  }
}
/*******************************************
               停 止 线 检 测
        判断条件比较苛刻，可适当调整
  可以选择手机控制停车，把注释掉的解注即可
********************************************/
void Stop(void)
{
  //char str[100];       //串口接收数组
  
  //检测起跑线程序，加了限制条件，防止误判
  if( /*(gpio_get(PTA28)==1 && gpio_get(PTA29)==1)
       ||*/ (gpio_get(PTE8)==1 && gpio_get(PTE9)==0 && gpio_get(PTE10)==0 && gpio_get(PTE12)==1)
         
       || (gpio_get(PTE8)==1 && gpio_get(PTE9)==1 && gpio_get(PTE10)==0 && gpio_get(PTE12)==1)
       || (gpio_get(PTE8)==1 && gpio_get(PTE9)==1 && gpio_get(PTE10)==0 && gpio_get(PTE12)==0)
         
       || (gpio_get(PTE8)==1 && gpio_get(PTE9)==0 && gpio_get(PTE10)==1 && gpio_get(PTE12)==1)
       || (gpio_get(PTE8)==0 && gpio_get(PTE9)==0 && gpio_get(PTE10)==1 && gpio_get(PTE12)==1)
         
       || (gpio_get(PTE8)==0 && gpio_get(PTE9)==1 && gpio_get(PTE10)==1 && gpio_get(PTE12)==0)
     /*|| (uart_querystr (UART4,str,sizeof(str)-1) != 0)*/)
  {
    if( ccd1.flag_edge[1]==11 
        && (ccd1.width>40&&ccd1.width<60) 
        && (ccd1.mid_Line[0]>45&&ccd1.mid_Line[0]<81) 
        && (distance[0]<520)
       )
    {
      while(1)               //检测到停线，进入死循环，主函数死循环转移到此处
      {
        stop_flag = 1;        //停车标志置位
        tsl1401_get_img1();   //采集 线性CCD 图像 ，函数内部不带检测起跑线，需注意此函数与主函数中采集函数稍有区别
      }
    }
  }
}
