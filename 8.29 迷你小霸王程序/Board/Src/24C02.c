/*
* @file         24C02.c
  * @brief      24C02相关函数实现
  * @author     xiao_fang
  * @version    v1.0
  * @date       2017-3-12
*/
#include "common.h"
#include "24C02.h"
#include "MKL_i2c.h"
#include "MKL_SysTick.h"

/*
 *  @brief      初始化24c02
 *  @since      v1.0
*/
void flash_24c02_init(void)
{
  i2c_init(I2C0,FLASH_IIC_SPEED);
}

/*
 *  @brief      从24c02读取一个字节
 *  @since      v1.0
 *  address     要读取的地址(0~255)
*/
uint8 flash_24c02_read_byte(uint8 address)
{
  uint8 data = 0;
  systick_delay_ms(10);          //读写之间加延时
  data = i2c_read_reg(I2C0, IIC_ADDRESS, address);
  return data;
}

/*
 *  @brief      往24c02写入一个字节
 *  @since      v1.0
 *  data        要写入的数据
 *  address     要写入数据的地址(0~255)
*/
void flash_24c02_write_byte(uint8 data, uint8 address)
{
  systick_delay_ms(10);          //读写之间加延时
  i2c_write_reg(I2C0, IIC_ADDRESS, address, data);
}

/*
 *  @brief      从24c02中擦除一个字节（填入0x00）
 *  @since      v1.0
 *  address     要擦除的地址(0~255)
*/
void flash_24c02_erase_byte(uint8 address)
{
  systick_delay_ms(10);          //读写之间加延时
  i2c_write_reg(I2C0, IIC_ADDRESS, address, 0x00);
}
/*
 *  @brief      向24c02中连续写入多个字节
 *  @since      v1.0
 *  address     要写入的地址(注意地址溢出)
 *  data        要写入的数据
 *  lenth       要写入的数据的长度
*/
void flash_24c02_Write_lenth_byte(uint8 address, uint32 data, uint8 lenth)
{  	
	uint8 t=0;
	for(t=0;t<lenth;t++)
	{
            flash_24c02_write_byte((data>>(8*t))&0xff,address+t);
	}												    
}
/*
 *  @brief      从24c02中连续读取多个字节
 *  @since      v1.0
 *  address     要读取的地址(注意地址溢出)
 *  lenth       要读取的数据的长度
*/
uint32 flash_24c02_read_lenth_byte(uint8 address,uint8 lenth)
{  	
	uint8 t;
	uint32 data=0;
	for(t=0;t<lenth;t++)
	{
		data<<=8;
		data+=flash_24c02_read_byte(address+lenth-t-1); 	 				   
	}
	return data;												    
}

/*
 *  @brief      对24c02进行全片擦除（全部填入0x00）
 *  @since      v1.0
 *  全部擦除耗时较长
*/
void flash_24c02_erase_all(void)
{
  uint8 address = 0;
  for(address = 0;address < 255;address ++)
  {
    systick_delay_ms(10);          //读写之间加延时
    i2c_write_reg(I2C0, IIC_ADDRESS, address, 0x00);
  }
  systick_delay_ms(10);
  i2c_write_reg(I2C0, IIC_ADDRESS, 255, 0x00);  //255号扇区单独擦除
}
