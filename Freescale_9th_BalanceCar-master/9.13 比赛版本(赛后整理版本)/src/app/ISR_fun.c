#include "include.h"
#include "math.h"
/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名       ：ISR_fun.c
 * 描述         ：高端霸气的中断服务函数的子函数，供给ISR.c
 * 实验平台     ：凌立印象开发板
 * 库版本       ：基于野火库
 * 嵌入系统     ：
 * 作者         ：xuxu
**********************************************************************************/



/*************************************************************************
*  函数名称：Water_LEDs
*  功能说明：自己的4个led灯
*  参数说明：无
*  函数返回：无
*  修改时间：2012-2-14    已测试
*************************************************************************/
void Water_LEDs()
{
    unsigned char Led_num = (Time_1ms/500)%4;
    if(Led_num==0)
    {_LED1 = 0;_LED2 = 1;_LED3 = 1;_LED4 = 1;}
    else if(Led_num==1)
    {_LED1 = 1;_LED2 = 0;_LED3 = 1;_LED4 = 1;}
    else if(Led_num==2)
    {_LED1 = 1;_LED2 = 1;_LED3 = 0;_LED4 = 1;}
    else if(Led_num==3)
    {_LED1 = 1;_LED2 = 1;_LED3 = 1;_LED4 = 0;}
}



/*************************************************************************
*  函数名称：Measure_speed
*  功能说明：测量速度
*  参数说明：无
*  函数返回：无
*  修改时间：2012-2-14    已测试
*************************************************************************/
void Measure_speed()
{
    static unsigned char SpeedDirection_L;
    static unsigned char SpeedDirection_R;
    SpeedDirection_L = (FTM1_QDCTRL & FTM_QDCTRL_QUADIR_MASK);
    SpeedDirection_R = (FTM2_QDCTRL & FTM_QDCTRL_QUADIR_MASK);
    //左轮测速
    if(SpeedDirection_L==0)	//正转
    {
        if(10000 - FTM1_CNT < 5000)//防止bug
          Speed.L_5ms = 10000 - FTM1_CNT;
    }
    else                        //反转
    {
        if(FTM1_CNT < 5000)//防止bug
          Speed.L_5ms = 0 - FTM1_CNT;
    }
    //右轮测速
    if(SpeedDirection_R==0)	//正转
    {
        if(10001 - FTM2_CNT < 5000)//防止bug
          Speed.R_5ms = 0 - (10000-FTM2_CNT);
    }
    else                         //反转
    {
        if(FTM2_CNT < 5000)//防止bug
          Speed.R_5ms = FTM2_CNT;
    }
    #define ratio 1.953125//后来换了小的龙丘编码器，256线，原来用的500线
    Speed.L_5ms = (int)(Speed.L_5ms*ratio);
    Speed.R_5ms = (int)(Speed.R_5ms*ratio);
    //clear
    FTM1_CNT = 0;
    FTM2_CNT = 0;
}


/*************************************************************************
*  函数名称：Measure_Acc
*  功能说明：测量加速度计值
*  参数说明：无
*  函数返回：无（但包含全局变量a_sin和a_cos）
*  修改时间：2012-2-14    已测试
*************************************************************************/
void Measure_Acc()
{
    static float a_sin_new=0,a_sin[3];
    static float a_cos_new=0,a_cos[3];
    
    for(int i = 0;i < 3;i++)
        a_sin[i] = Get_mma8451_once('Z');
    a_sin_new = (a_sin[0] + a_sin[1] + a_sin[2])/3.0;
    
    for(int i = 0;i < 3;i++)
        a_cos[i] = Get_mma8451_once('X');
    a_cos_new = (a_cos[0] + a_cos[1] + a_cos[2])/3.0;
    
    //低通滤波
    Angle.a_sin = Angle.a_sin*0.5 + a_sin_new*0.5;//（90至-90）sin值 1到0到-1变化,左为正
    Angle.a_cos = Angle.a_cos*0.5 + a_cos_new*0.5;//（90至-90) cos值 0到-1到0变化,上为负
}



