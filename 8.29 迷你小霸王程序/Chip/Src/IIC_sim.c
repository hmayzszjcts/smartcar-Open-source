/*
* @file         IIC_sim.c
  * @brief      模拟iic相关函数实现
  * @author     xiao_fang
  * @version    v1.0
  * @date       2017-3-12
*/
#include "common.h"
#include "IIC_sim.h"
#include "MKL_gpio.h"

static void IIC_delay(volatile uint16_t time)
{
    while(time)
    {
        time--;
    }
}
  
/**************************实现函数********************************************
*函数原型:		void IIC_Init(void)
*功　　能:		初始化I2C对应的接口引脚。
*******************************************************************************/
void IIC_Init(void)
{			    
    gpio_init  (PTB9, GPO, 1); //初始化SCL0
    gpio_init  (PTB7, GPO, 1); //初始化SDA0
    port_init(PTB9 , ALT1 | PULLUP);
    port_init(PTB7 , ALT1 | PULLUP);
}

/**************************实现函数********************************************
*函数原型:		void IIC_Start(void)
*功　　能:		产生IIC起始信号
*******************************************************************************/
void IIC_Start(void)
{
	SDA_DDR_OUT();
	SDA_H();	  	  
	SCL_H();
	IIC_DELAY();
 	SDA_L();//START:when CLK is high,DATA change form high to low 
	IIC_DELAY();
	SCL_L();
}

/**************************实现函数********************************************
*函数原型:		void IIC_Stop(void)
*功　　能:	    //产生IIC停止信号
*******************************************************************************/	  
void IIC_Stop(void)
{
	SDA_DDR_OUT();
	SCL_L();
	SDA_L();//STOP:when CLK is high DATA change form low to high
 	IIC_DELAY();
	SCL_H(); 
	SDA_H();
	IIC_DELAY();							   	
}

/**************************实现函数********************************************
*函数原型:		u8 IIC_Wait_Ack(void)
*功　　能:	    等待应答信号到来 
//返回值：1，接收应答失败
//        0，接收应答成功
*******************************************************************************/
uint8 IIC_Wait_Ack(void)
{
	uint8 ucErrTime=0;
	SDA_DDR_OUT(); 
	SDA_H(); IIC_DELAY();	   
	SCL_H(); IIC_DELAY();
        SDA_DDR_IN();
	while(SDA_IN())
	{
		ucErrTime++;
		if(ucErrTime>50)
		{
			IIC_Stop();
			return 1;
		}
	  IIC_DELAY();
	}
	SCL_L();//ê±?óê?3?0 	   
	return 0;  
} 

/**************************实现函数********************************************
*函数原型:		void IIC_Ack(void)
*功　　能:	    产生ACK应答
*******************************************************************************/
void IIC_Ack(void)
{
	SCL_L();
	SDA_DDR_OUT();
	SDA_L();
	IIC_DELAY();
	SCL_H();
	IIC_DELAY();
	SCL_L();
}
	
/**************************实现函数********************************************
*函数原型:		void IIC_NAck(void)
*功　　能:	    产生NACK应答
*******************************************************************************/	    
void IIC_NAck(void)
{
	SCL_L();
	SDA_DDR_OUT();
	SDA_H();
	IIC_DELAY();
	SCL_H();
	IIC_DELAY();
	SCL_L();
}					 				     

/**************************实现函数********************************************
*函数原型:		void IIC_Send_Byte(u8 txd)
*功　　能:	    IIC发送一个字节
*******************************************************************************/		  
void IIC_Send_Byte(uint8 txd)
{                        
    uint8 t=8;   
    SDA_DDR_OUT(); 	    
    SCL_L();
    while(t--)
    {
        if(txd&0x80)
        {
            SDA_H();
        }
        else
        {
            SDA_L();
        }
        txd<<=1; 
        IIC_DELAY();   
	SCL_H();
	IIC_DELAY(); 
	SCL_L();	
	//IIC_delay();
    }
} 	 
   
