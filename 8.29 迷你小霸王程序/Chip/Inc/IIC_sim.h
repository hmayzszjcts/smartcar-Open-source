/*
* @file         IIC_sim.h
  * @brief      iic相关函数实现
  * @author     xiao_fang
  * @version    v1.0
  * @date       2017-3-12
*/
#ifndef __IIC_SIM_H__
#define __IIC_SIM_H__

#define IIC_SCL         PTB9
#define IIC_SDA         PTB7

#define SCL_H()         gpio_set (PTB9, 1)
#define SCL_L()         gpio_set (PTB9, 0)
#define SCL_DDR_OUT()   gpio_ddr (PTB9, 1)
#define SCL_DDR_IN()    gpio_ddr (PTB9, 0)

#define SDA_H()         gpio_set (PTB7, 1)
#define SDA_L()         gpio_set (PTB7, 0)
#define SDA_IN()        gpio_get (PTB7)
#define SDA_DDR_OUT()   gpio_ddr (PTB7, 1)
#define SDA_DDR_IN()    gpio_ddr (PTB7, 0)

#define IIC_DELAY()     IIC_delay(200)


static void IIC_delay(volatile uint16_t time);
void IIC_Init(void);
void IIC_Start(void);
void IIC_Stop(void);
uint8 IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);
void IIC_Send_Byte(uint8 txd);
uint8 IIC_Read_Byte(unsigned char ack);
unsigned char I2C_ReadOneByte(unsigned char I2C_Addr,unsigned char addr);
uint8 IICreadBytes(uint8 dev, uint8 reg, uint8 length, uint8 *data);
uint8 IICwriteBytes(uint8 dev, uint8 reg, uint8 length, uint8* data);
uint8 IICreadByte(uint8 dev, uint8 reg, uint8 *data);
unsigned char IICwriteByte(unsigned char dev, unsigned char reg, unsigned char data);

#endif