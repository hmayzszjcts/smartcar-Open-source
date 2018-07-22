#include "MPU6050.h"

//宏定义调用底层的I2C接口
#define MPU6050_OPEN()      	IIC_init()
#define MPU6050_WR(reg,value)   simiic_write_reg(MPU6050_ADRESS, reg, value)	//mpu6050 写寄存器
#define MPU6050_RD(reg)         simiic_read_reg(MPU6050_ADRESS, reg, IIC)		//mpu6050 读寄存器


S_FLOAT_XYZ 
	GYRO_Real,		// 陀螺仪转化后的数据
	ACC_Real,		// 加速度计转化后的数据
	Attitude_Angle,	// 当前角度
	Last_Angle,		// 上次角度
	Target_Angle,	// 目标角度
	Now_Ang_Vel;	// 当前角速度
	

S_INT16_XYZ
	GYRO,			// 陀螺仪原始数据
	GYRO_Offset,	// 陀螺仪温飘
	GYRO_Last,		// 陀螺仪上次数据
	ACC, 			// 加速度计数据
	ACC_Offset,		// 加速度计温飘
	ACC_Last;		// 加速度计上次数据


char Offset_OK = 0;
 
/*
 * 函数名：MPU6050_Init
 * 描述  ：陀螺仪初始化
 * 输入  ：无
 * 输出  ：0成功  1失败
 * 调用  ：外部调用
 */
uchar MPU6050_Init(void)
{ 
	uchar res;
	
	MPU6050_OPEN();           				// 初始化 mpu6050 接口
	DELAY_MS(100);
	MPU6050_WR(MPU_PWR_MGMT1_REG,0X80);		// 复位MPU6050
  	DELAY_MS(10);
	MPU6050_WR(MPU_PWR_MGMT1_REG,0X00);		// 唤醒MPU6050 
	DELAY_MS(10);
	MPU6050_WR(MPU_PWR_MGMT1_REG,0X01);		// 设置CLKSEL,PLL X轴为参考
	DELAY_MS(10);
	MPU6050_WR(MPU_GYRO_CFG_REG,3<<3);		// 陀螺仪传感器,±500dps	// 0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
	DELAY_MS(10);
	MPU6050_WR(MPU_ACCEL_CFG_REG,0<<3);		// 加速度传感器,±2g		// 0,±2g;1,±4g;2,±8g;3,±16g
	DELAY_MS(10);
	MPU6050_WR(MPU_SAMPLE_RATE_REG,0X07);	// 设置MPU6050的采样率,8KHz
	DELAY_MS(10);
	MPU6050_WR(MPU_CFG_REG,0X00);			// 设置数字低通滤波器,256Hz
	DELAY_MS(100);
	
	res=MPU6050_RD(MPU_DEVICE_ID_REG);
	if(res==MPU6050_ADRESS)// 器件ID正确
	{
             //   MPU6050_WR(MPU_PWR_MGMT2_REG,0x0B);   //加速度X,Y,陀螺仪X
                DELAY_MS(10);
		MPU6050_Offset();
		
		return 0;
 	}
	else 
	{
		return 1;
	}
}

/*
 * 函数名：MPU6050_Offset
 * 描述  ：传感器采集零偏
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用
 */
void MPU6050_Offset(void)
{
	uint8 i, Count = 100;
	int64 temp[6] = {0};
	
//	GYRO_Offset.X = 0;
	GYRO_Offset.Y = 0;
//	GYRO_Offset.Z = 0;
	
	for (i = 0; i < Count; i++)
	{
		MPU6050_GetData(&GYRO, &ACC);	// 读取陀螺仪数据
		DELAY_MS(2);
		
		temp[0] += ACC.X;
///		temp[1] += ACC.Y;
//		temp[2] += ACC.Z;
//		
//		temp[3] += GYRO.X;
		temp[4] += GYRO.Y;
//		temp[5] += GYRO.Z;
	}
	ACC_Offset.X = temp[0] / Count;
//	ACC_Offset.Y = temp[1] / Count;
//	ACC_Offset.Z = temp[2] / Count;
//	
//	GYRO_Offset.X = temp[3] / Count;
	GYRO_Offset.Y = temp[4] / Count;
//	GYRO_Offset.Z = temp[5] / Count;
	
	Offset_OK = 1;
}

/*
 * 函数名：MPU6050_GetData
 * 描述  ：获得传感器所有数据
 * 输入  ：*GYRO 陀螺仪		*ACC 加速度计
 * 输出  ：无
 * 调用  ：外部调用
 */
