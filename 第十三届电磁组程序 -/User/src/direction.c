/*********************************************************************************************************************
 * @file  DirectionControl.c
 *  				方向控制
 * @core			S9KEA128
 * @date   2018
 ********************************************************************************************************************/
#include  "main.h"

/**********全局变量定义********/ 
float Direction_P = 610;	//方向控制P  提高P偏差响应变快 如果发现转弯及时但是过不了就应增大P
float Direction_D = 2200;	//方向控制D
float DirectionErr[2];	//方向偏差（g_fDirectionError[0]为一对水平电感的差比和偏差）
				//（g_fDirectionError[1]为一对垂直电感的差比和偏差）
float DirectionErr_dot[2];//方向偏差微分（g_fDirectionError_dot[0]为一对水平电感的差比和偏差微分）
				//（g_fDirectionError_dot[1]为一对垂直电感的差比和偏差微分）
float DirectionOut=0;	//方向控制输出
int16 sensor[5]={0};       //获取的电感值

int16 sensorFilter[5]={0};	//阶梯滤波的电感值（未使用）
uint8 Flag_Round1 = 0;		//进入环岛的标志（在环岛里为ON）
uint8 Flag_Round2 = 0;
uint8 Flag_Round3 = 0;  //目前判断是向左拐还是向右拐

/**
 * @file		方向控制
 *				一般情况下：用两水平电感的差比和作为偏差
 *				在环岛时中：用量垂直电感的差比和作为偏差
 *
 *									电感值对应变量
 *
 *				g_ValueOfAD[0]									g_ValueOfAD[1]
 *				(水平左电感)									（水平右电感）
 *						g_ValueOfAD[2]					g_ValueOfAD[3]
 *						（垂直左电感）					（垂直右电感）
 * @date		2018
 */
void DirectionControl(void)
{
  static float DirectionErrTemp[2][5]={0};  //临时储存变量
	Read_ADC();		//获取电感值	
	if(sensor[0]<10 && sensor[1]<10)  Stop_Flag=1;	//冲出赛道停车保护	
	sensor[0] = (sensor[0] < 10? 10:sensor[0]);	//电感值限幅
	sensor[1] = (sensor[1] < 10? 10:sensor[1]);
	sensor[2] = (sensor[2] < 10? 10:sensor[2]);
	sensor[3] = (sensor[3] < 10? 10:sensor[3]);
        sensor[4] = (sensor[4] < 10? 10:sensor[4]);
	//水平电感偏差计算		
	DirectionErr[0] = (float)(sensor[0]-sensor[1])/(sensor[0]+sensor[1]);//左-右水平电感的差比和作为偏差
	DirectionErr[0] = (DirectionErr[0]>= 1? 1:DirectionErr[0]);    //偏差限幅	
	DirectionErr[0] = (DirectionErr[0]<=-1?-1:DirectionErr[0]);
	//垂直电感偏差计算
	DirectionErr[1] = (float)(sensor[2]-sensor[3])/(sensor[2]+sensor[3]);//左-右垂直电感的差比和作为偏差
	DirectionErr[1] = (DirectionErr[1]>= 1? 1:DirectionErr[1]);	//偏差限幅
	DirectionErr[1] = (DirectionErr[1]<=-1?-1:DirectionErr[1]);	
	//水平电感偏差微分计算(历史误差循环队列)  
	DirectionErrTemp[0][4] = DirectionErrTemp[0][3];
	DirectionErrTemp[0][3] = DirectionErrTemp[0][2];
	DirectionErrTemp[0][2] = DirectionErrTemp[0][1];
	DirectionErrTemp[0][1] = DirectionErrTemp[0][0];
	DirectionErrTemp[0][0] = DirectionErr[0];
	DirectionErr_dot[0] = 5*(DirectionErrTemp[0][0]-DirectionErrTemp[0][3]);//水平电感的偏差微分
	DirectionErr_dot[0] = (DirectionErr_dot[0]> 0.7? 0.7:DirectionErr_dot[0]);//偏差微分限幅
	DirectionErr_dot[0] = (DirectionErr_dot[0]<-0.7?-0.7:DirectionErr_dot[0]);
        //垂直电感偏差微分计算(历史误差队列循环)  
	DirectionErrTemp[1][4] = DirectionErrTemp[1][3];
	DirectionErrTemp[1][3] = DirectionErrTemp[1][2];
	DirectionErrTemp[1][2] = DirectionErrTemp[1][1];
	DirectionErrTemp[1][1] = DirectionErrTemp[1][0];
	DirectionErrTemp[1][0] = DirectionErr[1];
	DirectionErr_dot[1] = 5*(DirectionErrTemp[1][0]-DirectionErrTemp[1][3]);//垂直电感的偏差微分
	DirectionErr_dot[1] = (DirectionErr_dot[1]> 0.7? 0.7:DirectionErr_dot[1]);//偏差微分限幅
	DirectionErr_dot[1] = (DirectionErr_dot[1]<-0.7?-0.7:DirectionErr_dot[1]);	
	//以下为环岛处理
	if((sensor[0]>900)&&(sensor[1]>900)&&(sensor[2]<200)&&(sensor[3]<200)&&(Flag_Round2<3))
	{
          if(Flag_Round1==0)
             {Flag_Round1=1;}
//          if(Flag_Round2==1)
//             {DirectionErr[1]=DirectionErr[1]*10;
//             DirectionErr_dot[1]=DirectionErr_dot[1]*10;}
	}	
	if(Flag_Round1>50) 
	{
		Flag_Round1=0;
                Flag_Round2=Flag_Round2+1;                
	}
	if(Flag_Round1==0){
          if(DirectionErr[0]<0.2&&DirectionErr[0]>-0.2){
            DirectionOut=DirectionErr[0]*(Direction_P-500)+DirectionErr_dot[0]*(Direction_D-2000);}  //这一部分由于水平电感位置不同要根据一定规律设置
          else{
            DirectionOut=DirectionErr[0]*Direction_P+DirectionErr_dot[0]*Direction_D;}
        }
	else
        {   DirectionOut=DirectionErr[1]*700+DirectionErr_dot[1]*1600;
            Flag_Round1=Flag_Round1+1;
        }  //总这一部分由于水平电感位置不同要根据一定规律设置	  
}
/**
 * @file  电感信号采集和中值滤波
 * @note      	
 * @date  2018
 **/
