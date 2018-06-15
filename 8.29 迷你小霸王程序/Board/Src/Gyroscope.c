/*
* @file         Gyroscope.c
  * @brief      倾角传感器相关函数实现
  * @author     xiao_fang
  * @version    v1.0
  * @date       2017-7-31
*/
#include "common.h"
#include "Gyroscope.h"
#include "MKL_adc.h"
#include "IIC_sim.h"

/*
 *  @brief      初始化MPU6050
 *  @since      v1.0
*/
uint8 MPU_Init(void)
{ 
    uint8 res;
    IIC_Init();
    IICwriteByte(MPU_ADDR, MPU_PWR_MGMT1_REG, 0X80);    //复位MPU6050
    systick_delay_ms(100);
    IICwriteByte(MPU_ADDR, MPU_PWR_MGMT1_REG, 0X00);    //唤醒MPU6050
    MPU_Set_Gyro_Fsr(3);	                        //设置陀螺仪				
    MPU_Set_Accel_Fsr(0);				//设置加速度计	
    MPU_Set_Rate(50);					//设置采样率	
    IICwriteByte(MPU_ADDR,MPU_INT_EN_REG,0X00);	        //关闭INT中断
    IICwriteByte(MPU_ADDR,MPU_USER_CTRL_REG,0X00);	//关闭IIC主模式
    IICwriteByte(MPU_ADDR,MPU_FIFO_EN_REG,0X00);	//关闭FIFO
    IICwriteByte(MPU_ADDR,MPU_INTBP_CFG_REG,0X80);      //INT低电平有效
    res=I2C_ReadOneByte(MPU_ADDR ,MPU_DEVICE_ID_REG);   //读取器件地址
    if(res==0x68)
    {
        IICwriteByte(MPU_ADDR,MPU_PWR_MGMT1_REG,0X01);	//启动加速度计
	IICwriteByte(MPU_ADDR,MPU_PWR_MGMT2_REG,0X00);	//启动陀螺仪      
        return 0;
     }
    else 
    {
        return 1;
    }
	
}
/*
 *  @brief      设置陀螺仪满量程范围
 *  @since      v1.0
*　0:250DPS   1:500DPS   2:1000DPS   3:2000DPS
*/
uint8 MPU_Set_Gyro_Fsr(uint8 fsr)
{
	return IICwriteByte(MPU_ADDR,MPU_GYRO_CFG_REG,fsr<<3);//éè??íó?Yò??úá?3ì·??§  
}
 
/*
 *  @brief      设置加速度计满量程范围
 *  @since      v1.0
 *　0:2G   1:4G   2:8G   3:16G
*/
uint8 MPU_Set_Accel_Fsr(uint8 fsr)
{
	return IICwriteByte(MPU_ADDR,MPU_ACCEL_CFG_REG,fsr<<3);//éè???ó?ù?è′??D?÷?úá?3ì·??§  
}

/*
 *  @brief      设置数字低通滤波器
 *  @since      v1.0
 *　lpf:滤波频率
*/
uint8 MPU_Set_LPF(uint16 lpf)
{
	uint8 data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return IICwriteByte(MPU_ADDR,MPU_CFG_REG,data);  
}

/*
 *  @brief      设置采样率
 *  @since      v1.0
 *　rate:4~1000HZ
*/
uint8 MPU_Set_Rate(uint16 rate)
{
	uint8 data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=IICwriteByte(MPU_ADDR,MPU_SAMPLE_RATE_REG,data);
 	return MPU_Set_LPF(rate/2);	
}

/*
 *  @brief      得到温度数据
 *  @since      v1.0
 *　返回温度数据
*/
int16 MPU_Get_Temperature(void)
{
    uint8 buf[2]; 
    int16 raw;
	float temp;
        IICreadBytes(MPU_ADDR, MPU_TEMP_OUTH_REG, 2, buf);
    raw=((uint16)buf[0]<<8)|buf[1];  
    temp=36.53+((double)raw)/340;  
    return (int16)(temp);
}

/*
 *  @brief      得到陀螺仪数据
 *  @since      v1.0
 *　返回陀螺仪数据
*/
uint8 MPU_Get_Gyroscope(int16 *gx,int16 *gy,int16 *gz)
{
    uint8 buf[6],res; 
        res=IICreadBytes(MPU_ADDR, MPU_GYRO_XOUTH_REG, 6, buf);
	if(res==6)
	{
		*gx=((uint16)buf[0]<<8)|buf[1];  
		*gy=((uint16)buf[2]<<8)|buf[3];  
		*gz=((uint16)buf[4]<<8)|buf[5];
	} 	
    return res;;
}

/*
 *  @brief      得到加速度计数据
 *  @since      v1.0
 *　返回加速度计数据
*/
uint8 MPU_Get_Accelerometer(int16 *ax,int16 *ay,int16 *az)
{
    uint8 buf[6],res;  
        res=IICreadBytes(MPU_ADDR, MPU_ACCEL_XOUTH_REG, 6, buf);
	if(res==6)
	{
		*ax=((uint16)buf[0]<<8)|buf[1];  
		*ay=((uint16)buf[2]<<8)|buf[3];  
		*az=((uint16)buf[4]<<8)|buf[5];
	} 	
    return res;;
}