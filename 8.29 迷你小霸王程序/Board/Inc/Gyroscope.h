/*
* @file         Gyroscope.h
  * @brief      倾角传感器相关函数实现
  * @author     xiao_fang
  * @version    v1.0
  * @date       2017-7-31
*/

#ifndef __GYROSCOPE_H__
#define __GYROSCOPE_H__

#define MPU_SELF_TESTX_REG		0X0D	//自检寄存器x
#define MPU_SELF_TESTY_REG		0X0E	//自检寄存器y
#define MPU_SELF_TESTZ_REG		0X0F	//自检寄存器z
#define MPU_SELF_TESTA_REG		0X10	//自检寄存器a
#define MPU_SAMPLE_RATE_REG		0X19	//采样平率分频器
#define MPU_CFG_REG			0X1A	//配置寄存器
#define MPU_GYRO_CFG_REG		0X1B	//陀螺仪配置寄存器
#define MPU_ACCEL_CFG_REG		0X1C	//加速度计配置寄存器
#define MPU_MOTION_DET_REG		0X1F	//运动检测阀值设置寄存器
#define MPU_FIFO_EN_REG			0X23	//FIFO使能寄存器
#define MPU_I2CMST_CTRL_REG		0X24	//IIC主机设置寄存器

#define MPU_I2CMST_STA_REG		0X36	//IIC主机状态寄存器
#define MPU_INTBP_CFG_REG		0X37	//中断设置寄存器
#define MPU_INT_EN_REG			0X38	//中断使能寄存器
#define MPU_INT_STA_REG			0X3A	//中断状态寄存器

#define MPU_ACCEL_XOUTH_REG		0X3B	//X轴高位加速度寄存器
#define MPU_ACCEL_XOUTL_REG		0X3C	//X轴低位加速度寄存器
#define MPU_ACCEL_YOUTH_REG		0X3D	//Y轴高位加速度寄存器
#define MPU_ACCEL_YOUTL_REG		0X3E	//Y轴低位加速度寄存器
#define MPU_ACCEL_ZOUTH_REG		0X3F	//Z轴高位加速度寄存器
#define MPU_ACCEL_ZOUTL_REG		0X40	//Z轴低位加速度寄存器

#define MPU_TEMP_OUTH_REG		0X41	//温度高位寄存器
#define MPU_TEMP_OUTL_REG		0X42	//温度低位寄存器

#define MPU_GYRO_XOUTH_REG		0X43	//X轴高位陀螺仪寄存器
#define MPU_GYRO_XOUTL_REG		0X44	//X轴低位陀螺仪寄存器
#define MPU_GYRO_YOUTH_REG		0X45	//Y轴高位陀螺仪寄存器
#define MPU_GYRO_YOUTL_REG		0X46	//Y轴低位陀螺仪寄存器
#define MPU_GYRO_ZOUTH_REG		0X47	//Z轴高位陀螺仪寄存器
#define MPU_GYRO_ZOUTL_REG		0X48	//Z轴低位陀螺仪寄存器

#define MPU_I2CMST_DELAY_REG	        0X67	//IIC主机延时管理寄存器
#define MPU_SIGPATH_RST_REG		0X68	//信号通道复位寄存器
#define MPU_MDETECT_CTRL_REG	        0X69	//运动检测控制寄存器
#define MPU_USER_CTRL_REG		0X6A	//用户控制寄存器
#define MPU_PWR_MGMT1_REG		0X6B	//电源管理寄存器1
#define MPU_PWR_MGMT2_REG		0X6C	//电源管理寄存器2 
#define MPU_FIFO_CNTH_REG		0X72	//FIFO计数寄存器高位
#define MPU_FIFO_CNTL_REG		0X73	//FIFO计数寄存器低位
#define MPU_FIFO_RW_REG			0X74	//FIFO读写寄存器
#define MPU_DEVICE_ID_REG		0X75	//器件ID寄存器
 
#define MPU_ADDR			0XD0    //器件地址


uint8 MPU_Init(void);
uint8 MPU_Set_Gyro_Fsr(uint8 fsr);
uint8 MPU_Set_Accel_Fsr(uint8 fsr);
uint8 MPU_Set_LPF(uint16 lpf);
uint8 MPU_Set_Rate(uint16 rate);
int16 MPU_Get_Temperature(void);
uint8 MPU_Get_Gyroscope(int16 *gx,int16 *gy,int16 *gz);
uint8 MPU_Get_Accelerometer(int16 *ax,int16 *ay,int16 *az);


#endif