void Read_ADC(void) 
{
     int16 i,j,k,temp;
     int16 ad_value[5][5],ad_valueF[5],ad_sum[5];
//     int16 ad_value_old[5],ad_value_new[5];
     for(i=0;i<5;i++)
     {   ad_value[0][i]=AD_Avg(AD1, ADC_12bit, 5);  // AD1水平左
         ad_value[1][i]=AD_Avg(AD2, ADC_12bit, 5);  // AD2水平右
         ad_value[2][i]=AD_Avg(AD3, ADC_12bit, 5);  // AD3垂直左
         ad_value[3][i]=AD_Avg(AD4, ADC_12bit, 5);  // AD4垂直右
     }  //没有归一化处理
/*=========================冒泡排序升序==========================*///舍弃最大值和最小值
     for(i=0;i<5;i++)
     {
       for(j=0;j<4;j++)
       {
         for(k=0;k<4-j;k++)  //五个点感知只需要交换4次
         {
           if(ad_value[i][k]>ad_value[i][k+1]) //前面的比后面的大则进行交换
           {
             temp=ad_value[i][k+1];
             ad_value[i][k+1]=ad_value[i][k];
             ad_value[i][k]= temp;
           }
         }
       }
     }
/*===========================中值滤波=================================*/
     for(i=0;i<5;i++)    //求中间三项的和
     {
        ad_sum[i] =ad_value[i][1]+ad_value[i][2]+ad_value[i][3];  //舍去最大最小取中间三项
        ad_valueF[i]=ad_sum[i]/3;  //求平均值
     }	
	 for(i=0;i<5;i++)  {//将数值中个位数除掉,降低过高精度
	     sensor[i] = (int16)(ad_valueF[i]/10*10);  //
		//采集梯度平滑，每次采集最大变化40(没有用到)
//		ad_value_old[i]=sensorFilter[i];
//		ad_value_new[i]=sensor[i];		
//		if(ValueOfADNew[i]>=ValueOfADOld[i])
//			g_ValueOfADFilter[i] = ((ValueOfADNew[i]-ValueOfADOld[i])>50?(ValueOfADOld[i]+50):ValueOfADNew[i]);  //减小变化量
//		else
//			g_ValueOfADFilter[i] = ((ValueOfADNew[i]-ValueOfADOld[i])<-60?(ValueOfADOld[i]-60):ValueOfADNew[i]);  //减小变化量
	 }
}
	 


