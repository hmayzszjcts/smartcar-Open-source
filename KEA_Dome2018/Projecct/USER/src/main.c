

#include "headfile.h"


/*系统变量初始化部分*/
void PIDStructurePointerInit(PID_Type_Def *PID_Balance, PID_Type_Def *PID_Speed, PID_Type_Def *PID_Path, PID_Type_Def *PID_Diff ){ //PID初始值给定


  PID_Balance->Kp = 8;
  PID_Balance->Kd = 1.2;
  PID_Balance->out = 0;
  
  
  PID_Speed->Kp = 10;
  PID_Speed->Ki = 0.05;
  PID_Speed->out   = 0;
  
  
  PID_Path->Kp = 2.0;
  PID_Path->Kd = 0.0;
  PID_Path->offset = 1.0;
  PID_Path->out    = 0;
  

  PID_Diff->Kp = 15.0;
  PID_Diff->Kd = 2.0;
  PID_Diff->Ki = 0.0; 
  
  PID_Diff->offset = 0.35;
  PID_Diff->out    = 0;
  
  
  for(uint8 n=0; n<10; n++){
    
    PID_Balance->err_k[n] = 0;
    PID_Speed->err_k[n]   = 0;
    PID_Path->err_k[n]    = 0;
    PID_Diff->err_k[n]    = 0;  
    PID_Speed->out_k[n]   = 0;
  };
  
};



void SensorStructurePointerInit(Acc_Type_Def *Acc, Gyro_Type_Def *Gyro, Angle_Type_Def *Angle, Speed_Type_Def *Speed, Inductor_Type_Def *InductorA, Inductor_Type_Def *InductorB){ //结构体指针初始化成员

  
  /*Gyro*/  
  Gyro->X_Final=0;
  Gyro->X_Offset=0;
  Gyro->X_Value[0]=0;
  Gyro->X_Value[1]=0;
  Gyro->X_Value[2]=0;

  Gyro->Z_Final=0;
  Gyro->Z_Offset=0;
  Gyro->Z_Value[0]=0;
  Gyro->Z_Value[1]=0;
  Gyro->Z_Value[2]=0;
  
  InductorA->LeftFinal=0;
  InductorA->RightFinal=0;
  InductorA->LeftValue[0]=0;
  InductorA->LeftValue[1]=0;
  InductorA->LeftValue[2]=0;
  
  InductorB->LeftFinal=0;
  InductorB->RightFinal=0;
  InductorB->LeftValue[0]=0;
  InductorB->LeftValue[1]=0;
  InductorB->LeftValue[2]=0;

  //角度目标值  也就是平衡值  没有转换成角度值 但是是线性关系。
  Angle->PiontValue = 2300;
  
  //速度目标值  没有转换成真实速度  与速度线性关系
  Speed->PiontSpeed = 2000;

  
  Speed->PiontSpeedMax=5000;
 
  Speed->PiontSpeedMin=1500;

  
  
  
  //此处应照车放在赛道上 电感在导线正上方 最大值 电感离导线最远点为最小值 
  InductorA->LeftMax  = 1200;
  InductorA->LeftMin  = 125;
  InductorA->RightMax = 1200;
  InductorA->RightMin = 125;
  
  
  InductorB->LeftMax = 1200;
  InductorB->LeftMin = 125;
  InductorB->RightMax = 1200;
  InductorB->RightMin = 125;
  

  
};


void CarBalanceSystemInit(Gyro_Type_Def *Gyro){//系统初始化

  
  //uart_init(UART1,9600);
 
  gpio_init(H2,GPO,1);
  
  //关于ADC管脚接的东西 在headfile.h里有定义
  adc_init(ADC0_SE0);	
  adc_init(ADC0_SE1);	
  adc_init(ADC0_SE2);	
  adc_init(ADC0_SE3);	
  adc_init(ADC0_SE4);	
  adc_init(ADC0_SE5);	
  adc_init(ADC0_SE6);
  adc_init(ADC0_SE7);
  
  adc_init(ADC0_SE8);		
  adc_init(ADC0_SE9);	
  adc_init(ADC0_SE10);	
  adc_init(ADC0_SE11);	
  adc_init(ADC0_SE12);	
  adc_init(ADC0_SE13);	
  adc_init(ADC0_SE14);	
  adc_init(ADC0_SE15);
  
 
  
  
  OLED_Init();
  
  OLED_ShowString(0,0,"SysInit Finish!");
  OLED_ShowString(0,16,"Car balance2.0V");
  OLED_Refresh_Gram();

  // 使用数字陀螺仪  要初始化IIC
  //IIC_init();
  
  
  //此处陀螺仪中值获取很重要   不过数字陀螺仪默认就是中值 无需获取  
  // 如果数字陀螺仪中值为0  那么  此处 直接赋值0即可
  Gyro->Z_Offset = adc_once(ADC0_SE2, ADC_12bit);
  Gyro->X_Offset = adc_once(ADC0_SE3, ADC_12bit);
  
  
  systick_delay_ms(10);

  //左电机
  ftm_pwm_init(ftm2,ftm_ch2,13000,0);	
  ftm_pwm_init(ftm2,ftm_ch3,13000,0);		
  
  //右电机  
  ftm_pwm_init(ftm2,ftm_ch4,13000,0);		
  ftm_pwm_init(ftm2,ftm_ch5,13000,0);	
  

  systick_delay_ms(10);  
  
  //ftm的计数引脚可查看KEA128_port_cfg.h文件内的FTM0_COUNT_PIN与FTM1_COUNT_PIN的宏定义得知
  //一个ftm同一时间只能实现一种功能，如果用于测速就不能用于输出PWM	
  ///////////////////////////////////////////////////////////////////////////////////////////////
  ftm_count_init(ftm0);   //对E0引脚输入的脉冲进行计数    E0接编码器LSB    
  gpio_init(E1,GPI,0);    //用于判断方向                  C5接编码器DIR
  port_pull(E1);          //IO上拉
  
  ftm_count_init(ftm1);   //对E7引脚输入的脉冲进行计数    E7接编码器LSB
  gpio_init(H6,GPI,0);    //用于判断方向                  H5接编码器DIR
  port_pull(H6);          //IO上拉
  //////////////////////////////////////////////////////////////////////////////////////////////
 

  //定时器中断
  pit_init(pit0,0x9000);//1.539ms
  
  systick_delay_ms(10);  
  
  

};



//绝对值函数   必须要取地址放入形参
void AbsoluteValue(float *Value, float *Return){
  
  if( (*Value) <0) (*Return) = -1*(*Value);
  else *Value = *Return;
}

/*原始数据获取部分*/
void GetAccelerationValue(Acc_Type_Def *Acc){ //获取ACC原始数据
  
  Acc->Z_Value[0] = Acc->Z_Value[1];
  Acc->Z_Value[1] = Acc->Z_Value[2];
  Acc->Z_Value[2] = ACC_Z;//本句是获取当前加速度  上两句都是 上次值和上上次值

  //利用多次值进行滤波  参数自行调整看效果
  Acc->Z_Final = 0.33*Acc->Z_Value[2]+0.33*Acc->Z_Value[1]+0.33*Acc->Z_Value[0];
  
};

void GetAngleSpeedValue(Gyro_Type_Def *Gyro){  //获取Gyro原始数据
  
  Gyro->Z_Value[0] = Gyro->Z_Value[1];
  Gyro->Z_Value[1] = Gyro->Z_Value[2];
  Gyro->Z_Value[2] = GYRO_Z - Gyro->Z_Offset;//如果是数字陀螺仪请给Gyro->Z_Offset赋值  在初始化的时候
  
  Gyro->X_Value[0] = Gyro->X_Value[1]; //前前次
  Gyro->X_Value[1] = Gyro->X_Value[2]; //前次
  Gyro->X_Value[2] = GYRO_X - Gyro->X_Offset; //本次
  
  
};


