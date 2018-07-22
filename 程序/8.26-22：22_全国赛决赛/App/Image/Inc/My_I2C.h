#ifndef __MY_I2C_H__
#define __MY_I2C_H__
#include "common.h"
#include "include.h"


#define My_SCL	PTD8	// 定义SCL引脚
#define My_SDA	PTD9	// 定义SDA引脚


#define ack 1      //主应答
#define no_ack 0   //从应答

    						  
//-----------------OLED IIC端口定义----------------  					   

 		     

typedef enum IIC
{
    IIC,
    SCCB
} IIC_type;

#define SDA        	gpio_get (My_SDA)
#define SDA0()     	gpio_set (My_SDA, 0)	// IO口输出低电平
#define SDA1()     	gpio_set (My_SDA, 1)	// IO口输出高电平  
#define SCL0()    	gpio_set (My_SCL, 0)	// IO口输出低电平
#define SCL1()		gpio_set (My_SCL, 1)	// IO口输出高电平
#define DIR_OUT()	gpio_ddr (My_SDA, GPO)	// 输出方向
#define DIR_IN()   	gpio_ddr (My_SDA, GPI)	// 输入方向

  

void  IIC_init(void);
void  IIC_start(void);
void  IIC_stop(void);
void  IIC_ack_main(uint8 ack_main);
void  send_ch(uint8 c);
uint8 read_ch(void);
void  simiic_write_reg(uint8 dev_add, uint8 reg, uint8 dat);
uint8 simiic_read_reg(uint8 dev_add, uint8 reg, IIC_type type);



#endif
