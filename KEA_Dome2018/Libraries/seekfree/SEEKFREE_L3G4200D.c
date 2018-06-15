/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		L3G4200D陀螺仪
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.23
 * @Target core		S9KEA128AMLK
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-11-6
 * @note	
					L3G4200D接线定义
					------------------------------------ 
						SCL                 查看SEEKFREE_IIC文件内的SEEKFREE_SCL宏定义
						SDA                 查看SEEKFREE_IIC文件内的SEEKFREE_SDA宏定义
					------------------------------------ 
 ********************************************************************************************************************/



#include "SEEKFREE_L3G4200D.h"

int16 gyro_x = 0, gyro_y = 0, gyro_z = 0;

uint8 Temperature = 0;




//-------------------------------------------------------------------------------------------------------------------
//  @brief      初始化L3G4200D陀螺仪
//  @param      NULL
//  @return     void						
//  @since      v1.0
//  Sample usage:				调用该函数前，请先调用模拟IIC的初始化
//-------------------------------------------------------------------------------------------------------------------
void InitL3G4200D(void)
{
    simiic_write_reg(L3G4200_DEV_ADD, CTRL_REG1_4200, 0x4f);   //
    simiic_write_reg(L3G4200_DEV_ADD, CTRL_REG2_4200, 0x00);   //00
    simiic_write_reg(L3G4200_DEV_ADD, CTRL_REG3_4200, 0x08);   //
    simiic_write_reg(L3G4200_DEV_ADD, CTRL_REG4_4200, 0xB0);   //+-2000dps
    simiic_write_reg(L3G4200_DEV_ADD, CTRL_REG5_4200, 0x00);
    Temperature = simiic_read_reg(L3G4200_DEV_ADD, OUT_TEMP_4200, IIC);
}
uint32 time;
//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取L3G4200D陀螺仪数据
//  @param      NULL
//  @return     void						
//  @since      v1.0
//  Sample usage:				执行该函数后，直接查看对应的变量即可
//-------------------------------------------------------------------------------------------------------------------
void get_l3g4200d(void)
{
    uint8 dat[6];
    uint8 status;
    status = simiic_read_reg(L3G4200_DEV_ADD, STATUS_REG_4200, IIC);
    if(status>15)
    {
        status = status;
    }
    if( status&0x80 )//查看数据是否准备好了。如果没有准备好，读取则可能读取到错误数据。读取频率最好是高于数据输出频率
    {
        simiic_read_regs(L3G4200_DEV_ADD, OUT_X_L_4200|0X80, dat, 6, IIC);  //或上0x80是因为需要多字节读取数据
        gyro_x = (int16)((uint16)dat[1]<<8 | dat[0])>>3;                   
        gyro_y = (int16)((uint16)dat[3]<<8 | dat[2])>>3;
        gyro_z = (int16)((uint16)dat[5]<<8 | dat[4])>>3;
    }
}