void MPU6050_GetData(S_INT16_XYZ *GYRO, S_INT16_XYZ *ACC)
{
	if (Offset_OK)
	{
		ACC->X = GetData(MPU_ACCEL_XOUTH_REG);	// 获取加速度计原始数据
//		ACC->Y = GetData(MPU_ACCEL_YOUTH_REG);
//		ACC->Z = GetData(MPU_ACCEL_ZOUTH_REG);
//		
//		GYRO->X = GetData(MPU_GYRO_XOUTH_REG) - GYRO_Offset.X;	// 获取陀螺仪原始数据
		GYRO->Y = GetData(MPU_GYRO_YOUTH_REG) - GYRO_Offset.Y;
//		GYRO->Z = GetData(MPU_GYRO_ZOUTH_REG) - GYRO_Offset.Z;
	}
	else
	{
		ACC->X = GetData(MPU_ACCEL_XOUTH_REG);	// 获取加速度计原始数据并归一化
//		ACC->Y = GetData(MPU_ACCEL_YOUTH_REG);
//		ACC->Z = GetData(MPU_ACCEL_ZOUTH_REG);
//		
//		GYRO->X = GetData(MPU_GYRO_XOUTH_REG);	// 获取陀螺仪原始数据并归一化
		GYRO->Y = GetData(MPU_GYRO_YOUTH_REG);
//		GYRO->Z = GetData(MPU_GYRO_ZOUTH_REG);
	}
}

/*
 * 函数名：GetData
 * 描述  ：获得16位数据
 * 输入  ：REG_Address 寄存器地址
 * 输出  ：返回寄存器数据
 * 调用  ：外部调用
 */
int16 GetData(uchar REG_Address)
{
	uchar H, L;

	H = MPU6050_RD(REG_Address);
	L = MPU6050_RD(REG_Address+1);
	
	return ((H<<8)|L);   //合成数据
}


#define AcceRatio 	16384.0f
#define GyroRatio 	16.4f
#define Gyro_Gr		0.0010653	// 角速度变成弧度	此参数对应陀螺2000度每秒
#define ACC_FILTER_NUM 5		// 加速度计滤波深度
#define GYRO_FILTER_NUM 7		// 陀螺仪滤波深度
int32 ACC_X_BUF[ACC_FILTER_NUM], ACC_Y_BUF[ACC_FILTER_NUM], ACC_Z_BUF[ACC_FILTER_NUM];	// 滤波缓存数组
int32 GYRO_X_BUF[GYRO_FILTER_NUM], GYRO_Y_BUF[GYRO_FILTER_NUM], GYRO_Z_BUF[GYRO_FILTER_NUM];
/*
 * 函数名：Data_Filter
 * 描述  ：数据滑动滤波
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void Data_Filter(void)	// 数据滤波
{
	uchar i;
	int64 temp1 = 0, temp2 = 0, temp3 = 0, temp4 = 0, temp5 = 0, temp6 = 0;
	
	ACC_X_BUF[0] = ACC.X;	// 更新滑动窗口数组
//	ACC_Y_BUF[0] = ACC.Y;
//	ACC_Z_BUF[0] = ACC.Z;
//	GYRO_X_BUF[0] = GYRO.X;
	GYRO_Y_BUF[0] = GYRO.Y;
//	GYRO_Z_BUF[0] = GYRO.Z;
	
	for(i=0;i<ACC_FILTER_NUM;i++)
	{
		temp1 += ACC_X_BUF[i];
//		temp2 += ACC_Y_BUF[i];
//		temp3 += ACC_Z_BUF[i];
//		
	}
	for(i=0;i<GYRO_FILTER_NUM;i++)
	{
//		temp4 += GYRO_X_BUF[i];
		temp5 += GYRO_Y_BUF[i];
//		temp6 += GYRO_Z_BUF[i];
	}
	
	ACC_Real.X = temp1 / ACC_FILTER_NUM / AcceRatio * 9.8;
//	ACC_Real.Y = temp2 / ACC_FILTER_NUM / AcceRatio * 9.8;
//	ACC_Real.Z = temp3 / ACC_FILTER_NUM / AcceRatio * 9.8;
//	GYRO_Real.X = temp4 / GYRO_FILTER_NUM / GyroRatio;
	GYRO_Real.Y = temp5 / GYRO_FILTER_NUM / GyroRatio;
//	GYRO_Real.Z = temp6 / GYRO_FILTER_NUM / GyroRatio;
	
	for(i = 0; i < ACC_FILTER_NUM - 1; i++)
	{
		ACC_X_BUF[ACC_FILTER_NUM-1-i] = ACC_X_BUF[ACC_FILTER_NUM-2-i];
//		ACC_Y_BUF[ACC_FILTER_NUM-1-i] = ACC_Y_BUF[ACC_FILTER_NUM-2-i];
//		ACC_Z_BUF[ACC_FILTER_NUM-1-i] = ACC_Z_BUF[ACC_FILTER_NUM-2-i];
//		
	}
	for(i = 0; i < GYRO_FILTER_NUM - 1; i++)
	{
//		GYRO_X_BUF[GYRO_FILTER_NUM-1-i] = GYRO_X_BUF[GYRO_FILTER_NUM-2-i];
		GYRO_Y_BUF[GYRO_FILTER_NUM-1-i] = GYRO_Y_BUF[GYRO_FILTER_NUM-2-i];
//		GYRO_Z_BUF[GYRO_FILTER_NUM-1-i] = GYRO_Z_BUF[GYRO_FILTER_NUM-2-i];
	}
}

/*
 * 函数名：Get_Attitude
 * 描述  ：姿态解算
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void Get_Attitude(void)	// 姿态解算
{
	IMUupdate(GYRO_Real.X*Gyro_Gr*GyroRatio, 
			  GYRO_Real.Y*Gyro_Gr*GyroRatio, 
			  GYRO_Real.Z*Gyro_Gr*GyroRatio, 
			  ACC_Real.X * AcceRatio / 9.8, 
			  ACC_Real.Y * AcceRatio / 9.8, 
			  ACC_Real.Z * AcceRatio / 9.8);	// 姿态解算出欧拉角
}


//===============================四元素============================================
#define Kp 1.6f //10.0f             	// proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 0.001f//1.2f // //0.008f  	// integral gain governs rate of convergence of gyroscope biases
#define halfT 0.005f                   	// half the sample period采样周期的一半
float q0 = 1, q1 = 0, q2 = 0, q3 = 0; 	// quaternion elements representing the estimated orientation
float exInt = 0, eyInt = 0, ezInt = 0; 	// scaled integral error
/*
 * 函数名：IMUupdate
 * 描述  ：四元素解算欧拉角
 * 输入  ：陀螺仪 加速度计
 * 输出  ：无
 * 调用  ：内部调用
 */
