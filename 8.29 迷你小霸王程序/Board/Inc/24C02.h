/*
* @file         24C02.h
  * @brief      24C02相关函数实现
  * @author     xiao_fang
  * @version    v1.0
  * @date       2017-3-12
*/
#ifndef __24C02_H__
#define __24C02_H__

#define FLASH_IIC_SPEED 100*1000        //IIC传输速率
#define IIC_ADDRESS 0X50        //24c02硬件7bit地址

void flash_24c02_init(void);            //初始化24c02
uint8 flash_24c02_read_byte(uint8 address);             //从24c02读取一个字节
void flash_24c02_write_byte(uint8 data, uint8 address);         //往24c02写入一个字节
void flash_24c02_erase_byte(uint8 address);              //从24c02中擦除一个字节（填入0x00）
void flash_24c02_erase_all(void);                //对24c02进行全片擦除（全部填入0x00）
void flash_24c02_Write_lenth_byte(uint8 address, uint32 data, uint8 lenth);     //向24c02中连续写入多个字节
uint32 flash_24c02_read_lenth_byte(uint8 address,uint8 lenth);          //从24c02中连续读取多个字节
#endif