void GetInductorValue(Inductor_Type_Def *InductorA, Inductor_Type_Def *InductorB){ //获取inductor原始数据
  
  
  InductorA->LeftValue[0] = InductorA->LeftValue[1];
  InductorA->LeftValue[1] = InductorA->LeftValue[2];
  InductorA->LeftValue[2] = L1;//本句是获取当前电感值  L1在haedfile.h文件中有定义 
  
  InductorA->RightValue[0] = InductorA->RightValue[1];
  InductorA->RightValue[1] = InductorA->RightValue[2];
  InductorA->RightValue[2] = L2;//本句是获取当前电感值  L2在haedfile.h文件中有定义 
  
  InductorB->LeftValue[0] = InductorB->LeftValue[1];
  InductorB->LeftValue[1] = InductorB->LeftValue[2];
  InductorB->LeftValue[2] = L3;//本句是获取当前电感值  L3在haedfile.h文件中有定义 

  InductorB->RightValue[0] = InductorB->RightValue[1];
  InductorB->RightValue[1] = InductorB->RightValue[2];
  InductorB->RightValue[2] = L4;//本句是获取当前电感值  L4在haedfile.h文件中有定义 
  
  
};

void GetSpeedValue(Speed_Type_Def *Speed){ //获取Speed原始数据
  
  
  for(uint8 n=0; n<15 ; n++){  //循环误差保存
    
    //存储前14次的值  
    Speed->ActualLiftPulse[n] = Speed->ActualLiftPulse[n+1]; //0~14位赋值
   
    if( n==14 ){ //当前值 15次  最新值
      Speed->ActualLiftPulse[n+1] = ftm_count_get(ftm1)*(-1); //本次 15位赋值  
      
      ftm_count_clean(ftm1);  //清除ftm1计数值  
    }//此处一定要修改好 根据你们实际的硬件  对应好左右轮子  因为编码器是对着安装的  那么
    //在车子往前走的时候  左右是相反的  所以要把其中一个负值编码器变为正的
    //两个电机存在同样的问题  都是对着安装的  
  }
  
  
  
  for(uint8 n=0; n<15 ; n++){  //循环保存
    
    Speed->ActualRightPulse[n] = Speed->ActualRightPulse[n+1]; //0~14位赋值
   
    if( n==14 ){
      Speed->ActualRightPulse[n+1] = ftm_count_get(ftm0);  //本次 15位赋值
      ftm_count_clean(ftm0);  //清除ftm0计数值  
    }
    
  }

  if(gpio_get(H6)) //检测 ftm1的转向   H6  E1是针对左右编码器的  要弄清楚 不要到时候改过以后
    //左边编码器的方向  去控制右边编码器的 正负号   那就完蛋了
    Speed->ActualLiftPulse[15] = Speed->ActualLiftPulse[15];
  else                
    Speed->ActualLiftPulse[15] = -Speed->ActualLiftPulse[15];             
  
  
  if(gpio_get(E1))  //检测 ftm0的转向  
    Speed->ActualRightPulse[15] = Speed->ActualRightPulse[15];
  else                
    Speed->ActualRightPulse[15] = -Speed->ActualRightPulse[15]; //这个负号取决于你车向前走时  编码器值是否为正      
  
};


void GetButtonStatus(Button_Type_Def *Button){ //Button原始数据获取
  
  Button->Up[0] = Button->Up[1];
  Button->Up[1] = Button->Up[2];
  Button->Up[2] = Button->Up[3];
  Button->Up[3] = Button->Up[4];
  Button->Up[4] = UP;//第四次是最新的值  按键值  在haedfile.h文件中有定义
  
  Button->Down[0] = Button->Down[1];
  Button->Down[1] = Button->Down[2];
  Button->Down[2] = Button->Down[3];
  Button->Down[3] = Button->Down[4];
  Button->Down[4] = DOWN;//第四次是最新的值  按键值  在haedfile.h文件中有定义
  
  Button->Life[0] = Button->Life[1];
  Button->Life[1] = Button->Life[2];
  Button->Life[2] = Button->Life[3];
  Button->Life[3] = Button->Life[4];
  Button->Life[4] = LIFE;//第四次是最新的值  按键值  在haedfile.h文件中有定义
  
  Button->Right[0] = Button->Right[1];
  Button->Right[1] = Button->Right[2];
  Button->Right[2] = Button->Right[3];
  Button->Right[3] = Button->Right[4];
  Button->Right[4] = RIGHT;//第四次是最新的值  按键值  在haedfile.h文件中有定义
  
  Button->Enter[0] = Button->Enter[1];
  Button->Enter[1] = Button->Enter[2];
  Button->Enter[2] = Button->Enter[3];
  Button->Enter[3] = Button->Enter[4];
  Button->Enter[4] = ENTER;//第四次是最新的值  按键值  在haedfile.h文件中有定义
 
};


void GetSwitchStatus(Switch_Type_Def *Switch){ //开关原始数据获取
  
  Switch->Bm1[0] = Switch->Bm1[1];
  Switch->Bm1[1] = Switch->Bm1[2];
  Switch->Bm1[2] = SW1; //拨码开关  第四次是最新的值  在haedfile.h文件中有定义
  
  Switch->Bm2[0] = Switch->Bm2[1];
  Switch->Bm2[1] = Switch->Bm2[2];
  Switch->Bm2[2] = SW2; //拨码开关  第四次是最新的值  在haedfile.h文件中有定义
  
  Switch->Bm3[0] = Switch->Bm3[1];
  Switch->Bm3[1] = Switch->Bm3[2];
  Switch->Bm3[2] = SW3; //拨码开关  第四次是最新的值  在haedfile.h文件中有定义
  
  Switch->Bm4[0] = Switch->Bm4[1];
  Switch->Bm4[1] = Switch->Bm4[2];
  Switch->Bm4[2] = SW4; //拨码开关  第四次是最新的值  在haedfile.h文件中有定义
 
  Switch->Bm5[0] = Switch->Bm5[1];
  Switch->Bm5[1] = Switch->Bm5[2];
  Switch->Bm5[2] = SW5; //拨码开关  第四次是最新的值  在haedfile.h文件中有定义
  
  Switch->Bm6[0] = Switch->Bm6[1];
  Switch->Bm6[1] = Switch->Bm6[2];
  Switch->Bm6[2] = SW6; //拨码开关  第四次是最新的值  在haedfile.h文件中有定义
};

/*原始数据处理部分*/
void GyroHighPassFilter(Gyro_Type_Def *Gyro){ //Gyro滤波

  float K =0.58;//权值滤波  看情况调节
  
  Gyro->Z_Value[2] = K * Gyro->Z_Value[2] +  (1-K) * Gyro->Z_Value[1];
  
  Gyro->Z_Final = Gyro->Z_Value[2];
 

  Gyro->X_Value[2] = K * Gyro->X_Value[2] +  (1-K) * Gyro->X_Value[1];
  
  Gyro->X_Final = Gyro->X_Value[2];
  

  //权值滤波 去除高频干扰
};


void AngleHardwareFilter(Angle_Type_Def *Angle){ //角度滤波
  //本函数是直接采集角度的  只有模拟陀螺仪和加速度计进行硬件融合直接解算角度值
  
  Angle->ActualValue[0] = Angle->ActualValue[1];
  Angle->ActualValue[1] = Angle->ActualValue[2];
  Angle->ActualValue[2] = ANGLE; //此处是获取 最新角度值  
  
  //滤波 Angle->FinalValue为最终值
  Angle->FinalValue = 0.6*Angle->ActualValue[2] + 0.2*Angle->ActualValue[1] + 0.2*Angle->ActualValue[0];
  

};