/*************************************************************************
*  函数名称：Angle_control
*  功能说明：角度融合，计算出角度，计算角度pwm输出
*  参数说明：无
*  函数返回：无
*  修改时间：2012-2-14    已测试
*************************************************************************/
#define Gyro_OFFSET (-4)
void Angle_control()
{
    //直立
    #define k_Gyro 0.07   //角速度放大倍数
    #define dt 0.005
    #define tg 2         //**跟踪时间:1~4
    static float mG_delta=0;      //和angle_speed一个级别
    static float Angle_Gyro_old=0;
    
    //用哪个三角函数决定于怎么安装，实测竖着安装用asin好
    /*if(Angle.a_cos <= 0)
    {
        if(Angle.a_cos==0)
            Angle.m_angle = asin(Angle.a_sin)*57.295779513;  	//180/pi=57.295779513
        else
            Angle.m_angle = atan(Angle.a_sin/(0 - Angle.a_cos))*57.295779513;
    }
    else
    {
        Angle.m_angle = atan(Angle.a_sin/Angle.a_cos)*57.295779513;
        Angle.m_angle = 180 - Angle.m_angle;
    }*/
    Angle.m_angle = asin(Angle.a_sin)*57.295779513;
    if(Angle.m_angle > 90)
        Angle.m_angle = 90;
    else if(Angle.m_angle < -90)
        Angle.m_angle = -90;
   
    
    //得到Angle.G_angle
    Angle_Gyro_old = Angle.Gyro;
    Angle.Gyro = (float)(Get_Gyro(1,'X') - Gyro_OFFSET);
    Angle.speed = 0 - k_Gyro *(Angle_Gyro_old + Angle.Gyro)/2.0;
    mG_delta = (Angle.m_angle - Angle.G_angle) / tg;
    Angle.G_angle += (Angle.speed + mG_delta) * dt;
        
    //得到Angle.PWM
    if(Ren.flag == 0)//正常情况下
    {
        Angle.PWM = (Angle.OFFSET - Angle.G_angle)*Angle.P
                      - Angle.speed * Angle.D;
    }
    else//有人字情况
    {
        Angle.PWM = (Angle.OFFSET - Angle.G_angle)*(Angle.P +0)
                      - Angle.speed * (Angle.D + 0);
    }
}




/*************************************************************************
*  函数名称：Speed_control
*  功能说明：计算速度pwm输出
*  参数说明：无
*  函数返回：无
*  修改时间：2012-2-14    已测试
*************************************************************************/
void Speed_control()
{
    static unsigned char speed_Period=0;//速度控制周期变量
    #define speed_Period_Constant 20 //速度控制周期
    static float speed_Delta=0;
    static float Tmp_Speed_P;
    static int temp_L_100ms=0,temp_R_100ms=0;
    
    speed_Period++;
    temp_L_100ms += Speed.L_5ms;
    temp_R_100ms += Speed.R_5ms;
    if(speed_Period >= 20)//速度PID反馈调试。100ms一次计算
    {
        speed_Period = 0;
        Speed.L_100ms = temp_L_100ms;
        Speed.R_100ms = temp_R_100ms;
        Speed.Car = (float)((Speed.L_100ms + Speed.R_100ms)/2.0);
        temp_L_100ms = 0;
        temp_R_100ms = 0;
        
        //速度控制
        speed_Delta = Speed.Goal - Speed.Car;
        
        //IIIIIIIIIIIIIIIIIIIII积分IIIIIIIIIIIIIIIIIII
        #define SPEEDUP_TIME 1500
        if(Time_1ms < Stand_Time + SPEEDUP_TIME)//站立之后的两秒之内：使劲跟随
        {
            if(Speed.Mode==3)
                Speed.PWM_Integral = 1000 + 3000*(Stand_Time + SPEEDUP_TIME - Time_1ms)/SPEEDUP_TIME;
            if(Speed.Mode==4)
                Speed.PWM_Integral = 0 + 3500*(Stand_Time + SPEEDUP_TIME - Time_1ms)/SPEEDUP_TIME;
            else
                Speed.PWM_Integral = 1000 + 3000*(Stand_Time + SPEEDUP_TIME - Time_1ms)/SPEEDUP_TIME;
        }
        else//站立之后的两秒之后
        {
            Speed.PWM_Integral += speed_Delta * Speed.I;
            //PWM_Integral限幅
            if(Speed.PWM_Integral > Speed.I_Limit_PWM_max)//限幅啊
                Speed.PWM_Integral = Speed.I_Limit_PWM_max;
            else if(Speed.PWM_Integral <= Speed.I_Limit_PWM_min)
                Speed.PWM_Integral = Speed.I_Limit_PWM_min;
        }
        
        //PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP
        //并不是普通限幅，用于还没快的时候
        if(speed_Delta> 0 - Speed.I_Error_Start && speed_Delta < Speed.I_Error_Start)
            Tmp_Speed_P = Speed.P * speed_Delta;
        else if(speed_Delta< 0 - Speed.I_Error_Start)
            Tmp_Speed_P = Speed.P * (0 - Speed.I_Error_Start);
        else if(speed_Delta > Speed.I_Error_Start)
            Tmp_Speed_P = Speed.P * Speed.I_Error_Start;
        
        //输出
        Speed.PWM_Per = ( Tmp_Speed_P
                         + Speed.PWM_Integral
                             - Speed.PWM)  /  speed_Period_Constant;
    }
    Speed.PWM += Speed.PWM_Per;
}




