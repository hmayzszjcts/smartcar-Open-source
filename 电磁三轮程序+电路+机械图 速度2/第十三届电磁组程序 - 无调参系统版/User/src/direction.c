/*********************************************************************************************************************
 * 
 * @file       		direction.c
 *  				方向控制
 * @core			S9KEA128
 * @date       		2018
 ********************************************************************************************************************/
#include  "includefile.h"


/**********全局变量定义********/ 
float g_dirControl_P = 700;		//方向控制P
float g_dirControl_D = 2200;	//方向控制D
float g_fDirectionError[2];		//方向偏差（g_fDirectionError[0]为一对水平电感的差比和偏差）
								//		  （g_fDirectionError[1]为一对垂直电感的差比和偏差）
float g_fDirectionError_dot[2];//方向偏差微分（g_fDirectionError_dot[0]为一对水平电感的差比和偏差微分）
								//			 （g_fDirectionError_dot[1]为一对垂直电感的差比和偏差微分）
float g_fDirectionControlOut;	//方向控制输出
int16 g_ValueOfAD[4]={0};		//获取的电感值
int16 g_ValueOfADFilter[4]={0};	//阶梯滤波的电感值（未使用）
uint8 Flag_Round = OFF;			//进入环岛的标志（在环岛里为ON）



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
	static float g_fDirectionErrorTemp[2][5];
	
	Read_ADC();		//获取电感值
	
	if(g_ValueOfAD[0]<50 && g_ValueOfAD[1]<50)	Flag_Stop = 1;	//冲出赛道停车保护
	
	g_ValueOfAD[0] = (g_ValueOfAD[0] < 10? 10:g_ValueOfAD[0]);	//四个电感值限幅
	g_ValueOfAD[1] = (g_ValueOfAD[1] < 10? 10:g_ValueOfAD[1]);
	g_ValueOfAD[2] = (g_ValueOfAD[2] < 10? 10:g_ValueOfAD[2]);
	g_ValueOfAD[3] = (g_ValueOfAD[3] < 10? 10:g_ValueOfAD[3]);
			
	g_fDirectionError[0] = (float)(g_ValueOfAD[0] - g_ValueOfAD[1])/(g_ValueOfAD[0] + g_ValueOfAD[1]);//水平电感的差比和作为偏差
	g_fDirectionError[0] = (g_fDirectionError[0]>= 1? 1:g_fDirectionError[0]);	//偏差限幅
	g_fDirectionError[0] = (g_fDirectionError[0]<=-1?-1:g_fDirectionError[0]);
	
	g_fDirectionError[1] = (float)(g_ValueOfAD[2] - g_ValueOfAD[3])/(g_ValueOfAD[2] + g_ValueOfAD[3]);//垂直电感的差比和作为偏差
	g_fDirectionError[1] = (g_fDirectionError[1]>= 1? 1:g_fDirectionError[1]);	//偏差限幅
	g_fDirectionError[1] = (g_fDirectionError[1]<=-1?-1:g_fDirectionError[1]);	
	  
	g_fDirectionErrorTemp[0][4] = g_fDirectionErrorTemp[0][3];
	g_fDirectionErrorTemp[0][3] = g_fDirectionErrorTemp[0][2];
	g_fDirectionErrorTemp[0][2] = g_fDirectionErrorTemp[0][1];
	g_fDirectionErrorTemp[0][1] = g_fDirectionErrorTemp[0][0];
	g_fDirectionErrorTemp[0][0] = g_fDirectionError[0];
	g_fDirectionError_dot[0] = 5*(g_fDirectionErrorTemp[0][0]-g_fDirectionErrorTemp[0][3]);//水平电感的偏差微分
	g_fDirectionError_dot[0] = (g_fDirectionError_dot[0]> 0.7? 0.7:g_fDirectionError_dot[0]);//偏差微分限幅
	g_fDirectionError_dot[0] = (g_fDirectionError_dot[0]<-0.7?-0.7:g_fDirectionError_dot[0]);

	g_fDirectionErrorTemp[1][4] = g_fDirectionErrorTemp[1][3];
	g_fDirectionErrorTemp[1][3] = g_fDirectionErrorTemp[1][2];
	g_fDirectionErrorTemp[1][2] = g_fDirectionErrorTemp[1][1];
	g_fDirectionErrorTemp[1][1] = g_fDirectionErrorTemp[1][0];
	g_fDirectionErrorTemp[1][0] = g_fDirectionError[1];
	g_fDirectionError_dot[1] = 5*(g_fDirectionErrorTemp[1][0]-g_fDirectionErrorTemp[1][3]);//垂直电感的偏差微分
	g_fDirectionError_dot[1] = (g_fDirectionError_dot[1]> 0.7? 0.7:g_fDirectionError_dot[1]);//偏差微分限幅
	g_fDirectionError_dot[1] = (g_fDirectionError_dot[1]<-0.7?-0.7:g_fDirectionError_dot[1]);
	
	//以下为环岛处理
	if((g_ValueOfAD[0]>2300)&&(g_ValueOfAD[1]>2300)&&(g_ValueOfAD[2]<800)&&(g_ValueOfAD[3]<800))
	{
		Flag_Round = ON;
		gpio_set(BUZZ,1);
	}
	
	if((g_ValueOfAD[2]>200)&&(g_ValueOfAD[3]>200)&&(Flag_Round == ON))
	{
		Flag_Round = OFF;
		gpio_set(BUZZ,0);
	}
	//flash中参数