void AngleSoftwareFilter(Acc_Type_Def *Acc, Gyro_Type_Def *Gyro , Angle_Type_Def *Angle){//角度滤波
 //本函数是适用于数字陀螺仪 通过软件互补融合滤波解算角度  
  
  Acc->Z_Final = 0.9*Acc->Z_Value[2] + 0.05*Acc->Z_Value[1] + 0.05*Acc->Z_Value[0];//权值低通滤波
  
  Gyro->Z_Final = 0.92*Gyro->Z_Value[2] + 0.05*Gyro->Z_Value[1] + 0.03*Gyro->Z_Value[0];//权值低通滤波
  
  //加速度  角速度采集
  
  
  //此处参数需要根据  清华方案里的方法去调节参数  看波形法
  Angle->FinalValue = 0.92*(Angle->FinalValue + 0.05*Gyro->Z_Final) + 0.08*(Acc->Z_Final);//融合滤波

  
};


void InductorSoftwareFilter(Inductor_Type_Def *InductorA, Inductor_Type_Def *InductorB, Angle_Type_Def *Angle, Switch_Type_Def *Switch){//电感值归一滤波 

  /*权值滤波 警告无视  */
  InductorA->LeftFinal = 0.8*InductorA->LeftValue[2] + 0.15*InductorA->LeftValue[1] + 0.05*InductorA->LeftValue[0];//-InductorA->LeftMin;
  InductorA->RightFinal = 0.8*InductorA->RightValue[2] + 0.15*InductorA->RightValue[1] + 0.05*InductorA->RightValue[0];//-InductorA->RightMin;
  
  InductorB->LeftFinal = 0.8*InductorB->LeftValue[2] + 0.15*InductorB->LeftValue[1] + 0.05*InductorB->LeftValue[0];
  InductorB->RightFinal = 0.8*InductorB->RightValue[2] + 0.15*InductorB->RightValue[1] + 0.05*InductorB->RightValue[0];


 

  /*数据归一化到0~1000*/
  InductorA->LeftFinal  =  1000*( (float)(InductorA->LeftFinal-InductorA->LeftMin)/(InductorA->LeftMax-InductorA->LeftMin) ); 
  InductorA->RightFinal  = 1000*( (float)(InductorA->RightFinal-InductorA->RightMin)/(InductorA->RightMax-InductorA->RightMin) );
  
  InductorB->LeftFinal  =  1000*( (float)(InductorB->LeftFinal-InductorB->LeftMin)/(InductorB->LeftMax-InductorB->LeftMin) ); 
  InductorB->RightFinal  = 1000*( (float)(InductorB->RightFinal-InductorB->RightMin)/(InductorB->RightMax-InductorB->RightMin) );
  
  if(InductorA->LeftFinal>999)InductorA->LeftFinal=999;
  if(InductorA->LeftFinal<  0)InductorA->LeftFinal=  0;
  if(InductorA->RightFinal>999)InductorA->RightFinal=999;
  if(InductorA->RightFinal< 0)InductorA->RightFinal=   0;
  
  
  if(InductorB->LeftFinal>999)InductorB->LeftFinal=999;
  if(InductorB->LeftFinal<  0)InductorB->LeftFinal=  0;
  if(InductorB->RightFinal>999)InductorB->RightFinal=999;
  if(InductorB->RightFinal< 0)InductorB->RightFinal=   0;
  
  //电感AB  A 是靠外侧的电感  B是靠内侧电感    RL就是左右
  
  InductorA->AndValues = InductorA->LeftFinal+InductorA->RightFinal;//电感和值
  
  InductorA->LeftDerivative = InductorA->LeftValue[2] - InductorA->LeftValue[0]; //左点感变化率
  
  InductorA->RightDerivative= InductorA->RightValue[2] - InductorA->RightValue[0];//右电感变化率
  
  InductorB->LeftDerivative = InductorB->LeftValue[2] - InductorB->LeftValue[0];
  
};


void SpeedSoftwareFilter(Speed_Type_Def *Speed){   //速度低通滤波
    
  /*警告无视*/
  Speed->ActualLiftPulse[15]=0.33*Speed->ActualLiftPulse[15] + 0.33*Speed->ActualLiftPulse[14] + 0.33*Speed->ActualLiftPulse[13];
  Speed->ActualRightPulse[15]=0.33*Speed->ActualRightPulse[15] + 0.33*Speed->ActualRightPulse[14] + 0.33*Speed->ActualRightPulse[13];
  // 6 2 2
  
  //实际速度 = R+L
  Speed->ActualSpeed[0] = Speed->ActualSpeed[1];
  Speed->ActualSpeed[1] = Speed->ActualSpeed[2];
  Speed->ActualSpeed[2] = 
    (Speed->ActualRightPulse[0]+Speed->ActualLiftPulse[0])+
    (Speed->ActualRightPulse[1]+Speed->ActualLiftPulse[1])+                    
    (Speed->ActualRightPulse[2]+Speed->ActualLiftPulse[2])+
    (Speed->ActualRightPulse[3]+Speed->ActualLiftPulse[3])+
    (Speed->ActualRightPulse[4]+Speed->ActualLiftPulse[4])+
    (Speed->ActualRightPulse[5]+Speed->ActualLiftPulse[5])+
    (Speed->ActualRightPulse[6]+Speed->ActualLiftPulse[6])+
    (Speed->ActualRightPulse[7]+Speed->ActualLiftPulse[7])+
    (Speed->ActualRightPulse[8]+Speed->ActualLiftPulse[8])+
    (Speed->ActualRightPulse[9]+Speed->ActualLiftPulse[9])+
    (Speed->ActualRightPulse[10]+Speed->ActualLiftPulse[10])+
    (Speed->ActualRightPulse[11]+Speed->ActualLiftPulse[11])+
    (Speed->ActualRightPulse[12]+Speed->ActualLiftPulse[12])+
    (Speed->ActualRightPulse[13]+Speed->ActualLiftPulse[13])+
    (Speed->ActualRightPulse[14]+Speed->ActualLiftPulse[14])+
    (Speed->ActualRightPulse[15]+Speed->ActualLiftPulse[15]);
  
  //滤波
  Speed->ActualSpeed[2] = 0.85*Speed->ActualSpeed[2]+0.1*Speed->ActualSpeed[1]+0.05*Speed->ActualSpeed[0]; 
  //7 2 1
  
  
  //实际差速  = L-R
  Speed->ActualDifferential[0] = Speed->ActualDifferential[1];
  Speed->ActualDifferential[1] = Speed->ActualDifferential[2];
  Speed->ActualDifferential[2] = Speed->ActualLiftPulse[15]-Speed->ActualRightPulse[15];
  Speed->ActualDifferential[2] = 0.8*Speed->ActualDifferential[2]+0.15*Speed->ActualDifferential[1]+0.05*Speed->ActualDifferential[0];
  //实际差速
  
  
  Speed->ActualInstantaneous[0] = Speed->ActualInstantaneous[1];  
  Speed->ActualInstantaneous[1] = Speed->ActualInstantaneous[2];
  Speed->ActualInstantaneous[2] = (Speed->ActualLiftPulse[15]+Speed->ActualRightPulse[15] + (Speed->ActualLiftPulse[14]+Speed->ActualRightPulse[14])) ;
  //瞬时速度计算
  
  Speed->ActualAcceleration = Speed->ActualInstantaneous[2] - Speed->ActualInstantaneous[0];
  //加速度计算
  
  
  
};