void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az)
{
	float norm;
	float vx, vy, vz;
	float ex, ey, ez;

	// 先把这些用得到的值算好
	float q0q0 = q0*q0;
	float q0q1 = q0*q1;
	float q0q2 = q0*q2;
	float q1q1 = q1*q1;
	float q1q3 = q1*q3;
	float q2q2 = q2*q2;
	float q2q3 = q2*q3;
	float q3q3 = q3*q3;

	if (ax*ay*az == 0)
	{
		return;
	}
		
	norm = sqrt(ax*ax + ay*ay + az*az);	// acc数据归一化
	ax = ax / norm;
	ay = ay / norm;
	az = az / norm;

	// estimated direction of gravity and flux (v and w)	估计重力方向和流量/变迁
	vx = 2*(q1q3 - q0q2);									// 四元素中xyz的表示
	vy = 2*(q0q1 + q2q3);
	vz = q0q0 - q1q1 - q2q2 + q3q3 ;

	// error is sum of cross product between reference direction of fields and direction measured by sensors
	ex = (ay*vz - az*vy) ;		// 向量外积在相减得到差分就是误差
	ey = (az*vx - ax*vz) ;
	ez = (ax*vy - ay*vx) ;

	exInt = exInt + ex * Ki;	// 对误差进行积分
	eyInt = eyInt + ey * Ki;
	ezInt = ezInt + ez * Ki;

	// adjusted gyroscope measurements
	gx = gx + Kp*ex + exInt;	// 将误差PI后补偿到陀螺仪，即补偿零点漂移
	gy = gy + Kp*ey + eyInt;
	gz = gz + Kp*ez + ezInt;	// 这里的gz由于没有观测者进行矫正会产生漂移，表现出来的就是积分自增或自减

	// integrate quaternion rate and normalise	// 四元素的微分方程
	q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
	q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
	q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
	q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;

	// normalise quaternion
	norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
	q0 = q0 / norm;
	q1 = q1 / norm;
	q2 = q2 / norm;
	q3 = q3 / norm;
	
	Attitude_Angle.Y = asin(-2*q1*q3 + 2*q0*q2) * 57.3; // pitch
//	Attitude_Angle.X = atan2(2*q2*q3 + 2*q0*q1, -2*q1*q1 - 2*q2*q2 + 1)*57.3; // roll
//	Attitude_Angle.Z = atan2(2*q1*q2 + 2*q0*q3, -2*q2*q2 - 2*q3*q3 + 1)*57.3; // yaw
	Attitude_Angle.Z = 0;
}

