/* @file        fuzzy.c
  * @brief      模糊控制函数
  * @author     xiao_fang
  * @version    v1.0
  * @date       2017-3-12
*/
#include "common.h"
#include "Fuzzy.h"
#define RANK 7

float fuzzy_kp = 0.0;   //模糊算得的kp值
float fuzzy_kd = 0.0;   //模糊算得的kd值
float fuzzy_speed = 0.0;
/*
 *  @brief      舵机模糊控制
 *  @since      v1.0
 *  P
 *  D 
*/
void KP_Fuzzy(float position , float position_error)
{
    int fe = 0,fec = 0,fec_speed = 0;//输入到模糊里的偏差
    int pe = 0,pec = 0,pec_speed = 0;//输入到模糊里的偏差变化量
    int num=0;
    float eRule[7]={0,23,49,80,111,148,195};                             //偏差的分段点                        
    float ecRule[7]={0,0.9,1.8,3.6,5.4,7.2,9};                 //偏差变化量的分段点
    float ec_speedRule[7]={-50,-30,-15,0,15,30,50};			//偏差变化量
    
	
					
	//偏差变化隶属度
    /*速度模糊调节*/
              /*原值*/
//    float Rule_kp[7]={0.7,0.75,0.8,0.85,0.9,0.95,1};          //kp隶属函数中心值
//    float Rule_kd[7]={6,6.5,7,7.5,8,8.5,9};                 //kp隶属函数中心值
//	float Rule_speed[7]={2.5,2.6,2.7,2.8,2.85,2.9,2.95};
          /*调节后 速度等级15*/
//    float Rule_kp[7]={0.58,0.60,0.62,0.64,0.66,0.68,0.7};          //kp隶属函数中心值
//    float Rule_kd[7]={5,5.5,6.0,6.5,7.0,7.5,8.0};                 //kp隶属函数中心值
//	  float Rule_speed[7]={2.4,2.53,2.55,2.57,2.59,2.61,2.63};
             /*速度等级16*/
    float Rule_kp[7]={0.62,0.64,0.66,0.68,0.70,0.72,0.72};          //kp隶属函数中心值
    float Rule_kd[7]={6,6.5,7,7.5,8,9,10};                 //kp隶属函数中心值
    float Rule_speed[7]={2.5,2.65,2.68,2.7,2.72,2.75,2.78};
            /*速度等级17*/
//      float Rule_kp[7]={0.63,0.65,0.67,0.72,0.75,0.77,0.81};          //kp隶属函数中心值
//      float Rule_kd[7]={11,13,14,15,16,17,19};                 //kp隶属函数中心值
//      float Rule_speed[7]={2.58,2.80,2.82,2.85,2.87,2.89,2.91};
            /*速度等级18*/
//    float Rule_kp[7]={0.64,0.66,0.68,0.73,0.76,0.79,0.83};          //kp隶属函数中心值
//    float Rule_kd[7]={16,17.5,20,21,22,23,24};                 //kp隶属函数中心值
//	  float Rule_speed[7]={2.7,2.95,2.98,3,3.03,3.08,3.1};
            /*速度等级19*/
//    float Rule_kp[7]={0.64,0.66,0.68,0.73,0.76,0.79,0.83};          //kp隶属函数中心值
//    float Rule_kd[7]={20,21,22,23,24,25,26};                 //kp隶属函数中心值
//	  float Rule_speed[7]={2.7,2.95,3.03,3.08,3.1,3.13,3.2};
            /*速度等级20*/
//    float Rule_kp[7]={0.62,0.66,0.68,0.73,0.76,0.79,0.86};          //kp隶属函数中心值
//    float Rule_kd[7]={12,16,18,20,22,25,28};                 //kp隶属函数中心值
//      float Rule_speed[7]={2.7,2.9,3.25,3.28,3.33,3.36,3.4};
    
    
    float eFuzzy[2]={0.0,0.0};                              //偏差隶属度
    float ecFuzzy[2]={0.0, 0.0};                                //偏差变化隶属度
    float ec_speedFuzzy[2]={0.0, 0.0};
    float U1Fuzzy_kp[7]={0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    float U1Fuzzy_kd[7]={0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    float U1Fuzzy_speed[7]={0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    int rule_kp[7][7]=
    {
  //ec 0 1 2 3 4 5 6  //e
      {2,2,1,1,1,0,0},//0    3,2,2,1,1,1,0
      {3,3,2,1,1,1,0},//1    2,2,1,1,1,0,0 
      {4,3,3,2,2,1,1},//2    2,2,1,1,1,0,0 
      {4,4,3,3,2,1,1},//3    4,3,2,1,1,1,0 
      {4,4,4,3,2,2,1},//4
      {5,5,5,4,4,3,2},//5
      {6,6,5,5,4,3,3} //6
    };

    int rule_kd[7][7]=
    {
  //ec 0 1 2 3 4 5 6  //e
      {3,3,4,5,5,6,6},//0
      {3,3,4,4,5,5,5},//1
      {2,3,3,4,5,5,5},//2
      {1,2,2,3,3,4,5},//3
      {1,1,2,2,3,3,3},//4
      {0,1,1,1,2,2,3},//5
      {0,0,1,1,1,2,2} //6
    };
    
    int rule_speed[7][7]=
{
  //ec-3-2-1 0 1 2 3  //e  
      {5,5,6,6,4,3,2},//0 
      {4,4,6,6,3,2,1},//1 
      {4,4,5,5,3,2,1},//2 
      {3,4,5,5,2,1,1},//3 
      {3,3,4,4,2,1,0},//4 
      {3,2,3,4,1,1,0},//5 
      {3,2,2,4,1,0,0} //6 
};
    /*-------------------------------------------*/
    /*----------------确定输入量-----------------*/
    /*-------------------------------------------*/
    
    fe = (int)(ABS(position));          //偏差和偏差的变化量均取绝对值做计算
    fec = (int)(ABS(position_error));
    fec_speed = 0.0;
    fuzzy_kp = 0.0;
    fuzzy_kd = 0.0;
    fuzzy_speed = 0.0;
    /*-------------------------------------------*/
    /*---------------确定kp隶属度----------------*/
    /*-------------------------------------------*/
    if(fe < eRule[0])     		       
    {
        eFuzzy[0] =1.0; 
        pe= 0;         
    }
    else if(fe < eRule[1])	        
    {       
        eFuzzy[0] = (eRule[1]-fe)/(eRule[1]-eRule[0]);
        pe = 0;
    }
    else if(fe < eRule[2])	       
    {
        eFuzzy[0] = (eRule[2] -fe)/(eRule[2]-eRule[1]);
        pe =1;
    }
    else if(fe < eRule[3])	       
    {
        eFuzzy[0] = (eRule[3] -fe)/(eRule[3]-eRule[2]);
        pe =2;
    }
    else if(fe < eRule[4])		       
    {   
        eFuzzy[0] = (eRule[4]-fe)/(eRule[4]-eRule[3]);
        pe=3;
    }
    else if(fe < eRule[5])		       
    {
        eFuzzy[0] = (eRule[5]-fe)/(eRule[5]-eRule[4]);
        pe=4;
    }
    else if(fe < eRule[6])		        
    {
        eFuzzy[0] = (eRule[6]-fe)/(eRule[6]-eRule[5]);
        pe=5;
    }		
    else						        
    {
        eFuzzy[0] =1.0;
        pe=6;
    }
    eFuzzy[1] = 1.0 - eFuzzy[0];                    //eFuzzy[0]+eFuzzy[1]=1;

    
    /*-------------------------------------------*/
    /*---------------确定kd隶属度----------------*/
    /*-------------------------------------------*/
    if(fec <= ecRule[0])
    {
        ecFuzzy[0] =1.0;
        pec = 0;
    }
    else if(fec < ecRule[1])
    {
        ecFuzzy[0] = (ecRule[1] - fec)/(ecRule[1]-ecRule[0]);
        pec = 0 ;
    }
    else if(fec < ecRule[2])
    {
        ecFuzzy[0] = (ecRule[2] - fec)/(ecRule[2]-ecRule[1]);
        pec = 1;
    }
    else if(fec < ecRule[3])
    {
        ecFuzzy[0] = (ecRule[3] - fec)/(ecRule[3]-ecRule[2]);
        pec = 2 ;
    }
    else if(fec < ecRule[4])
    { 
        ecFuzzy[0] = (ecRule[4] - fec)/(ecRule[4]-ecRule[3]);
        pec=3;
    }
    else if(fec < ecRule[5])		
    {
        ecFuzzy[0] = (ecRule[5]-fec)/(ecRule[5]-ecRule[4]);
        pec=4;
    }
    else if(fec<ecRule[6])		
    {
        ecFuzzy[0] = (ecRule[6]-fec)/(ecRule[6]-ecRule[5]);
        pec=5;
    }		
    else										
    {
        ecFuzzy[0] =1.0;
        pec=6;
    }
    ecFuzzy[1] = 1.0 - ecFuzzy[0];
    /*-------------------------------------------*/
    /*---------------确定speed隶属 --------------*/
    /*-------------------------------------------*/
	if(fec_speed <= ec_speedRule[0])
	{
	    ec_speedFuzzy[0] =1.0;
	    pec_speed = 0;
	}
	else if(fec_speed < ec_speedRule[1])
	{
	    ec_speedFuzzy[0] = (ec_speedRule[1] - fec_speed)/(ec_speedRule[1]-ec_speedRule[0]);
	    pec_speed = 0 ;
	}
	else if(fec_speed < ec_speedRule[2])
	{
	    ec_speedFuzzy[0] = (ec_speedRule[2] - fec_speed)/(ec_speedRule[2]-ec_speedRule[1]);
	    pec_speed = 1;
    }
	else if(fec_speed < ec_speedRule[3])
	{
	    ec_speedFuzzy[0] = (ec_speedRule[3] - fec_speed)/(ec_speedRule[3]-ec_speedRule[2]);
	    pec_speed = 2 ;
	}
	else if(fec_speed < ec_speedRule[4])
	{ 
	    ec_speedFuzzy[0] = (ec_speedRule[4] - fec_speed)/(ec_speedRule[4]-ec_speedRule[3]);
	    pec_speed=3;
	}
	else if(fec_speed < ec_speedRule[5])		
	{
	    ec_speedFuzzy[0] = (ec_speedRule[5]-fec_speed)/(ec_speedRule[5]-ec_speedRule[4]);
	    pec_speed=4;
	}
	else if(fec_speed<ec_speedRule[6])		
	{
	    ec_speedFuzzy[0] = (ec_speedRule[6]-fec_speed)/(ec_speedRule[6]-ec_speedRule[5]);
	    pec_speed=5;
	}		
	else										
	{
	    ec_speedFuzzy[0] =1.0;
	    pec_speed=6;
	}
	ec_speedFuzzy[1] = 1.0 - ec_speedFuzzy[0];
    
    /*-------------------------------------------*/
    /*---------------查询kp规则表----------------*/
    /*-------------------------------------------*/  
    if(pe<(RANK-1) && pec<(RANK-1))        // e和e'都没有达到边缘
    {
        num = rule_kp[pec][pe];
        U1Fuzzy_kp[num] += eFuzzy[0]*ecFuzzy[0];

        num = rule_kp[pec][pe+1];
        U1Fuzzy_kp[num] += eFuzzy[1]*ecFuzzy[0];	
    
        num = rule_kp[pec+1][pe];
        U1Fuzzy_kp[num] += eFuzzy[0]*ecFuzzy[1];
    
        num = rule_kp[pec+1][pe+1];
        U1Fuzzy_kp[num] += eFuzzy[1]*ecFuzzy[1];
    }
    else if(pe==(RANK-1) && pec<(RANK-1))  // e达到边缘
    {
        num = rule_kp[pec][pe];
        U1Fuzzy_kp[num] += eFuzzy[0]*ecFuzzy[0];                
        num = rule_kp[pec+1][pe];
        U1Fuzzy_kp[num] += eFuzzy[0]*ecFuzzy[1];
    }
    else if(pe<(RANK-1) && pec==(RANK-1))  // e'达到边缘
    {
        num = rule_kp[pec][pe];
        U1Fuzzy_kp[num] += eFuzzy[0]*ecFuzzy[0];	
        num = rule_kp[pec][pe+1];
        U1Fuzzy_kp[num] += eFuzzy[1]*ecFuzzy[0];
    }
    else				       // e和e'同时达到边缘
    {     
        num = rule_kp[pec][pe];
        U1Fuzzy_kp[num] += eFuzzy[0]*ecFuzzy[0];		
    }
    
    
    /*-------------------------------------------*/
    /*---------------查询kd规则表----------------*/
    /*-------------------------------------------*/
    if(pe<(RANK-1) && pec<(RANK-1))        // e和e'都没有达到边缘
    {
        num = rule_kd[pec][pe];
        U1Fuzzy_kd[num] += eFuzzy[0]*ecFuzzy[0];
    
        num = rule_kd[pec][pe+1];
        U1Fuzzy_kd[num] += eFuzzy[1]*ecFuzzy[0];	
    
        num = rule_kd[pec+1][pe];
        U1Fuzzy_kd[num] += eFuzzy[0]*ecFuzzy[1];
    
        num = rule_kd[pec+1][pe+1];
        U1Fuzzy_kd[num] += eFuzzy[1]*ecFuzzy[1];
    }
    else if(pe==(RANK-1) && pec<(RANK-1))  // e达到边缘9
    {
        num = rule_kd[pec][pe];
        U1Fuzzy_kd[num] += eFuzzy[0]*ecFuzzy[0];                
        num = rule_kd[pec+1][pe];
        U1Fuzzy_kd[num] += eFuzzy[0]*ecFuzzy[1];
    }
    else if(pe<(RANK-1) && pec==(RANK-1))  // e'达到边缘
    {
        num = rule_kd[pec][pe];
        U1Fuzzy_kd[num] += eFuzzy[0]*ecFuzzy[0];	
        num = rule_kd[pec][pe+1];
        U1Fuzzy_kd[num] += eFuzzy[1]*ecFuzzy[0];
    }
    else				       // e和e'同时达到边缘
    {
        num = rule_kd[pec][pe];
        U1Fuzzy_kd[num] += eFuzzy[0]*ecFuzzy[0];		
    }
    
//    /*-------------------------------------------*/
    /*---------------查询speed规则表-------------*/
    /*-------------------------------------------*/
	if(pe<(RANK-1) && pec_speed<(RANK-1))        // e和e'都没有达到边缘
	{
	    num = rule_speed[pec_speed][pe];
	    U1Fuzzy_speed[num] += eFuzzy[0]*ec_speedFuzzy[0];
	    
	    num = rule_speed[pec_speed][pe+1];
	    U1Fuzzy_speed[num] += eFuzzy[1]*ec_speedFuzzy[0];	
	    
	    num = rule_speed[pec_speed+1][pe];
	    U1Fuzzy_speed[num] += eFuzzy[0]*ec_speedFuzzy[1];
	    
	    num = rule_speed[pec_speed+1][pe+1];
	    U1Fuzzy_speed[num] += eFuzzy[1]*ec_speedFuzzy[1];
	}
	else if(pe==(RANK-1) && pec_speed<(RANK-1))  // e达到边缘
	{
	    num = rule_speed[pec_speed][pe];
	    U1Fuzzy_speed[num] += eFuzzy[0]*ec_speedFuzzy[0];                
	    num = rule_speed[pec_speed+1][pe];
	    U1Fuzzy_speed[num] += eFuzzy[0]*ec_speedFuzzy[1];
	}
	else if(pe<(RANK-1) && pec_speed==(RANK-1))  // e'达到边缘
	{
	    num = rule_speed[pec_speed][pe];
	    U1Fuzzy_speed[num] += eFuzzy[0]*ec_speedFuzzy[0];	
	    num = rule_speed[pec_speed][pe+1];
	    U1Fuzzy_speed[num] += eFuzzy[1]*ec_speedFuzzy[0];
	}
	else				       // e和e'同时达到边缘
	{
	    num = rule_speed[pec_speed][pe];
	    U1Fuzzy_speed[num] += eFuzzy[0]*ec_speedFuzzy[0];		
	}
    
    /*-------------------------------------------*/
    /*-----------------解模糊--------------------*/
    /*-------------------------------------------*/
    for(int l=0;l<RANK;l++)
    {
        fuzzy_kp += U1Fuzzy_kp[l] * Rule_kp[l];
        fuzzy_kd += U1Fuzzy_kd[l] * Rule_kd[l];
        fuzzy_speed += U1Fuzzy_speed[l] * Rule_speed[l];
    }
}