void ButtonLowPassFilter(Button_Type_Def *Button){ //按键低通滤波
  
  Button->Up_Final = Button->Up[0] || Button->Up[1] || Button->Up[2] || Button->Up[3] || Button->Up[4]; 
  //有一个1  最终值就是1 全部为0 最终值才为0
  if(Button->Up_Final==1) Button->Up_Final = 0;//取反    以1为有效值
  else Button->Up_Final = 1; 
  
  
  Button->Down_Final = Button->Down[0] || Button->Down[1] || Button->Down[2] || Button->Down[3] || Button->Down[4]; 
  //有一个1  最终值就是1 全部为0 最终值才为0
  if(Button->Down_Final==1) Button->Down_Final = 0;//取反    以1为有效值
  else Button->Down_Final = 1; 
  
  
  Button->Life_Final = Button->Life[0] || Button->Life[1] || Button->Life[2] || Button->Life[3] || Button->Life[4]; 
  //有一个1  最终值就是1 全部为0 最终值才为0
  if(Button->Life_Final==1) Button->Life_Final = 0;//取反    以1为有效值
  else Button->Life_Final = 1; 
  
  
  Button->Right_Final = Button->Right[0] || Button->Right[1] || Button->Right[2] || Button->Right[3] || Button->Right[4]; 
  //有一个1  最终值就是1 全部为0 最终值才为0
  if(Button->Right_Final==1) Button->Right_Final = 0;//取反    以1为有效值
  else Button->Right_Final = 1; 
  
  
  Button->Enter_Final = Button->Enter[0] || Button->Enter[1] || Button->Enter[2] || Button->Enter[3] || Button->Enter[4]; 
  //有一个1  最终值就是1 全部为0 最终值才为0
  if(Button->Enter_Final==1) Button->Enter_Final = 0;//取反    以1为有效值
  else Button->Enter_Final = 1; 
  
};

void SwitchLowPassFilter(Switch_Type_Def *Switch){ //开关低通滤波
  
  Switch->Bm1_Final = Switch->Bm1[0] || Switch->Bm1[1] || Switch->Bm1[2]; 
  if(Switch->Bm1_Final==1) Switch->Bm1_Final = 0;//取反    以1为有效值
  else Switch->Bm1_Final = 1; 
  
  Switch->Bm2_Final = Switch->Bm2[0] || Switch->Bm2[1] || Switch->Bm2[2];
  if(Switch->Bm2_Final==1) Switch->Bm2_Final = 0;//取反    以1为有效值
  else Switch->Bm2_Final = 1; 
  
  Switch->Bm3_Final = Switch->Bm3[0] || Switch->Bm3[1] || Switch->Bm3[2];
  if(Switch->Bm3_Final==1) Switch->Bm3_Final = 0;//取反    以1为有效值
  else Switch->Bm3_Final = 1; 
  
  Switch->Bm4_Final = Switch->Bm4[0] || Switch->Bm4[1] || Switch->Bm4[2];
  if(Switch->Bm4_Final==1) Switch->Bm4_Final = 0;//取反    以1为有效值
  else Switch->Bm4_Final = 1; 
  
  Switch->Bm5_Final = Switch->Bm5[0] || Switch->Bm5[1] || Switch->Bm5[2];
  if(Switch->Bm5_Final==1) Switch->Bm5_Final = 0;//取反    以1为有效值
  else Switch->Bm5_Final = 1; 
  
  Switch->Bm6_Final = Switch->Bm6[0] || Switch->Bm6[1] || Switch->Bm6[2];
  if(Switch->Bm6_Final==1) Switch->Bm6_Final = 0;//取反    以1为有效值
  else Switch->Bm6_Final = 1; 
};


void OutOfControlSignal(Speed_Type_Def *Speed, PID_Type_Def *PID){   //速度失控信号检测
  
  if( (Speed->ActualSpeed[2]-Speed->ActualSpeed[1]) > 400  &&  Speed->ActualSpeed[2]>10000){ 
    //加速度过大 车轮空转

    
    ftm_pwm_duty(ftm2,ftm_ch2,0);
    ftm_pwm_duty(ftm2,ftm_ch3,0);
    ftm_pwm_duty(ftm2,ftm_ch4,0);
    ftm_pwm_duty(ftm2,ftm_ch5,0);
//    
    
    
    while(1){
      
//     led(LED0,LED_ON);  //根据你们的硬件设置报警灯
      
      systick_delay_ms(80);
      
//      led(LED0,LED_OFF);
      
      systick_delay_ms(80);
    
    };

  }

};



/*被控制量处理部分*/
void BalanceFeedbackControl(PID_Type_Def *PID, Angle_Type_Def *Angle, Gyro_Type_Def *Gyro, Ramp_Type_Def *Ramp){ //直立反馈控制
  
  PID->err = (Angle->PiontValue-Angle->FinalValue);//硬件
  //偏差  =  角度目标-角度反馈值
  
  
  PID->out =   1000 + PID->Kp * PID->err + PID->Kd * Gyro->Z_Final;
  //1000是为了给一个PWM中点   Gyro->Z_Final就是 直立的角速度  也就是D控制  PD
  
  //一般情况下  调直立时   先给一个P参数  D给0   就能立起来   如果发现不能立疯转  
  //多半是 pid方向相反    把(Angle->PiontValue-Angle->FinalValue)  里面调换下位置  即可
  
  

};


void DirectionFeedbackControl(PID_Type_Def *PID, Inductor_Type_Def *InductorA, Inductor_Type_Def *InductorB, Gyro_Type_Def *Gyro, Speed_Type_Def *Speed, Ramp_Type_Def *Ramp){ //方向反馈控制

  
  InductorA->RightFinal = PID->offset*InductorA->RightFinal;  
  InductorA->LeftFinal = (2-PID->offset)*InductorA->LeftFinal;
  //调整零偏
  

  //主要是在用 A电感
  
  PID->err = 10.0*(sqrt(1.0*InductorA->RightFinal)-sqrt(1.0*InductorA->LeftFinal) )/ (InductorA->RightFinal+InductorA->LeftFinal);
  
  
  
   
  if( InductorA->AndValues < 25 && Ramp->Flag[0]!=1) {//丢线找回 且不在坡道
   
    PID->err += 0.55*PID->err_k[6]; //找回强度

  }
  else if( InductorA->AndValues < 50 && Ramp->Flag[0]!=1) {//丢线找回
   
    PID->err += 0.50*PID->err_k[6]; //找回强度
  
  }
  else if( InductorA->AndValues < 75 && Ramp->Flag[0]!=1) {//丢线找回
   
    PID->err += 0.45*PID->err_k[7]; //找回强度
  
  }
  else if( InductorA->AndValues < 100 && Ramp->Flag[0]!=1) {//丢线找回
   
    PID->err += 0.40*PID->err_k[7]; //找回强度
  
  }
  else if( InductorA->AndValues < 125 && Ramp->Flag[0]!=1) {//丢线找回
   
    PID->err += 0.35*PID->err_k[8]; //找回强度
  
  }
  else if( InductorA->AndValues< 150 && Ramp->Flag[0]!=1) {//丢线找回
   
    PID->err += 0.30*PID->err_k[8]; //找回强度
  
  }
  else if( InductorA->AndValues < 175 && Ramp->Flag[0]!=1) {//丢线找回
   
    PID->err += 0.25*PID->err_k[9]; //找回强度9
  
  }
  else if( InductorA->AndValues < 200 && Ramp->Flag[0]!=1) {//丢线找回200
   
    PID->err += 0.2*PID->err_k[9]; //找回强度9
  
  }
  
  if(InductorA->AndValues<150)  PID->err = 0;
  

 
  PID->out = PID->Kp * PID->err * Speed->ActualInstantaneous[2] + PID->Kd * Gyro->X_Final; 
  //转向PID输出计算 Gyro->X_Final; 是转向角速度 也是D控制 
  
  
  Speed->PiontDifferential = (int16)(PID->out);  //给定差速环目标值
  //把转向环  计算出来的输出值  给差速目标环
  
  for(uint8 n=0; n<9 ; n++){  //循环误差保存
    
    PID->err_k[n] = PID->err_k[n+1]; //0~8位赋值
   
    if( n==8 )
      PID->err_k[n+1] = PID->err;  //本次 9位赋值
    
  }
  
};


