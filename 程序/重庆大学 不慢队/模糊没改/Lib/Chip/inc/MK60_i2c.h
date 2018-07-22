/*!
 * @file       MK60_i2c.h
 * @brief      i2c驱动头文件
 */

#ifndef     __MK60_I2C_H__
#define     __MK60_I2C_H__
/**
 *  @brief I2C模块编号
 */
typedef enum I2Cn
{
    I2C0  = 0,
    I2C1  = 1
} I2Cn_e;

/**
 *  @brief 主机读写模式选择
 */
typedef enum MSmode
{
    MWSR =   0x00,  /* 主机写模式  */
    MRSW =   0x01   /* 主机读模式  */
} MSmode;


uint32  i2c_init(I2Cn_e i2cn, uint32 baud);                               //初始化I2C
void    i2c_write_reg(I2Cn_e, uint8 SlaveID, uint8 reg, uint8 Data);      //往地址里的写内容
uint8   i2c_read_reg (I2Cn_e, uint8 SlaveID, uint8 reg);                  //从地址里读取内容

#endif  //__MK60_I2C_H__