//	g_dirControl_P = float01;	//700
//	g_dirControl_D = float02;	//2200
	
	//方向算法（位置式PD）
	g_fDirectionControlOut = g_fDirectionError[0]*g_dirControl_P + g_fDirectionError_dot[0]*g_dirControl_D;
	if(Flag_Round == ON)
	g_fDirectionControlOut = g_fDirectionError[1]*700 + g_fDirectionError_dot[1]*1600;	
	  
}


/**
 * @file		电感信号采集和处理
 * @note      	
 * @date		2018
 */
void Read_ADC(void)
{
     int16  i,j,k,temp;
     int16  ad_valu[4][5],ad_valu1[4],ad_sum[4];
	 int16 ValueOfADOld[4],ValueOfADNew[4];

     for(i=0;i<5;i++)
     {
         ad_valu[0][i]=ad_ave(AD1, ADC_12bit, 5);  			// AD1水平左
         ad_valu[1][i]=ad_ave(AD2, ADC_12bit, 5);     		// AD2水平右
         ad_valu[2][i]=ad_ave(AD3, ADC_12bit, 5);  			// AD3垂直左
         ad_valu[3][i]=ad_ave(AD4, ADC_12bit, 5);     		// AD4垂直右		 
     }
/*=========================冒泡排序升序==========================*///舍弃最大值和最小值
     for(i=0;i<4;i++)
     {
        for(j=0;j<4;j++)
        {
           for(k=0;k<4-j;k++)
           {
              if(ad_valu[i][k] > ad_valu[i][k+1])        //前面的比后面的大  则进行交换
              {
                 temp = ad_valu[i][k+1];
                 ad_valu[i][k+1] = ad_valu[i][k];
                 ad_valu[i][k] = temp;
              }
           }
        }
     }
/*===========================中值滤波=================================*/
     for(i=0;i<4;i++)    //求中间三项的和
     {
        ad_sum[i] = ad_valu[i][1] + ad_valu[i][2] + ad_valu[i][3];
        ad_valu1[i] = ad_sum[i] / 3;
     }

	
	 for(i=0;i<4;i++)            //将数值中个位数除掉
	 {
	 	g_ValueOfAD[i] = (int16)(ad_valu1[i]/10*10);

		//采集梯度平滑，每次采集最大变化40
		ValueOfADOld[i] = g_ValueOfADFilter[i];
		ValueOfADNew[i] = g_ValueOfAD[i];
		
		if(ValueOfADNew[i]>=ValueOfADOld[i])
			g_ValueOfADFilter[i] = ((ValueOfADNew[i]-ValueOfADOld[i])>50?(ValueOfADOld[i]+50):ValueOfADNew[i]);
		else
			g_ValueOfADFilter[i] = ((ValueOfADNew[i]-ValueOfADOld[i])<-60?(ValueOfADOld[i]-60):ValueOfADNew[i]);
	 }
	 
}