void DifferentialFeedbackControl(PID_Type_Def *PID, Speed_Type_Def *Speed, Gyro_Type_Def *Gyro){//差速反馈控制
     
  //PID->offset只是一个比例分配  PID->offset=2     
  
  int16 SpeedDifferentialValue = PID->offset * Speed->ActualDifferential[2] + (1-PID->offset)*(-0.18*Gyro->X_Final);
  //  得到差速值  是编码器和   转向角速度共同决定差速反馈值
  
  
  //Speed->PiontDifferential = 0;
  //差速目标-  差速实际值
  PID->err = ( Speed->PiontDifferential -  SpeedDifferentialValue);

  PID->integral += PID->err;
  
  //积分上下限
  if(PID->integral>50000)PID->integral=50000;
  
  if(PID->integral<-50000)PID->integral=-50000;
  
  if(Speed->ActualSpeed[2]< 50)PID->integral = 0;
  //低速积分清零
  
  PID->out =  PID->Kp * PID->err + PID->Kd * ( SpeedDifferentialValue + PID->err_k[0]);// + PID->Ki * PID->integral;
  //PID计算
  
  for(uint8 n=0; n<9 ; n++){  //循环误差保存
    
    PID->err_k[n] = PID->err_k[n+1]; //0~8位赋值
   
    if( n==8 )
      PID->err_k[n+1] = PID->err;  //本次 9位赋值
    
  }
  
 
  PID->err_k[0] = SpeedDifferentialValue;//借用一下PID->err_k[0]咯  存入上次值 差速
  

  //输出幅度限制    
  if(PID->out>2000)PID->out=2000;//1800
  if(PID->out<-2000)PID->out=-2000;


  
};

void SpeedFeedbackControl(PID_Type_Def *PID, Speed_Type_Def *Speed, Ramp_Type_Def *Ramp, Switch_Type_Def *Switch){ //速度反馈控制
  

  PID->out_k[9] = PID->out;//保存成上一次输出值
  
  PID->out_k[0] = 0;
  
  
  
  PID->err = Speed->ActualSpeed[2] - Speed->PiontSpeed;
  
  
  PID->integral = PID->err_k[0]+PID->err_k[1]+PID->err_k[2]+PID->err_k[3]+PID->err_k[4]+
  
          PID->err_k[5]+PID->err_k[6]+PID->err_k[7]+PID->err_k[8]+PID->err_k[9]+PID->err;


 
  PID->out = PID->Kp * PID->err;// + PID->Ki * PID->integral;
  
  PID->out *= 0.05; //缩小速度环输出
  
  
  if(PID->out >  4800) PID->out= 4800; //后退最低角度4200
  
  if(PID->out < -1800) PID->out= -1800;//前进最低角度-1800
 
  

  
  for(uint8 n=0; n<9 ; n++){  //循环误差保存
    
    PID->err_k[n] = PID->err_k[n+1]; //0~8位赋值
   
    if( n==8 )
      PID->err_k[n+1] = PID->err;  //本次 9位赋值
    
  }
  

};

void PWMFeedbackControl(PWM_Type_Def *PWM, PID_Type_Def *PID_Balance, PID_Type_Def *PID_Speed, PID_Type_Def *PID_Path, PID_Type_Def *PID_Diff, Speed_Type_Def *Speed, Switch_Type_Def *Switch){ //PWM叠加反馈控制
  
  float DiffK=1;
 
    
  PID_Speed->out_k[0] += (PID_Speed->out - PID_Speed->out_k[9]) * 0.0625;//分段成1/16分增量
  
  //PID_Speed->out_k[9]是上次值 //PID_Speed->out是本次值  PID_Speed->out_k[0] 是平滑输出增量值 
  //叠加的时候把   平滑输出增量值 + 上次的值 得到16个周期内 每4ms周期应给的值
  
  PWM->PiontValue  = (int16)(PID_Balance->out)+ (int16)(PID_Speed->out_k[0] + PID_Speed->out_k[9]) ;
  //把所有环输出相加
  
  
  PWM->Differential = (int16)(PID_Diff->out);
 //差速PWM
  
  
  if(PWM->Differential>0){
    
    PWM->LiftValue = PWM->PiontValue + (DiffK) * PWM->Differential;
  
    PWM->RightValue = PWM->PiontValue - (2-DiffK) * PWM->Differential;      
    
  }
  
  if(PWM->Differential<0){ //差速小于零时  左边给目标占空比+ 系数*差速
    
    PWM->LiftValue = PWM->PiontValue + (2-DiffK) * PWM->Differential;
  
    PWM->RightValue = PWM->PiontValue - (DiffK) * PWM->Differential;      
    
  }
  
  
  if(PWM->Differential==0){  //差速=0时  左右给一样的目标占空比
    
    PWM->LiftValue = PWM->PiontValue;
    
    PWM->RightValue = PWM->PiontValue;
  }
};

