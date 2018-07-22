
#ifndef PID_h
#define PID_h


#define Motor_Max  3500
#define Motor_Min    0

typedef struct 
{
	float Kp;
	float Ki;
	float Kd;
	int32 e0;
	int32 e1;
	int32 e2;
	int32 SUM;
	int32 DefaultSum;

}MotorPidStruct;
MotorPidStruct MotorPID;



#endif