/*************************************************************************
*  函数名称：Direction_control和Direction_control_with_Ren
*  功能说明：计算方向pwm输出（包含和不包含人字）
*  参数说明：无
*  函数返回：无
*  修改时间：2012-2-14    已测试
*************************************************************************/
#define Dir_Gyro_OFFSET 10
void Direction_control()
{
    Direction.Gyro = (float)(Get_Gyro(1,'Y') - Dir_Gyro_OFFSET); //向右为负
    if(Block.flag==0)//有黑砖头障碍情况
    {
        Direction.PWM_Per = Direction.P * (black_centre - CENTER )	//输出
            + Direction.D * Direction.Gyro
                - Direction.PWM;
    }
    else//没有黑砖头障碍情况
    {
        if(Time_1ms < Block.time + Block.delaytime)
            Direction.PWM_Per = Direction.P * (black_centre + Block.Offset - CENTER )	//输出
                         + Direction.D * Direction.Gyro
                           - Direction.PWM;
        else
            Block.flag = 0;
    }
    Direction.PWM += Direction.PWM_Per;
}

void Direction_control_with_Ren()//???人字时候是否需要方向的D值？
{
    if(Time_1ms > Ren.record_time + Ren.Delaytime1)//看见了但是延时打角
    {
        Direction.Gyro = (float)(Get_Gyro(1,'Y') - Dir_Gyro_OFFSET); //向右为负
        if(Ren.Loaction > CENTER)//右拐
        {
            black_centre = 160 + Ren.Strength;
        }
        else//左拐
        {
            if(Speed.Mode==3)//摄像头歪了，3档位打左拐补丁
            {
                if(Multiple_line_enable==1)
                    black_centre = 160 - Ren.Strength - 27;
                else
                    black_centre = 160 - Ren.Strength - 22;
            }
            else if(Speed.Mode==4 || Speed.Mode==5)//摄像头歪了，4,5档位打左拐补丁
            {
                if(Multiple_line_enable==1)
                    black_centre = 160 - Ren.Strength - 33;
                else
                    black_centre = 160 - Ren.Strength - 30;
            }
            else//摄像头歪了，其它档位左拐和右拐正常
            {
                black_centre = 160 - Ren.Strength;
            }
        }
        
        //时间计数
        if(Time_1ms > Ren.record_time + Ren.Delaytime1 + Ren.Delaytime2)
        {
            Ren.flag = 0;
        }
        
        //输出
        Direction.PWM_Per=(Direction.P * (black_centre - CENTER)	//输出
                           + Direction.D * Direction.Gyro
                               - Direction.PWM);
        Direction.PWM += Direction.PWM_Per;
    }
}