void ButtonFunctionControl(PID_Type_Def *PID_Balance, PID_Type_Def *PID_Speed, PID_Type_Def *PID_Path, PID_Type_Def *PID_Diff, Angle_Type_Def *Angle, Speed_Type_Def *Speed, Inductor_Type_Def *InductorA, Inductor_Type_Def *InductorB, Button_Type_Def *Button, uint8 *Page){ //按键功能实现
  //OLED  按键 调节PID参数用的   
  
  
  if(Button->Life_Final==1)if(*Page>0)(*Page)--;
  
  if(Button->Right_Final==1)if(*Page<135)(*Page)++;

  //page0
  if(*Page<=5){  //UP  kp
    
    if(Button->Up_Final==1 ){
 
      if(PID_Balance->Kp<30) PID_Balance->Kp = PID_Balance->Kp+0.01;
    }
        
    if(Button->Down_Final==1 ){

      if(PID_Balance->Kp>0) PID_Balance->Kp = PID_Balance->Kp-0.01;
    }

  }
 
  if(*Page<=10 && *Page>5){  //UP  kd
    
    if(Button->Up_Final==1){
  
      if(PID_Balance->Kd<30) PID_Balance->Kd = PID_Balance->Kd+0.01;
    }
        
    if(Button->Down_Final==1){
    
      if(PID_Balance->Kd>0) PID_Balance->Kd = PID_Balance->Kd-0.01;
    }
    
  }
  
  if(*Page<=15 && *Page>10){  // angle piont
    
    if(Button->Up_Final==1){
  
      if(Angle->PiontValue<2400) Angle->PiontValue = Angle->PiontValue+1;
    }
        
    if(Button->Down_Final==1){
    
      if(Angle->PiontValue>1600) Angle->PiontValue = Angle->PiontValue-1;
    }
    
  }
   
  if(*Page<=20 && *Page>15){  //speed kp
    
    
    if(Button->Up_Final==1){
   
      if(PID_Speed->Kp<30)PID_Speed->Kp = PID_Speed->Kp+0.1;

    }
      
    
    if(Button->Down_Final==1){      
  
      if(PID_Speed->Kp>0)PID_Speed->Kp = PID_Speed->Kp-0.1;
    
    }
    
   
  }
 
  if(*Page<=25 && *Page>20){  //speed ki
    
    
    if(Button->Up_Final==1){ 
  
      if(PID_Speed->Ki<10)PID_Speed->Ki = PID_Speed->Ki+0.001;
    }
        
    
    if(Button->Down_Final==1){  
 
      if(PID_Speed->Ki>0)PID_Speed->Ki = PID_Speed->Ki-0.001;
    }

    
  }
  
  if(*Page<=30 && *Page>25){  //speeed piont
    
    if(Button->Up_Final==1){
  
      if(Speed->PiontSpeed<10000)Speed->PiontSpeed = Speed->PiontSpeed+10;
    }
        
    if(Button->Down_Final==1){

      if(Speed->PiontSpeed>0)Speed->PiontSpeed = Speed->PiontSpeed-10;
    }
    
    
  }

  
  
  
  //page1
  if(*Page<=35 && *Page>30){  //path kp
    
    if(Button->Up_Final==1){

      if(PID_Path->Kp<30)PID_Path->Kp = PID_Path->Kp+0.01;
    }
        
    if(Button->Down_Final==1 ){

      if(PID_Path->Kp>0)PID_Path->Kp = PID_Path->Kp-0.01;
    }

    
  }
  
  if(*Page<=40 && *Page>35){  //path kd 
    
    
    if(Button->Up_Final==1){

      if(PID_Path->Kd<30)PID_Path->Kd = PID_Path->Kd+0.01;
    }
        
    if(Button->Down_Final==1){
   
      if(PID_Path->Kd>0)PID_Path->Kd = PID_Path->Kd-0.01;
    }

  }
  
  if(*Page<=45 && *Page>40){  //path piont
    
    
    if(Button->Up_Final==1){
      
      if(PID_Path->offset<2)PID_Path->offset = PID_Path->offset+0.001;
    
    }
      
    if(Button->Down_Final==1){
    
      if(PID_Path->offset>0)PID_Path->offset = PID_Path->offset-0.001;
    
    }
    
    
  }
  
  if(*Page<=50 && *Page>45){  //diff kp
    
    if(Button->Up_Final==1){

      if(PID_Diff->Kp<30)PID_Diff->Kp = PID_Diff->Kp+0.01;
    }
        
    if(Button->Down_Final==1 ){

      if(PID_Diff->Kp>0)PID_Diff->Kp = PID_Diff->Kp-0.01;
    }

    
  }
  
  if(*Page<=55 && *Page>50){  //diff kd
    
    if(Button->Up_Final==1){

      if(PID_Diff->Kd<30)PID_Diff->Kd = PID_Diff->Kd+0.01;
    }
        
    if(Button->Down_Final==1 ){

      if(PID_Diff->Kd>0)PID_Diff->Kd = PID_Diff->Kd-0.01;
    }

    
  }
   
  if(*Page<=60 && *Page>55){  //diff kd
    
    if(Button->Up_Final==1){

      if(PID_Diff->Ki<30)PID_Diff->Ki = PID_Diff->Ki+0.01;
    }
        
    if(Button->Down_Final==1 ){

      if(PID_Diff->Ki>0)PID_Diff->Ki = PID_Diff->Ki-0.01;
    }

    
  }
  
  if(*Page<=65 && *Page>60){  //diff offset
    
    if(Button->Up_Final==1){

      if(PID_Diff->offset<1)PID_Diff->offset = PID_Diff->offset+0.001;
    }
        
    if(Button->Down_Final==1 ){

      if(PID_Diff->offset>0)PID_Diff->offset = PID_Diff->offset-0.001;
    }

    
  }
  
  
 
  
  
  //page2
  if(*Page<=70 && *Page>65){ //左电感最大值
    
    if(Button->Up_Final==1 && InductorA->LeftMax < 4000)InductorA->LeftMax ++;
        
    
    if(Button->Down_Final==1 && InductorA->LeftMax > 100)InductorA->LeftMax --;
    
    
  }
  
  if(*Page<=75 && *Page>70){ //左电感最小值
    
    
    if(Button->Up_Final==1 && InductorA->LeftMin < 4000)InductorA->LeftMin ++;
        
    
    if(Button->Down_Final==1 && InductorA->LeftMin > 100)InductorA->LeftMin --;
    
  }
  
  if(*Page<=80 && *Page>75){ //右电感最大值
    
    if(Button->Up_Final==1 && InductorA->RightMax < 4000)InductorA->RightMax ++;
        
    
    if(Button->Down_Final==1 && InductorA->RightMax > 100)InductorA->RightMax --;
    
    
  }
  
  if(*Page<=85 && *Page>80){ //右电感最小值
    
    
    if(Button->Up_Final==1 && InductorA->RightMin< 4000)InductorA->RightMin ++;
        
    
    if(Button->Down_Final==1 && InductorA->RightMin> 100)InductorA->RightMin --;
    
  }
  
  if(*Page<=90 && *Page>85){ //中间电感最大值
    
    
    if(Button->Up_Final==1 && InductorB->LeftMax< 4000)InductorB->LeftMax ++;
        
    
    if(Button->Down_Final==1 && InductorB->LeftMax> 100)InductorB->LeftMax --;
    
  }
  
  if(*Page<=95 && *Page>90){//中间电感最小值
    
    
    if(Button->Up_Final==1 && InductorB->LeftMin< 4000)InductorB->LeftMin ++;
        
    
    if(Button->Down_Final==1 && InductorB->LeftMin> 100)InductorB->LeftMin --;
    
  }
  
  if(*Page<=100 && *Page>95){//最大速度
    
    
    if(Button->Up_Final==1 && Speed->PiontSpeedMax<10000)Speed->PiontSpeedMax ++;
        
    
    if(Button->Down_Final==1 && Speed->PiontSpeedMax >0)Speed->PiontSpeedMax --;
    
  }
  
  if(*Page<=105 && *Page>100){//最小速度
  
    if(Button->Up_Final==1 && Speed->PiontSpeedMin < Speed->PiontSpeedMax)Speed->PiontSpeedMin ++;
        
    
    if(Button->Down_Final==1 && Speed->PiontSpeedMin > 0)Speed->PiontSpeedMin --;
    
    
  }

  
//  
//  //page3
//  if(*Page<=110 && *Page>105){//减速距离
//    
//    if(Button->Up_Final==1 && Speed->SlowDistance < 6000)Speed->SlowDistance ++;
//        
//    
//    if(Button->Down_Final==1 && Speed->SlowDistance > 0)Speed->SlowDistance --;
//  }
//  
//  if(*Page<=115 && *Page>110){//加速距离
//    
//    if(Button->Up_Final==1 && Speed->AccDistance < 6000)Speed->AccDistance ++;
//        
//    
//    if(Button->Down_Final==1 && Speed->AccDistance > 0)Speed->AccDistance --;
//  }
//  
//  if(*Page<=120 && *Page>115){//错误距离
//    
//    if(Button->Up_Final==1 && Speed->FaultDistance < 150000)Speed->FaultDistance+=100;
//        
//    
//    if(Button->Down_Final==1 && Speed->FaultDistance > 100)Speed->FaultDistance-=100;
//  }
  
};


