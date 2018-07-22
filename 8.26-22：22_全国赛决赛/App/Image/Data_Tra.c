//用来传送数据到匿名上位机
#include "Data_tra.h"

#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))

uint8_t ARMED = 0;
uint8_t data_to_send[120];

float Value[9];

void Data_Send_Status(void)
{
	uint8_t i;
	uint8_t _cnt=0;
	int16_t _temp;
	uint8_t sum = 0;
	int32_t _temp2 = 0;						//传递大气压
	data_to_send[_cnt++]=0x88;
	data_to_send[_cnt++]=0xAF;
	data_to_send[_cnt++]=0x1C;
	
	Prepare_Anonymous_Data();	// 准备数据
	
	//传递加速度值
	_temp = (int)(Value[0]);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(Value[1]);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(Value[2]);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	//传递陀螺仪值
	_temp = (int)(Value[3]);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(Value[4]);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(Value[5]);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	//传递电磁计值
	_temp = (int)(0);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(0);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(0);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	_temp = (int)(Value[6]*100);//传递横滚值
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(Value[7]*100);//传递俯仰值
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	//_temp = (int)(Q_ANGLE.YAW*100);
	_temp = (int)(Value[8]*10);//传递转向角
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	data_to_send[_cnt++]=BYTE3(_temp2);
	data_to_send[_cnt++]=BYTE2(_temp2);
	data_to_send[_cnt++]=BYTE1(_temp2);
	data_to_send[_cnt++]=BYTE0(_temp2);
		
	
	for(i=0;i<_cnt - 4;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++]=sum;
	
	uart_putbuff(UART4, data_to_send, _cnt);	// 使用串口发送
}

void Prepare_Anonymous_Data(void)
{
//	Value[0] = ACC_Real.X;
//	Value[1] = ACC_Real.Y;
//	Value[2] = ACC_Real.Z;
//	Value[3] = GYRO_Real.X;
//	Value[4] = GYRO_Real.Y;
//	Value[5] = GYRO_Real.Z;
//	Value[6] = Attitude_Angle.X;
//	Value[7] = Attitude_Angle.Y;
//	Value[8] = Attitude_Angle.Z;
}
