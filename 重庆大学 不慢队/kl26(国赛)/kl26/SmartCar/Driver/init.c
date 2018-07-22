#include "include.h"
void Para_Init();
void GPIO_Init();
void Fix_Offset();
void All_Init()
{   
   GPIO_Init(); 
   I2C_Init(); //I2C引脚以及传感器初始化
   Read_Switch();
   tpm_pwm_init(TPM0,TPM_CH1,10000,0);  //PWM初始化 
   tpm_pwm_init(TPM0,TPM_CH2,10000,0); 
   tpm_pwm_init(TPM0,TPM_CH4,10000,0); 
   tpm_pwm_init(TPM0,TPM_CH5,10000,0); 
   adc_init(ADC0_SE8); //ccd
   adc_init(ADC0_SE14);//电压采集
   pit_init_ms(PIT0,1); //1ms定时中断
   Para_Init();         //参数初始化
   tpm_pulse_init(TPM1,TPM_CLKIN0,TPM_PS_1);  //编码盘计数
   tpm_pulse_init(TPM2,TPM_CLKIN1,TPM_PS_1);  //编码盘计数
   uart_init(UART0,115200); 
   uart_rx_irq_en(UART0);//使能串口接收中断 
   OLED_Init();
   Fix_Offset();
   EEPROM_init();
   spi_init(SPI1,SPI_PCS0,MASTER,200*1000);   //初始化为10k低速率
   if(SD_Initialize())
   {
     SD_OK=1; 
   }
}

//参数赋值初始化
void Para_Init()
{
 PID_ANGLE.P=0.10;
 PID_ANGLE.D=0.01;
 PID_SPEED.P=1.5;
 PID_SPEED.I=0.05;
 PID_TURN.P=0.01;
 PID_TURN.D=0.005; 
 Fuzzy_Kp=0.005;
 Fuzzy_Kd=0.0005;
 SetSpeed=3.8;
 Set_Angle=50;
 Threshold=30;
 Acc_Offset=230;
 Hill_Slow_Ratio=0.6;
 Strong_Turn_Angle=40;
 Strong_Turn_Speed=60;
 CCD_Offset=200;
}
//GPIO初始化
void GPIO_Init()
{
  //灯塔
  lptmr_pulse_init(LPT0_ALT2,0xFFFF,LPT_Rising);
  gpio_init(PTB10,GPO,0); 
  //拨码开关
  gpio_init(PTB3,GPI,0);                   
  port_init_NoALT(PTB3,PULLUP);
  gpio_init(PTB7,GPI,0);                     
  port_init_NoALT(PTB7,PULLUP);
  gpio_init(PTB8,GPI,0);                     
  port_init_NoALT(PTB8,PULLUP);
  gpio_init(PTB9,GPI,0);                     
  port_init_NoALT(PTB9,PULLUP);
  //按键输入
  gpio_init(PTA4,GPI,0);                    
  port_init_NoALT(PTA4,PULLUP); 
  gpio_init(PTA5,GPI,0);                     
  port_init_NoALT(PTA5,PULLUP);
  gpio_init(PTA6,GPI,0);                     
  port_init_NoALT(PTA6,PULLUP);
  gpio_init(PTA7,GPI,0);                     
  port_init_NoALT(PTA7,PULLUP);
  gpio_init(PTA12,GPI,0);                     
  port_init_NoALT(PTA12,PULLUP);
  gpio_init(PTA13,GPI,0);                     
  port_init_NoALT(PTA13,PULLUP);
  //LED初始化&扩展接口初始化
  gpio_init(PTE26,GPO,0);  //extern
  gpio_init(PTE29,GPO,0);
  gpio_init(PTE30,GPO,0); 
  gpio_init(PTE31,GPO,0); 
  //CCD管脚
  gpio_init(PTB1,GPO,0); 
  gpio_init(PTB2,GPO,0); 
  //OLED管脚
  gpio_init(PTA14,GPO,0); 
  gpio_init(PTA15,GPO,0);
  gpio_init(PTA16,GPO,0);
  gpio_init(PTA17,GPO,0);
}
//修正传感器偏差
void Fix_Offset()
{
  int Offset_X_Sum=0,Offset_Y_Sum=0;  //偏差累积
  int i; 
  LED_GREEN_ON; 
  LED_RED_ON;
  LED_BLUE_ON;
  DELAY_MS(1000); 
  for(i=0;i<20;i++)
  {
   Offset_X_Sum+= Get_X_Gyro();
   DELAY_MS(5); 
   Offset_Y_Sum+= Get_Y_Gyro();
   DELAY_MS(5);
  } 
  Gyro_X_Offset=(int)(Offset_X_Sum/20.0);
  Gyro_Y_Offset=(int)(Offset_Y_Sum/20.0);
}
