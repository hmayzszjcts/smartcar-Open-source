/*********************************************************************************************************************
 * 
 * @file       		ir.c
 *  				红外解码
 ********************************************************************************************************************/
#include "common.h"
#include "KEA128_port.h"


uint8 ircode[4];	//解码后的数值
uint8 user_code;	//用户码
uint8 irkey_value=0;//对应键值
//红外线初始化
void ir_init(void)
{
  irq_init();
  port_pull(IRQ_PIN);

}

uint16 get_high_time(void)
{
  uint16 hightime;
  pit_time_start(PIT1);
  while(gpio_get(IRQ_PIN)==1);
  hightime = pit_time_get_us(PIT1);

  return hightime;    
}


uint16 get_low_time(void)
{
  uint16 lowtime;
  pit_time_start(PIT1);
  while(gpio_get(IRQ_PIN)==0);
  lowtime = pit_time_get_us(PIT1);

  return lowtime;    
}

//红外解码
void ir_decode(void)
{
  uint8 i=0,j=0;
  uint16 time=0;
  uint8 byt=0;
  
  time = get_low_time();
  if((time<3700) || (time>4200)){   
   return;
  }
  
  time = get_high_time();
  if((time<3700) || (time>4200)){
    return;
  }
  
  for(i=0;i<=2;i++){
    for(j=0;j<=7;j++){
      time = get_low_time();            
      time = get_high_time();
      if((time>900)&&(time<1200)){
        byt >>= 1;
      }
      else{
        byt >>= 1; 
        byt |= 0x80; 
      }     
    }
    ircode[i] = byt; 
  }
  
  user_code = ircode[1] + ircode[2];
  
  DELAY_MS(10);
  
#if 0		//若想在oled上看键值，则改为1
oled_fill(0);
oled_print_16x8short(0,0,user_code);
#else
  code_mapping();
#endif
  
}

//对应键值
void code_mapping(void)
{
  if(user_code==176){           //电源键
	Stop_Flag =! Stop_Flag;
  }
  else if(user_code==81){                        
	Flag_Debuge = ON;		//进入界面调试模式
  }
  else if(user_code==110){
    irkey_value = BT_UP_PRESS;  //上
  }  
  else if(user_code==210){
    irkey_value = BT_OK_PRESS; //ok
  }
  else if(user_code==155){
    irkey_value = BT_LE_PRESS;  //左
  }
  else if(user_code==35){
    irkey_value = BT_RI_PRESS;  //右
  }
  else if(user_code==230){
    irkey_value = BT_DN_PRESS;  //下
  }
  else if(user_code==198){     //静音键
      
  }
}

//获得红外按键
uint8 GetIRKey(void)
{
  return irkey_value; 
}

//设置红外按键
void SetIRKey(unsigned char key)
{
    irkey_value = key;
}