/*输出量处理部分*/
void PrintParameterToOLED(PID_Type_Def *PID_Balance, PID_Type_Def *PID_Speed, PID_Type_Def *PID_Path, PID_Type_Def *PID_Diff, Angle_Type_Def *Angle, Speed_Type_Def *Speed , PWM_Type_Def *PWM, Inductor_Type_Def *InductorA, Inductor_Type_Def *InductorB, Curve_Type_Def *Curve, uint8 Page){    //打印参数到OLED上

  uint8 mode[10];
  static uint8 Last=0;
  
  if(Page<=30){    
  
    if(Page<=5) mode[0] = 0;
    else mode[0] = 1;
    
    if(Page<=10 && Page>5) mode[1] =0;
    else mode[1] = 1;
    
    if(Page<=15 && Page>10) mode[2] =0;
    else mode[2] = 1;
    
    if(Page<=20 && Page>15) mode[3] =0;
    else mode[3] = 1;
    
    if(Page<=25 && Page>20) mode[4] =0;
    else mode[4] = 1;
    
    if(Page<=30 && Page>25) mode[5] =0;
    else mode[5] = 1;
    
    if( Last!=1 ){
      OLED_Fill(0,0,127,63,0);
      Last = 1;
    }
    OLED_ShowString(0,0,"UP P    D         ");
    OLED_ShowNum(36,0,(int)(PID_Balance->Kp*10),3,12,mode[0]);  
    OLED_ShowNum(72,0,(int)(PID_Balance->Kd*10),3,12,mode[1]); 
    OLED_ShowString(0,12 ,"AnglePiont        "); 
    OLED_ShowNum(96,12,(int)(Angle->PiontValue),4,12,mode[2]);  
    
    OLED_ShowString(0,24 ,"Speed P   I        "); 
    OLED_ShowNum(56,24,(int)(PID_Speed->Kp*10),3,12,mode[3]);  
    OLED_ShowNum(96,24,(int)(PID_Speed->Ki*1000),3,12,mode[4]);  
    OLED_ShowString(0,36 ,"SpeedPiont        ");
    OLED_ShowNum(96,36,(int)(Speed->PiontSpeed),4,12,mode[5]);
      

    OLED_ShowNum(32,48,(int)(InductorA->LeftFinal),3,12,0);  
    OLED_ShowNum(56,48,(int)(InductorA->RightFinal),3,12,0);
    OLED_ShowNum(86,48,(int)(InductorB->LeftFinal),3,12,0); 
  }  
  
  else if(Page>30 && Page<=65){
    
    if(Page<=35 && Page>30) mode[0] = 0;
    else mode[0] = 1;
    
    if(Page<=40 && Page>35) mode[1] =0;
    else mode[1] = 1;
    
    if(Page<=45 && Page>40) mode[2] =0;
    else mode[2] = 1;
    
    if(Page<=50 && Page>45) mode[3] =0;
    else mode[3] = 1;
    
    if(Page<=55 && Page>50) mode[4] =0;
    else mode[4] = 1;
    
    if(Page<=60 && Page>55) mode[5] =0;
    else mode[5] = 1;
    
    if(Page<=65 && Page>60) mode[6] =0;
    else mode[6] = 1;
    
    
    
    if( Last!=2 ){
      OLED_Fill(0,0,127,63,0);
      Last = 2;
    }
    
    OLED_ShowString(0,0,"Path P    D         ");
    OLED_ShowNum(48,0,(int)(PID_Path->Kp*10),3,12,mode[0]);  
    OLED_ShowNum(96,0,(int)(PID_Path->Kd*100),3,12,mode[1]); 
    OLED_ShowString(0,12 ,"PathPiont        "); 
    OLED_ShowNum(84,12,(int)(PID_Path->offset*100),3,12,mode[2]);  
    
      
    OLED_ShowString(0,24 ,"Diff P     D        "); 
    OLED_ShowNum(56,24,(int)(PID_Diff->Kp*10),3,12,mode[3]);  
    OLED_ShowNum(96,24,(int)(PID_Diff->Kd*10),3,12,mode[4]);  
    OLED_ShowString(0,36 ,"Diff I     S        ");
    OLED_ShowNum(56,36,(int)(PID_Diff->Ki*10),3,12,mode[5]);
    OLED_ShowNum(96,36,(int)(PID_Diff->offset*100),3,12,mode[6]); //差速与陀螺仪融合比率
      
      

    OLED_ShowNum(32,48,(int)(InductorA->LeftFinal),3,12,0);  
    OLED_ShowNum(56,48,(int)(InductorA->RightFinal),3,12,0);
    OLED_ShowNum(86,48,(int)(InductorB->LeftFinal),3,12,0); 
      
  }
  
  else if(Page>65 && Page<=105){
    
    if(Page<=70 && Page>65) mode[0] = 0;
    else mode[0] = 1;
    
    if(Page<=75 && Page>70) mode[1] =0;
    else mode[1] = 1;
    
    if(Page<=80 && Page>75) mode[2] =0;
    else mode[2] = 1;
    
    if(Page<=85 && Page>80) mode[3] =0;
    else mode[3] = 1;
    
    if(Page<=90 && Page>85) mode[4] =0;
    else mode[4] = 1;
    
    if(Page<=95 && Page>90) mode[5] =0;
    else mode[5] = 1;
    
    if(Page<=100 && Page>95) mode[6] =0;
    else mode[6] = 1;
    
    if(Page<=105 && Page>100) mode[7] =0;
    else mode[7] = 1;
    
    
    if( Last!=3 ){
      OLED_Fill(0,0,127,63,0);
      Last = 3;
    }
    
    OLED_ShowString(0,0   ,"LAMAX");
    OLED_ShowNum(38,0,(int)(InductorA->LeftMax),4,12,mode[0]);
    OLED_ShowString(68,0  ,"LAMIN");
    OLED_ShowNum(108,0,(int)(InductorA->LeftMin),3,12,mode[1]);
    
    
    OLED_ShowString(0,12  ,"RAMAX");
    OLED_ShowNum(38,12,(int)(InductorA->RightMax),4,12,mode[2]);
    OLED_ShowString(68,12 ,"RAMIN");
    OLED_ShowNum(108,12,(int)(InductorA->RightMin),3,12,mode[3]);
    
    
    
    OLED_ShowString(0,24  ,"ZBMAX");
    OLED_ShowNum(38,24,(int)(InductorB->LeftMax),4,12,mode[4]);
    OLED_ShowString(68,24 ,"ZBMIN");
    OLED_ShowNum(108,24,(int)(InductorB->LeftMin),3,12,mode[5]);
    
    
    OLED_ShowString(0,36 ,"SpeedMax");
    OLED_ShowNum(84,36,(uint32)(Speed->PiontSpeedMax),4,12,mode[6]);
    
    OLED_ShowString(0,48 ,"SpeedMin");
    OLED_ShowNum(84,48,(uint32)(Speed->PiontSpeedMin),4,12,mode[7]);
    
  }
  
  OLED_Refresh_Gram();

};



void MotorControlFlow(PWM_Type_Def *PWM, Ramp_Type_Def *Ramp){ //电机直接控制
  

  //如测试线序 在此处 对*PWM赋值 测试完毕再注释

  //PWM->LiftValue = 1200;//左轮正传  往前走
  //PWM->LiftValue = 1000;
  //PWM->LiftValue = 800;
  
  //PWM->RightValue = 1200;//右轮正传 往前走
  //PWM->RightValue = 1000;
  //PWM->RightValue = 800;

  //左边右边都给 1000的时候 应该是不转的   
  
  if(PWM->LiftValue>(PWMMAX))PWM->LiftValue = (PWMMAX);
  if(PWM->LiftValue<(PWMMIN))PWM->LiftValue = (PWMMIN);
  if(PWM->RightValue>(PWMMAX))PWM->RightValue = (PWMMAX);
  if(PWM->RightValue<(PWMMIN))PWM->RightValue = (PWMMIN);
  
  //此处必须先调整好  保证 PWM值是 >1000时  是前进
  //PWM值< 1000 是后退   而且 两轮必须一致
  //能改硬件 就改驱动线  和 电机引线  例如 
  //给同样都是  1200  左边电机慢速往前  右边慢速往后 这时候 调换右电机线
  //如果出现如下情况  给1200  理论上是 正转 20%占空比  但出现转速很快 说明
  //驱动线序不对   可以在这里调换 通道  或者 直接改线序 
  
  if(PWM->LiftValue>1000){ //边轮前进  1000~2000
     ftm_pwm_duty(ftm2,ftm_ch2,0);
     ftm_pwm_duty(ftm2,ftm_ch3,PWM->LiftValue-1000); 
  }
  else if(PWM->LiftValue<1000){ //左轮是后退  0~1000
     ftm_pwm_duty(ftm2,ftm_ch2,1000-PWM->LiftValue); 
     ftm_pwm_duty(ftm2,ftm_ch3,0);    
  }  //如果发现 此处给定的是  控制 右轮转动 则调换下硬件驱动线线序  注意 是两根两根的调换  
   

  if(PWM->RightValue>1000){ //右轮是前进  1000~2000
     ftm_pwm_duty(ftm2,ftm_ch4,0);
     ftm_pwm_duty(ftm2,ftm_ch5,PWM->RightValue-1000); 
  }
  else if(PWM->RightValue<1000){ //右轮是后退  0~1000
     ftm_pwm_duty(ftm2,ftm_ch4,1000-PWM->RightValue);
     ftm_pwm_duty(ftm2,ftm_ch5,0);   
  }   //如果发现 此处给定的是  控制 左边轮转动 则调换下硬件驱动线线序  注意 是两根两根的调换   
  
};

