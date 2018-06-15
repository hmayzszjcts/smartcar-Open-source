/*********************************************************************************************************************
 * 
 * @file       		speed.c
 *  				速度控制
 * @core			S9KEA128
 * @date       		2018
 ********************************************************************************************************************/
#include  "includefile.h"


/**********全局变量定义********/
int32 LeftDead = 0;
int32 RighDead = 0;
float Ratio_Encoder_Left = 207/(1175*0.02);// 左轮速度=counter*左轮周长(mm)/(左轮转一圈对应的脉冲数*程序周期)
float Ratio_Encoder_Righ = 207/(1175*0.02);// 右轮速度=counter*右轮周长(mm)/(右轮转一圈对应的脉冲数*程序周期)
float g_fRealSpeed = 0;				//真实速度
float g_fLeftRealSpeed;
float g_fRighRealSpeed;
float g_fSpeedFilter = 0;			//阶梯滤波速度
float g_fExpectSpeed = 2800;		//期望速度
float g_fSpeedError;				//速度偏差
float g_fSpeedErrorTemp[5] = {0};
float fSpeedErrorInteg = 0;			
int32 g_nLeftpulse = 0,g_nRighpulse = 0;
float g_PWMOut;
int32 g_nLeftPWM, g_nRighPWM;
//速度控制
float g_speedControl_P = 320;		//速度P
float g_speedControl_I = 100;		//速度I
float g_fSpeedControlOut = 0;		//速度输出
						

/**
 * @file		PWM输出
 * @date		2018
 */
void PWMOut(void)
{
	int32 nLeftPWM = 0, nRighPWM = 0;
	
	if(Flag_Speed == OFF)		g_fSpeedControlOut = 0;		//如果Flag_Speed == OFF		不输出速度
	if(Flag_Direction == OFF)	g_fDirectionControlOut = 0;	//如果Flag_Direction == OFF 不输出方向
		
	g_nLeftPWM = (int32)(g_fSpeedControlOut - g_fDirectionControlOut);
	g_nRighPWM = (int32)(g_fSpeedControlOut + g_fDirectionControlOut); 

//	g_nLeftPWM = g_nLeftPWM*1.05;//电机差异补偿
//	g_nRighPWM = g_nRighPWM;
	
	if(Flag_Stop == OFF) 									//如果Flag_Stop == OFF电机输出0
	{
	  	g_nLeftPWM = 0;g_nRighPWM = 0;
	}

  	if(g_nLeftPWM < 0)
	{
		nLeftPWM = LeftDead - g_nLeftPWM;
		nLeftPWM = (nLeftPWM > 900? 900: nLeftPWM);
		ftm_pwm_duty(ftm2, ftm_ch2, nLeftPWM);
		ftm_pwm_duty(ftm2, ftm_ch3, 0);
	}
	else
	{
		nLeftPWM = LeftDead + g_nLeftPWM;
		nLeftPWM = (nLeftPWM > 900? 900: nLeftPWM);
		ftm_pwm_duty(ftm2, ftm_ch2, 0);
		ftm_pwm_duty(ftm2, ftm_ch3, nLeftPWM);
	}

	if(g_nRighPWM < 0)
	{
		nRighPWM = RighDead - g_nRighPWM;
		nRighPWM = (nRighPWM > 900? 900: nRighPWM);
		ftm_pwm_duty(ftm2, ftm_ch4, 0);
		ftm_pwm_duty(ftm2, ftm_ch5, nRighPWM);
	}
	else
	{
		nRighPWM = RighDead + g_nRighPWM;
		nRighPWM = (nRighPWM > 900? 900: nRighPWM);
		ftm_pwm_duty(ftm2, ftm_ch4, nRighPWM);
		ftm_pwm_duty(ftm2, ftm_ch5, 0);
	}	
}
   

/**
 * @file		计算速度偏差
 * @note      	产生全局变量g_fSpeedError
 * @date		2017
 */
void CalSpeedError(void)
{
	static float fSpeedOld = 0, fSpeedNew = 0;
	
	g_nLeftpulse = (gpio_get(Coder_dir_left) ==1?ftm_count_get(ftm0):-ftm_count_get(ftm0));//读取左轮脉冲
	ftm_count_clean(ftm0);
	g_nRighpulse = (gpio_get(Coder_dir_right)==0?ftm_count_get(ftm1):-ftm_count_get(ftm1));//读取右轮脉冲
	ftm_count_clean(ftm1);	
	
	g_fLeftRealSpeed = g_nLeftpulse*Ratio_Encoder_Left;
	g_fLeftRealSpeed = (g_fLeftRealSpeed>3400?3400:g_fLeftRealSpeed);		//滤左编码器的噪声
	g_fRighRealSpeed = g_nRighpulse*Ratio_Encoder_Righ;
	g_fRighRealSpeed = (g_fRighRealSpeed>3400?3400:g_fRighRealSpeed);		//滤右编码器的噪声
	
	g_fRealSpeed = (g_fLeftRealSpeed + g_fRighRealSpeed)*0.5;				//真实速度
	
	//速度采集梯度平滑，每次采集最大变化200
	fSpeedOld = g_fSpeedFilter;
	fSpeedNew = g_fRealSpeed;
	
	if(fSpeedNew>=fSpeedOld)
		g_fSpeedFilter = ((fSpeedNew-fSpeedOld)>300?(fSpeedOld+300):fSpeedNew);
	else
		g_fSpeedFilter = ((fSpeedNew-fSpeedOld)<-300?(fSpeedOld-300):fSpeedNew);
	
	//flash中参数
//	g_fExpectSpeed = float05;  //2800
	
	g_fSpeedError =  g_fExpectSpeed - g_fSpeedFilter;
	g_fSpeedErrorTemp[4] = g_fSpeedErrorTemp[3];
	g_fSpeedErrorTemp[3] = g_fSpeedErrorTemp[2];
	g_fSpeedErrorTemp[2] = g_fSpeedErrorTemp[1];
	g_fSpeedErrorTemp[1] = g_fSpeedErrorTemp[0];
	g_fSpeedErrorTemp[0] = g_fSpeedError;
	
}


/**
 * @file		速度控制
 * @note      	速度梯度平滑
 * @date		2017
 */
void SpeedControl(void)
{
	int8 index=1;

	CalSpeedError();	//计算速度偏差
	g_fSpeedError = (g_fSpeedError>800?800:g_fSpeedError);//速度偏差限幅
	//flash中参数
//	g_speedControl_P = float06;	//320
//	g_speedControl_I = float07;	//100
	
	//积分分离
	if((g_fSpeedError<=300)&&(g_fSpeedError>=-300))
	index=1;
	else
	index=0;	
	fSpeedErrorInteg = index *float08 * g_fSpeedError * 0.00001;
	
	if(Flag_Stop==OFF|Flag_Speed==OFF)
	{
		fSpeedErrorInteg = 0;	//停车积分清零
	}	
	//fSpeedErrorInteg = (fSpeedErrorInteg < 0.0? 0.0: fSpeedErrorInteg);//积分下限
	//fSpeedErrorInteg = (fSpeedErrorInteg > 400.0? 400.0: fSpeedErrorInteg);//积分上限
	//速度控制算法（增量式PI）
	g_fSpeedControlOut += g_speedControl_P*0.005*(g_fSpeedErrorTemp[0]-g_fSpeedErrorTemp[1]) + fSpeedErrorInteg;
	g_fSpeedControlOut = (g_fSpeedControlOut>=6000?6000:g_fSpeedControlOut);
	
}







