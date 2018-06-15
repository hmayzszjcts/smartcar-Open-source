/*********************************************************************************************************************
 * 
 * @file       		speed.c
 *  				速度控制
 * @core			S9KEA128
 * @date       		2018
 ********************************************************************************************************************/
#include  "includefile.h"


/**********全局变量定义********/
int32 LeftDead = 0;  //左电机死区
int32 RighDead = 0;  //右电机死区
float Ratio_Encoder_Left = 207/(1175*0.02);// 左轮速度=counter*左轮周长(mm)/(左轮转一圈对应的脉冲数*程序周期)
float Ratio_Encoder_Righ = 207/(1175*0.02);// 右轮速度=counter*右轮周长(mm)/(右轮转一圈对应的脉冲数*程序周期)
float g_fRealSpeed = 0;		//真实速度
float g_fLeftRealSpeed;
float g_fRighRealSpeed;
float g_fSpeedFilter = 0;		//阶梯滤波速度
float g_fExpectSpeed = 2800;		//期望速度
float g_fSpeedError;			//速度偏差
float g_fSpeedErrorTemp[5] = {0};
float fSpeedErrorInteg = 0;			
int32 g_nLeftpulse = 0,g_nRighpulse = 0;
float g_PWMOut;
int32 LeftPWM=0, RightPWM=0;  //
int32 LeftPWM_F= 0, RighPWM_F= 0;  //最终左右轮PWM输出
//速度控制
float g_speedControl_P=320;		//速度P
float g_speedControl_I=100;		//速度I
float g_fSpeedControlOut=0;		//速度输出						

/**
 * @file		PWM输出
 * @date		2018
 */
void PWMOut(void)
{	if(Flag_Speed == OFF)	g_fSpeedControlOut = 0;	//如果Flag_Speed == OFF	 不输出速度
	if(Flag_Direction == OFF)  DirectionOut = 0;	//如果Flag_Direction == OFF 不输出方向		
	LeftPWM = (int32)(300-DirectionOut);  //左电感PWM就是 
	RightPWM = (int32)(300+DirectionOut); 
        //电机差异补偿
//	g_nLeftPWM = g_nLeftPWM*1.05;
//	g_nRighPWM = g_nRighPWM;	
//	if(Flag_Stop == OFF) 									//如果Flag_Stop == OFF电机输出0
//	{
//	  	g_nLeftPWM=0;g_nRighPWM = 0;
//	}
  	if(LeftPWM < 0)
	{	LeftPWM_F = LeftDead - LeftPWM;
		LeftPWM_F = (LeftPWM_F > 950? 950: LeftPWM_F);
		ftm_pwm_duty(ftm2, ftm_ch3, LeftPWM_F);
		ftm_pwm_duty(ftm2, ftm_ch2, 0);
	}
	else
	{	LeftPWM_F = LeftDead + LeftPWM;
		LeftPWM_F = (LeftPWM_F > 950? 950: LeftPWM_F);
		ftm_pwm_duty(ftm2, ftm_ch3, 0);
		ftm_pwm_duty(ftm2, ftm_ch2, LeftPWM_F);
	}
	if(RightPWM < 0)
	{	RighPWM_F = RighDead - RightPWM;
		RighPWM_F = (RighPWM_F > 950? 950: RighPWM_F);
		ftm_pwm_duty(ftm2, ftm_ch1, RighPWM_F);
		ftm_pwm_duty(ftm2, ftm_ch0, 0);
	}
	else
	{	RighPWM_F = RighDead + RightPWM;
		RighPWM_F = (RighPWM_F > 950? 950: RighPWM_F);
		ftm_pwm_duty(ftm2, ftm_ch1, 0);
		ftm_pwm_duty(ftm2, ftm_ch0, RighPWM_F);
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
        //循环误差保存	
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
void SpeedControl(void){
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
	
	if(Stop_Flag==OFF|Flag_Speed==OFF)
	{
		fSpeedErrorInteg = 0;	//停车积分清零
	}	
	//fSpeedErrorInteg = (fSpeedErrorInteg < 0.0? 0.0: fSpeedErrorInteg);//积分下限
	//fSpeedErrorInteg = (fSpeedErrorInteg > 400.0? 400.0: fSpeedErrorInteg);//积分上限
	//速度控制算法（增量式PI）
	g_fSpeedControlOut += g_speedControl_P*0.005*(g_fSpeedErrorTemp[0]-g_fSpeedErrorTemp[1]) + fSpeedErrorInteg;
	g_fSpeedControlOut = (g_fSpeedControlOut>=6000?6000:g_fSpeedControlOut);
	
}