uint8 timer3ms = 0, timer4_5ms=0,timer24ms=0 ,timer64ms=0;

int main(void){
  
  //由于本程序闭环较多  调试过程比较繁琐  建议一开始的时候 屏蔽掉其他环
  //只调节最基础的环节  平衡车主要就是对于两个电机的控制  其中有一个函数
  //MotorControlFlow函数是直接控制电机的  这个函数需要调试好  其要求就是
  //占空比0~1000退后   1000~2000向前  通过调换驱动线  和 调换电机线  或者
  //调换程序中 ch2 3 4 5 的  23顺序  45顺序来  达到最终的效果  两个电机 
  //给1000以上是向前走   1000一下向后走  2000往前最快   0往后最快
  
  //当电机输出调整好了以后  再调节编码器反馈  我们在MotorControlFlow函数里
  //直接给  PWM->LiftValue = 1200;PWM->RightValue = 1200;  让两个电机固定占空比
  //转动  这时候 通过IAR在线调试观察  编码器测速值 是否正确  是否对应左轮右轮
  //(左电机转动对应左编码器  右电机转动对应右边编码器  不能错！)
  //用手阻止一个电机观察是否是那个电机的编码器转速下降  确保 编码器的方向一致！
  
  //编码器 和 电机调整好以后就方便了很多  接下来就是直立  前提是陀螺仪参数已经整定好
  //我们调试直立的时候  要把其他环都关闭 方法如下  函数PWMFeedbackControl 是把所有环输出
  //整合到电机PWM上的函数 其中  PWM->PiontValue  = (int16)(PID_Balance->out)+ (int16)(PID_Speed->out_k[0] + PID_Speed->out_k[9]) ;
  //这句话是把  三个环的输出 叠加在一起  我们只需要保留 直立环  这样调节直立比较顺手
  //其中还有一个 差速 也要给0  对直立就无影响   
  
  //当直立调节完毕后  速度环和直立的融合是比较难的  直立和速度是并环 需要精心调试，也可以
  //跳过速度环的调试  直接进入转向环  与差速环    他们之间是串级PID的关系  
  
  //如果出现中断  跑飞 或者  卡住在ADC死循环里  说明堆栈溢出  调整KEA128.ICF文件的  堆栈值 扩大  不超过12K即可
  
 
 
  
  get_clk();              //获取时钟频率 必须执行
  
  uint8 Page=0;
  
  PID_Type_Def  PID_Balance, PID_Speed, PID_Path, PID_Diff;
  
  RUN_Type_Def       RUN;
  Stop_Type_Def      Stop;
  Ramp_Type_Def      Ramp; 
  Curve_Type_Def     Curve;
  Acc_Type_Def       Acc;
  Gyro_Type_Def      Gyro;
  Angle_Type_Def     Angle;
  PWM_Type_Def       PWM;
  Speed_Type_Def     Speed;
  Button_Type_Def    Button;
  Switch_Type_Def    Switch;
  Inductor_Type_Def  InductorA, InductorB;
 
  
  SensorStructurePointerInit( &Acc, &Gyro, &Angle, &Speed, &InductorA, &InductorB );//传感器参数初始化
  PIDStructurePointerInit( &PID_Balance, &PID_Speed, &PID_Path, &PID_Diff );//PID参数初始化
  CarBalanceSystemInit( &Gyro ); //系统初始化  
  


  
  RUN.Flag[4] = 1;//运行出界标志位
  
  RUN.Flag[5] = 0;//启动电机标志位
  
  Stop.Flag[0] = 0;//停止线标志位延时启动标志位
  
  Stop.Flag[1] = 0;//停止标志位
  
  Ramp.Flag[0] = 0;//接近坡道标志位
  
  Ramp.Flag[1] = 0;//上坡道标志位
  
  Curve.Flag[1] = 0;//正差速标志位
  
  Curve.Flag[2] = 0;//反差速标志位
  
  Curve.Flag[3] = 0;//差速数目
  
  while(1){

    if( timer4_5ms  ){
      
        gpio_set(H2,1);//
      
      GetAccelerationValue( &Acc );  //加速度原始数据采集
      GetAngleSpeedValue( &Gyro );   //角速度原始数据采集
      GetSpeedValue( &Speed );       //获取原始速度数据
      GetInductorValue( &InductorA, &InductorB );//电感电压原始数据采集
      
      GyroHighPassFilter( &Gyro );  //角速度滤波处理
      AngleHardwareFilter( &Angle );//角度滤波处理
      SpeedSoftwareFilter( &Speed );//速度递归权值滤波
      InductorSoftwareFilter( &InductorA, &InductorB, &Angle, &Switch);//电感滤波处理

      BalanceFeedbackControl( &PID_Balance, &Angle, &Gyro, &Ramp );//平衡度反馈计算
      DirectionFeedbackControl( &PID_Path, &InductorA, &InductorB, &Gyro, &Speed, &Ramp );//路径反馈计算
      DifferentialFeedbackControl( &PID_Diff, &Speed, &Gyro);//差速反馈计算

      PWMFeedbackControl( &PWM, &PID_Balance, &PID_Speed, &PID_Path, &PID_Diff, &Speed, &Switch );//PWM叠加 直立 方向 速度PWM
      MotorControlFlow( &PWM, &Ramp ); //电机PWM输出控制
      
        gpio_set(H2,0);//
      
      timer4_5ms = 0;//清除时间标志位
    }
    
    //车子跑起来以后就不会执行  因为开销比较大 影响PID控制周期
    if( Speed.ActualSpeed[2]<100 && timer24ms && timer4_5ms != 1 ){ 
      
      GetButtonStatus( &Button ); //获取按键状态
      ButtonLowPassFilter( &Button );//判断按键状态
      
      
      ButtonFunctionControl( &PID_Balance, &PID_Speed, &PID_Path, &PID_Diff, &Angle, &Speed, &InductorA, &InductorB, &Button, &Page );//实现按键状态
      
      GetSwitchStatus( &Switch ); //获取开关状态
      SwitchLowPassFilter( &Switch );//判断开关状态

      timer24ms = 0;

    };
    
    
    if(timer64ms && timer4_5ms != 1 ){  
      
      if(Speed.ActualSpeed[2]<100)PrintParameterToOLED( &PID_Balance, &PID_Speed, &PID_Path, &PID_Diff, &Angle, &Speed, &PWM, &InductorA, &InductorB, &Curve, Page );
      //人机交互界面 高速运行不执行
     
      SpeedFeedbackControl( &PID_Speed, &Speed, &Ramp, &Switch ); //速度环反馈控制
      

       timer64ms = 0;
    };
    
    
  }
  
}




uint8 count = 0;

void PIT_CH0_IRQHandler(void)
{
 
  if( count<84 ) count++;
  else count = 0;
   
  
//  if( count%2==0 ) timer3ms = 1;
  
  if( count%3==0 ) timer4_5ms = 1;
  
  if( count%16==0 ) timer24ms = 1;
  
  if( count%42==0 ) timer64ms=1;
 
  
  PIT_FlAG_CLR(pit0);//清除中断标志位
  

  
}
