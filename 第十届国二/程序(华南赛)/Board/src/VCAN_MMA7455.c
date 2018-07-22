/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       VCAN_MMA7455.c
 * @brief      MMA7455驱动函数实现
 * @author     山外科技
 * @version    v5.0
 * @date       2013-07-9
 */

#include "common.h"
#include "MK60_port.h"
#include "MK60_i2c.h"
#include "VCAN_MMA7455.h"

//宏定义调用底层的I2C接口
#define MMA7455_OPEN(baud)      i2c_init(MMA7455_DEVICE,baud)
#define MMA7455_WR(reg,value)   i2c_write_reg(MMA7455_DEVICE,MMA7455_ADRESS,reg,value)  //mma7455 写寄存器
#define MMA7455_RD(reg)         i2c_read_reg(MMA7455_DEVICE,MMA7455_ADRESS,reg)         //mma7455 读寄存器

/*!
 *  @brief      MMA7455初始化，进入 2g 量程测试模式
 *  @since      v5.0
 *  Sample usage:            mma7455_init();    //初始化 MMA7455
 */
void mma7455_init(void)
{
    MMA7455_OPEN(400 * 1000);           //初始化mma7455接口，设置波特率 400k

    /*MMA进入 2g 量程测试模式*/
    MMA7455_WR(MMA7455_MCTL, 0x05);

    /*DRDY标置位,等待测试完毕*/
    while(!(MMA7455_RD(MMA7455_STATUS) & 0x01));
}

/*!
 *  @brief      MMA7455写寄存器
 *  @param      reg         寄存器
 *  @param      dat         需要写入的数据的寄存器地址
 *  @since      v5.0
 *  Sample usage:       mma7455_write_reg(MMA7455_XOFFL,0);   // 写寄存器 MMA7455_XOFFL 为 0
 */
void mma7455_write_reg(uint8 reg, uint8 Data)
{
    MMA7455_WR(reg, Data);
}

/*!
 *  @brief      MMA7455读寄存器
 *  @param      reg         寄存器
 *  @param      dat         需要读取数据的寄存器地址
 *  @since      v5.0
 *  Sample usage:       uint8 data = mma7455_read_reg(MMA7455_XOFFL);    // 读寄存器 MMA7455_XOFFL
 */
uint8 mma7455_read_reg(uint8 reg)
{
    return MMA7455_RD(reg);
}