/*************************************************************************
*  函数名称：Motor_Control
*  功能说明：pwm输出
*  参数说明：无
*  函数返回：无
*  修改时间：2012-2-14    已测试
*************************************************************************/

void Motor_Control()
{	
    static float pwm_Dead_L = 100;
    static float pwm_Dead_R = 100;
    static float all_PWM_L = 0;
    static float all_PWM_R = 0;
    
    //电机pwm输出    //300  400  500  600  700  800  900  1000 1100
    //测得的速度     //1130 2000 2750 3600 4350 5150 5870 6650 7450
    
    
    //计算总的输出
    if(Slope_flag == 0)//无坡道
    {
        all_PWM_L = Angle.PWM - Speed.PWM - Direction.PWM;
        all_PWM_R = Angle.PWM - Speed.PWM + Direction.PWM;
    }
    else//有坡道
    {
        if(Speed.Mode==1)
        {
            all_PWM_L = Angle.PWM - Speed.PWM*0.5 - Direction.PWM;
            all_PWM_R = Angle.PWM - Speed.PWM*0.5 + Direction.PWM;
        }
        else if(Speed.Mode==2)
        {
            all_PWM_L = Angle.PWM - Direction.PWM;
            all_PWM_R = Angle.PWM + Direction.PWM;
        }
        else if(Speed.Mode==3 || Speed.Mode==4 || Speed.Mode==5)
        {
            all_PWM_L = Angle.PWM + 2000 - Direction.PWM;
            all_PWM_R = Angle.PWM + 2000 + Direction.PWM;
        }
    }
    
    
    //倒地保护和终点线停车
    if(Angle.G_angle < -25 ||  Angle.G_angle > 40 || StartLine.flag==1)
    {
        if(Time_1ms > StartLine.time_record + StartLine.Delaytime)
        {
            all_PWM_L = 0;
            all_PWM_R = 0;
        }
    }
    
    //空转测速和磨轮胎
    //all_PWM_L = all_PWM_R=1000;
    
    
    //防止跳轮左右限幅:低速不用(国赛再调)
    //结果国赛也没调。。。慎用。。。
    if(Speed.Mode==1 || Speed.Mode==2 || Speed.Mode==3 || Speed.Mode==4)
    {
    }
    else if(Speed.Car > 5500)
    {
        if(all_PWM_L > 800 && all_PWM_R < 0)
            all_PWM_R = 0;
        else if(all_PWM_L < 0 && all_PWM_R > 800)
            all_PWM_L = 0;
    }
    
    
    //1.左面控制PWM
    if(all_PWM_L >= 0)
    {
        all_PWM_L += pwm_Dead_L;//加死区
        if(all_PWM_L >= 3990)   //限幅
            all_PWM_L = 3990;
        FTM_PWM_Duty(FTM0, CH2 , ((unsigned int)(all_PWM_L)) );//输出
        DIR_L_Forward;
    }
    else
    {
        all_PWM_L -= pwm_Dead_L;//加死区
        if(all_PWM_L <= -3990)  //限幅
            all_PWM_L = -3990;
        FTM_PWM_Duty(FTM0, CH2 , (unsigned int)(4000u + all_PWM_L));//输出
        DIR_L_Backward;
    }
    //2.右面控制PWM					
    if(all_PWM_R >= 0)
    {
        all_PWM_R += pwm_Dead_R;//加死区
        if(all_PWM_R >= 3990)   //限幅
            all_PWM_R = 3990;
        FTM_PWM_Duty(FTM0, CH1 ,  ((unsigned int)(all_PWM_R)));//输出
        DIR_R_Forward;
    }
    else
    {
        all_PWM_R -= pwm_Dead_R;//加死区
        if(all_PWM_R <= -3990)  //限幅
            all_PWM_R = -3990;
        FTM_PWM_Duty(FTM0, CH1 , (unsigned int)(4000 +  all_PWM_R)  );//输出
        DIR_R_Backward;
    }
}