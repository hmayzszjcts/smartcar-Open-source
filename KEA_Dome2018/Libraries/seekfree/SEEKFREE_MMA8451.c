/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		MMA8451加速度计
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



#include "SEEKFREE_MMA8451.h"


//SA0必须接地

int16 acc_x = 0, acc_y = 0, acc_z = 0;


//-------------------------------------------------------------------------------------------------------------------
//  @brief      初始化MMA8451加速度计
//  @param      NULL
//  @return     uint8			返回1成功，返回0失败						
//  @since      v1.0
//  Sample usage:				调用该函数前，请先调用模拟IIC的初始化
//-------------------------------------------------------------------------------------------------------------------
uint8 MMA845x_init(void)
{       
    uint16 v;	
    simiic_write_reg(MMA8451_DEV_ADD, CTRL_REG1,ASLP_RATE_20MS+DATA_RATE_2500US);	
          
    simiic_write_reg(MMA8451_DEV_ADD, XYZ_DATA_CFG_REG, FULL_SCALE_2G); //2G
         
    simiic_write_reg(MMA8451_DEV_ADD, CTRL_REG1, (ACTIVE_MASK+ASLP_RATE_20MS+DATA_RATE_5MS)&(~FREAD_MASK)); //激活状态   14bit
         

    v= simiic_read_reg(MMA8451_DEV_ADD, WHO_AM_I_REG, IIC);
    if((v == MMA8451Q_ID)||(v == MMA8452Q_ID)||(v == MMA8453Q_ID)) return 1;
	
	return 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取MMA8451加速度计数据
//  @param      NULL
//  @return     void						
//  @since      v1.0
//  Sample usage:				执行该函数后，直接查看对应的变量即可
//-------------------------------------------------------------------------------------------------------------------
void get_mma8451(void)
{	    
    uint8 dat[6];
    uint16 wx,wy,wz;
    
    simiic_read_regs(MMA8451_DEV_ADD, OUT_X_MSB_REG, dat, 6, IIC);  
    
    wx = (uint16)dat[0]<<8 | dat[1];
    //补码求出加速度原始对应数值
    if(dat[0]>0x7f) acc_x=-(int16)((~(wx>>2) + 1)&0X3FFF);  //移位取反加一再去掉无效字符 
    else            acc_x=(wx>>2)&0X3FFF; 	                //移位掉无效字符 	

    
    wy = (uint16)dat[2]<<8 | dat[3];
    //补码求出加速度原始对应数值
    if(dat[2]>0x7f) acc_y=-(int16)((~(wy>>2) + 1)&0X3FFF);  //移位取反加一再去掉无效字符 
    else            acc_y=(wy>>2)&0X3FFF; 	                //移位掉无效字符 	

    
    wz = (uint16)dat[4]<<8 | dat[5];
    //补码求出加速度原始对应数值
    if(dat[4]>0x7f) acc_z=-(int16)((~(wz>>2) + 1)&0X3FFF);  //移位取反加一再去掉无效字符 
    else            acc_z=(wz>>2)&0X3FFF; 	                //移位掉无效字符 

}





