/**************************实现函数********************************************
*函数原型:		u8 IIC_Read_Byte(unsigned char ack)
*功　　能:	    //读1个字节，ack=1时，发送ACK，ack=0，发送nACK 
*******************************************************************************/ 
uint8 IIC_Read_Byte(unsigned char ack)
{
    unsigned char i,receive=0;
    SDA_DDR_IN();
    for(i=0;i<8;i++ )
    {
        SCL_L(); 
        IIC_DELAY();
	SCL_H();
        receive<<=1;
        if(SDA_IN())receive++;   
	IIC_DELAY(); 
    }					 
    if (ack)
        IIC_Ack(); 
    else
        IIC_NAck();
    return receive;
}

/**************************实现函数********************************************
*函数原型:		unsigned char I2C_ReadOneByte(unsigned char I2C_Addr,unsigned char addr)
*功　　能:	    读取指定设备 指定寄存器的一个值
输入	I2C_Addr  目标设备地址
		addr	   寄存器地址
返回   读出来的值
*******************************************************************************/ 
unsigned char I2C_ReadOneByte(unsigned char I2C_Addr,unsigned char addr)
{
	unsigned char res=0;
	
	IIC_Start();	
	IIC_Send_Byte(I2C_Addr);	   

	IIC_Wait_Ack();
	IIC_Send_Byte(addr);
 
	IIC_Wait_Ack();	  
	IIC_Start();
	IIC_Send_Byte(I2C_Addr+1); 	   
	IIC_Wait_Ack();
	res=IIC_Read_Byte(0);	   
        IIC_Stop();

	return res;
}


/**************************实现函数********************************************
*函数原型:		u8 IICreadBytes(u8 dev, u8 reg, u8 length, u8 *data)
*功　　能:	    读取指定设备 指定寄存器的 length个值
输入	dev  目标设备地址
		reg	  寄存器地址
		length 要读的字节数
		*data  读出的数据将要存放的指针
返回   读出来的字节数量
*******************************************************************************/
uint8 IICreadBytes(uint8 dev, uint8 reg, uint8 length, uint8 *data)
{
    uint8 count = 0;
	
	IIC_Start();
	IIC_Send_Byte(dev);	   
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);   
    IIC_Wait_Ack();	  
	IIC_Start();
	IIC_Send_Byte(dev+1);  
	IIC_Wait_Ack();
	
    for(count=0;count<length;count++){
		 
		 if(count!=length-1)data[count]=IIC_Read_Byte(1);  
		 	else  data[count]=IIC_Read_Byte(0);	 
	}
    IIC_Stop();
    return count;
}

/**************************实现函数********************************************
*函数原型:		u8 IICwriteBytes(u8 dev, u8 reg, u8 length, u8* data)
*功　　能:	    将多个字节写入指定设备 指定寄存器
输入	dev  目标设备地址
		reg	  寄存器地址
		length 要写的字节数
		*data  将要写的数据的首地址
返回   返回是否成功
*******************************************************************************/ 
uint8 IICwriteBytes(uint8 dev, uint8 reg, uint8 length, uint8* data)
{
  
 	uint8 count = 0;
	IIC_Start();
	IIC_Send_Byte(dev);
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);   
        IIC_Wait_Ack();	  
	for(count=0;count<length;count++){
		IIC_Send_Byte(data[count]); 
		IIC_Wait_Ack(); 
	 }
	IIC_Stop();

    return 1; //status == 0;
}

/**************************实现函数********************************************
*函数原型:		u8 IICreadByte(u8 dev, u8 reg, u8 *data)
*功　　能:	    读取指定设备 指定寄存器的一个值
输入	dev  目标设备地址
		reg	   寄存器地址
		*data  读出的数据将要存放的地址
返回   1
*******************************************************************************/
uint8 IICreadByte(uint8 dev, uint8 reg, uint8 *data)
{
	*data=I2C_ReadOneByte(dev, reg);
    return 1;
}

/**************************实现函数********************************************
*函数原型:		unsigned char IICwriteByte(unsigned char dev, unsigned char reg, unsigned char data)
*功　　能:	    写入指定设备 指定寄存器一个字节
输入	dev  目标设备地址
		reg	   寄存器地址
		data  将要写入的字节
返回   1
*******************************************************************************/
unsigned char IICwriteByte(unsigned char dev, unsigned char reg, unsigned char data)
{
    return IICwriteBytes(dev, reg, 1, &data);
}