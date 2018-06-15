#include "path.h"
#include "control.h"
#include "includes.h"
#include "speed_new.h"
#include "direction.h"
#include "roundadout.h"
#include "info_deal_save.h"  
#include "ring.h"


uint8 run_start_flag=0;//是否开始跑
uint8 img_deal_flag=0;//开始采集处理图像
int line_lose_flag=0;
////////////////////////////////全局变量定义////////////////////////////////////

uint16 Bline_left[CAMERA_H];	 //左边线存放数组
uint16 Bline_right[CAMERA_H];	 //右边线存放数组
uint16 Pick_table[CAMERA_H];	 //中心线存放数组

uint16 left_pos[CAMERA_H];	 //左边线存放数组
uint16 right_pos[CAMERA_H];	 //右边线存放数组
uint16 center_pos[CAMERA_H];	 //中心线存放数组

uint8  Pick_flag[CAMERA_H];//该行是否找到黑线标志数组
uint8  Deal_flag[CAMERA_H];//处理数据是否有效标志数组
uint16 lost_already=0;
uint8 Pick_line=0;
uint16 maxBline_diff;
//uint8 PickCenter_flag=0;	//提取中线标志
uint8 Lost_Line_count=0;
uint8 Lost_left_count=0;
uint8 Lost_right_count=0;
uint8 Near_lost=0;//近处丢失标志
uint8 Shi_zi_line=0;
uint8 Shi_zi_num=0;
uint8 Shi_zi_flag=0;
uint8 stop_num=0;
int Position_diff=0;
uint8 Out_flag;		//出界标志，无效图像标志


uint8 last_vline;
uint8 valid_line=0;//最大有效行
uint8 judge_vl;//用于判断的有效行
uint8 last_lost=55;	//上一场丢失行
uint16 Bline_diff=0;//两黑线距离

/***************************************************************/
int judge_xielv[CAMERA_H-5];	//斜率判断数组 
uint8 zhidao_count_flag=0;	//直道判断标志
uint8 last_zhidao_flag=0;
uint8 lost_w_count=0;//白色丢失行变量
uint8 lost_b_count=0;//黑色丢失行变量

int even_diff=0;	//中心线平均偏差
int even_diff_near=0;
float D_differen=0.0;   //
int jiaodu_num=dj_center;	//角度值
int last_turn=dj_center;	//上一次转向值
int dj_pid_num=dj_center;	//舵机角度值

int dj_act_jiaodu=dj_center;//舵机实际角度

uint8 set_flag=1;//设置标志位
uint8 gl_zhangai_flag=0;
uint8 gl_zhangai_start=0;
uint8 gl_zhangai_end=0;

int ring_num=0;
int no_ring_num=0;
int RING_FLAG=0;
int HIGHT_RING_FLAG=0;
int one_flag=0;

int fact_weight[60]={0};

void control_hander()
{
    if((PIT_TFLG(1)&PIT_TFLG_TIF_MASK)!=0)
    {
      if(run_start_flag)
      {
            control_speed();
      }
    }
    PIT_TFLG(1)|=PIT_TFLG_TIF_MASK;       //清标志
}
void duoji_hander()
{
    if((PIT_TFLG(2)&PIT_TFLG_TIF_MASK)!=0)
    {
        img_deal_flag=1;
    }
    PIT_TFLG(2)|=PIT_TFLG_TIF_MASK;       //清标志
}
//功能 ；初始化控制的周期（pid的周期）
void init_control_circle()
{   
    set_vector_handler(PIT1_VECTORn,control_hander);   // 设置中断服务函数到中断向量表里
    set_vector_handler(PIT2_VECTORn,duoji_hander);   // 设置中断服务函数到中断向量表里
    pit_init_ms(PIT_PORT,PIT_CNT);
    pit_init_ms(PIT_PORT_time,PIT_time);//用于图像采集周期
    set_irq_priority((IRQn_Type)PIT1_VECTORn,2);
    set_irq_priority((IRQn_Type)PIT2_VECTORn,2);
}
void enable_pit_int(PITn_e pitn)
{
    enable_irq((IRQn_Type)((int)pitn + PIT0_IRQn));            //开中断
}
void disable_pit_int(PITn_e pitn)
{
    disable_irq((IRQn_Type)((int)pitn + PIT0_IRQn));            //开中断
}


void init_ctrl()
{
    init_control_circle(); //初始化PIT1，用于控制速度采集周期，没开中断
    //MOTOR_init();//初始化电机
    enable_pit_int(PIT2);//开始图像采集
#ifdef OPEN_CIRCLE_CONTROL //如果开环控制不开启控制速度的中断
      //qianjin();           //开环直接跑
#else
    enable_pit_int(PIT1);  //在PIT1中断中获取编码器值并控制住速度
//    lptmr_pulse_counter(BMQ_L_PIN);//PORTA19 为计数器信号输入，用于编码器计数
#endif
//    InitPar();//初始化参数//从内存读出数据 赋值给全局变量
   
}
void pirntf_path_type()
{
    switch(gl_path_type)
    {
    case Short_zhidao: printf("Short_zhidao\n");break;
    case lean_zhidao: printf("lean_zhidao\n");break;
    case Shi_zi: printf("Shi_zi\n");break;
    case Zhang_ai: printf("Zhang_ai\n");break;
    case Xiao_S: printf("Xiao_S\n");break;
    case Zhong_S: printf("Zhong_S\n");break;
    case Da_S: printf("Da_S\n");break;
    case Xiao_wan: printf("Xiao_wan\n");break;
    case Zhong_wan: printf("Zhong_wan\n");break;
    case Da_wan: printf("Da_wan\n");break;
    case T_Da_wan: printf("T_Da_wan\n");break;
    }
}
//改动了取中线行，改动了调整，改动了,改动了摇头舵机频率
int loop_none_hang()//环路最近处 和出口
{
    int lie=0;
    int ret=0,ret_1=0;
    int hang=0;
    for(hang=1;hang<6;hang++)//5
    {
        for(lie=0;lie<80;lie++)
        {
            if(get_img_point(hang,lie))
            {
                  ret++;      
            }    
        }
    }

    if(ret<3)//3
    {
        for(hang=6;hang<59;hang++)
        {
            if(get_img_point(hang,40))
            {
                  ret_1=hang;  
                  break;
            }    
        }
    }
    else
    {
        ret_1=1000;
    }
    return ret_1;

}
int ratio=0;

/*******************************************************************************
函数名称：stable_del
函数功能: 以前的数据处理控制程序
参数：无
*******************************************************************************/
int last_duoji_e=0;
int loop_ok_num=0;//圆环滤波变量

int in_shizi_flag=0;
int out_shizi_flag=0;
int other=0;
int other_one=0;
int shizi_on_off=0;
int loop_guaidian_flag=0;


void lose_line_find()
{
//  int hang=START_Y+3;
//  int left_lose_hang_pos=0,right_lose_hang_pos=0;
//  for(hang=8;hang<42;hang++)//先判断左边丢行情况
//  {
//    if(left_pos[hang]>40&&left_pos[hang]<75)//实际的边线丢行了
//    {
//        left_lose_hang_pos++;//记录丢行坐标
//    }
//  }
//  for(hang=8;hang<42;hang++)
//  {
//    if(right_pos[hang]>1&&right_pos[hang]<40)
//    {
//        right_lose_hang_pos++;//记录丢行坐标
//    }
//  }
////  if(left_lose_hang_pos>=30&&right_lose_hang_pos>=30)
////  {
////    line_lose_flag=1;
////  }
////  else 
//    
//   if((left_lose_hang_pos-right_lose_hang_pos)==0)
//  {
//    line_lose_flag=1;
//  }
//  else 
//  {
//    line_lose_flag=0;
//  }
  if(line_lockup(Bline_left,129)==1&&line_lockup(Bline_right,67)==1)
  {
    line_lose_flag=1;
  }
  else
  {
    line_lose_flag=0;
  }
    

}
//
//由于拐进圆环以后，查看底边是否有线，
int lose_pos_flag()
{
 int num=0;
 int ret=0;
 int left_num=0,right_num=0;
 if(ring_right_flag)
 {
    for(num=START_Y;num<=50;num++)
     {
       if(Bline_left[num]>=Bline_left[num+2])
       {
            left_num++;
            if(num>35)
            {
                ret=1;
                return ret;
            }
       }
       else
       {
            break;
       } 
     }
 }
 if(ring_left_flag)
 {
    for(num=START_Y;num<=50;num++)
     {
           if(Bline_right[num]<=Bline_right[num+2])
            {
                left_num++;
                if(num>35)
                {
                    ret=1;
                    return ret;
                }
                else
                {
                    break;
                }
            }     
     }
 
 }
 
 
    return ret;
    
}


int get_flag()
{   
    int num=30,i=8;
    int flag=0;
    if(ring_right_flag)
    {
        for(i;i<num;i++)
        {
            if(right_pos[i]==1)
            {
                flag++;
            }        
        }
    }
    else if(ring_left_flag)
    {
        for(i;i<num;i++)
        {
            if(left_pos[i]==79)
            {
                flag++;
            }        
        }
    }
    return flag;

}

int get_out_ring_w(int start,int end)
{
    int num=0;
    int weight=0;
    int flag=0;
    for(num=start;num<end;num++)
    {
        if(line_weight[num]>75||line_weight[num]==0)
        {
            flag++;
        }        
    }
    //weight=weight/(end-start);
    return flag;

}

    


int normal_flag=0;
int enter_ring_kp_flag=0;
int val=0;
int w=0;
int ww=0;
int num=0;
int time_num=0;
int val_num=0;
int out=0;
int ring_size=0;
int size_flag=0;
int ring_enter_process=0;

void get_ring_size()
{
  int hang=0,lie=0;
  lie=ring_lie_pos+12;
    for(hang=ring_hang_pos;hang<55;hang++)
    {
        if(get_img_point(hang,lie)==0&&get_img_point(hang+1,lie)!=0)
        {
            ring_size=hang;
            break;
        }
    }

}
void sigle_buxian()//单侧补线
{
    if(ring_right_flag)
    {
      if(even_diff>=0)
      {
        even_diff=-even_diff;
      }
      else 
      {
        with_right_to_center();  
      }
              
    }
    else if(ring_left_flag)
    {
      if(even_diff<=0)
      {
        even_diff=-even_diff;
      }
      else 
      {
        with_left_to_center();
      }
        
    }          
}
int again_clear_flag=0;
void stable_del()
{   
//        int side=38;  //35
        int side=38;  //35
  
        xielv_lvbo();//跳变差值限制法滤波 去除无效行
        lvbo(5);//滤波
        ti_jiaozheng(START_Y,valid_line);//矫正图像的梯形失真       	    
       
        bDistance();//赛道宽度法滤波 去除无效行
        //val=get_val(5,50,Pick_table,2);//求出环路时的中线拐点位置
        lose_line_find();//查看实际丢线情况
        //lose_pos_flag();
        
        ring_size=fing_enter_pos();
        w=get_line_weight(5,10);//赛道宽度
        ww=get_line_weight(25,30);//赛道宽度
        
        //check_and_stop(total_time,mark_stop());//起跑线检测
        //&&!start_line_ring_flag
//       if(!line_lose_flag&&!ring_fill_line_flag&&!out_ring_flag&&!out&&valid_line>=50&&!gl_zhangai_flag)//  判断出环路以后就不在判断了
        if(!line_lose_flag&&!ring_fill_line_flag&&!out_ring_flag&&!out&&valid_line>=50&&!gl_zhangai_flag&&(total_time>=50))//  判断出环路以后就不在判断了
       {//total_time
//           round_deal();//环岛识别 左右   
         //普通赛道跑好以后  打开注释
       }    
        if(Shi_zi_flag&&!ring_fill_line_flag&&!out)//环路与十字区分
        {   
            shizi_find_line();//重新找线//里面有判断圆环漏判
            bDistance();//赛道宽度法滤波 去除无效行  
        }
        /**************************************************************/
        /*************************************************************/
        if(ring_fill_line_flag&&!normal_flag&&ring_size>side)//圆环补线标志
        {//入环补线           
          sigle_buxian();
        }
       if(ring_fill_line_flag&&!normal_flag&&ring_size<=side&&!find_diu_hang(ring_size))
       {
            sigle_buxian();
       }
       else
        {
            getBlineCenter();//拟合出中线 //常规赛道 补线  
        }                    
      
      	averageLvBo();//均值滤波
        center_buxian();//也滤波
        If_LStraight();  //判断是否为直道        
       // check_and_stop(total_time,mark_stop());//起跑线检测
        get_even_diff_s();//获取有效行内中心线偏离平均值（获取误差）
        /*************************************************************/   
        if(ring_fill_line_flag&&!normal_flag)//进到环路关掉打死
        {          //入环打死
            zhidao_count_flag=0;//直道标志位置0
            if(ring_right_flag&&((ring_size<=side)||Shi_zi_flag||valid_line<=30)&&find_diu_hang(ring_size))//
            {
                even_diff=-33;    //防止方向抖动
                ring_enter_process=1;                
                enter_ring_kp_flag=1;                
                if(Shi_zi_flag>=10) 
//                 if((Shi_zi_flag<15&&Shi_zi_flag!=0)) 
                {
                    guai_shizi_ring_flag=1;//环路宽窄宽，第二个宽的位置打死
                }
            }
//            else if(ring_right_flag&&even_diff>=0&&find_diu_hang(ring_size)&&(ring_size<=35))
            else if(ring_right_flag&&ring_enter_process)
            {
               enter_ring_kp_flag=1;
               even_diff=-33;
            }              
            if(ring_left_flag&&((ring_size<=side)||Shi_zi_flag||valid_line<=30)&&find_diu_hang(ring_size))//
            {
                even_diff=33;  
                ring_enter_process=1;
                enter_ring_kp_flag=1;
                 if(Shi_zi_flag>=10) 
 //               if((Shi_zi_flag<15&&Shi_zi_flag!=0)) 
                {
                    guai_shizi_ring_flag=1;
                }
            }
//            else if(ring_left_flag&&even_diff<=0&&find_diu_hang(ring_size)&&(ring_size<=35))
            else if(ring_left_flag&&ring_enter_process)
            {
              enter_ring_kp_flag=1;
              even_diff=33;  
            }                        
        }  
//       
        if(guai_shizi_ring_flag==1&&Shi_zi_flag==0&&!normal_flag)
        {
            zhidao_count_flag=0;//直道标志位置0
            time_num++;//记录非十字的次数  
            //if(time_num>1)//滤波
            {
                time_num=0;
                normal_flag=1;//环路里面
            }
        }            
        if(normal_flag)//在环路里面预判出口
        {//出环打死
          zhidao_count_flag=0;//直道标志位置0
          enter_ring_kp_flag=0;
          ring_enter_process=0;
          val=get_out_ring_zhedian();//计算边线折点
            if(val<=18)//边线折点//  10  环路半径约越大  应该越小
            //if(val<=get_guai_val())//边线折点//  10  环路半径约越大  应该越小              
            {
                if(ring_right_flag)
                {
                    even_diff=-33;    //防止方向抖动
                    out_dasi_flag=1;
                }             
                else if(ring_left_flag)
                {
                    even_diff=33;  
                    out_dasi_flag=1;
                }                
            }                   
            if(val>32)//在环路里面折点靠后
            {
                val_num++;
                if(val_num>15)val_num=0;out_ring_flag=1;
            }           
        }

//        if(out_ring_flag&&out_dasi_flag&&out_dasi_flag&&(Shi_zi_flag<=15&&Shi_zi_flag>0))
        if(out_ring_flag&&out_dasi_flag&&out_dasi_flag&&Shi_zi_flag)
        {
          out=1;
          zhidao_count_flag=0;//直道标志位置0
          if(ring_right_flag)
          {
              even_diff=-33;    //防止方向抖动
          }             
          else if(ring_left_flag)
          {
              even_diff=33;  
          }    
        }
//        if(out&&out_ring_flag&&out_dasi_flag&&valid_line>=50)//40
        if(out&&out_dasi_flag&&valid_line>=50)//40
        {
            zhidao_count_flag=0;//直道标志位置0
            out_ring_flag=0;
            out=0;
            val=0;
            normal_flag=0;
            ring_size=0;
            enter_ring_kp_flag=0;
            ring_enter_process=0;
            out_dasi_flag=0;
            recognition_B_num=0;
            black_point_start=0;
            ring_clear();
        }        

        duoji_PD(even_diff);//传入误差通过舵机PD获取舵机转角  even_diff大于零左拐                         
       
        way_control(); //舵机转向

}
/*******************************************************************************
函数名称：clearDelPar
函数功能: 清零处理参数
参数：无
*******************************************************************************/
void clearDelPar()
{
	lost_already=0;
	Lost_Line_count=0;
	Lost_left_count=0;
	Lost_right_count=0;
	Pick_line=0;
	Shi_zi_line=0;
	Bline_diff=0;
	maxBline_diff=0;
	Shi_zi_flag=0;
	lost_w_count=0;
	lost_b_count=0;
        valid_line=0;
        gl_zhangai_start=0;
        gl_zhangai_end=0;
        gl_zhangai_flag=0;
        guai_dian_count=0;
        donw_guan_dian=0;
}
/******************************* 主函数  **************************************/
void ctrl_main()
{
    uint32 line;
 
    
    /////////////////////图像采集部分，找到两条边线///////////////////////////////
    clearDelPar();//清零处理参数
    for(line=0;line<IMG_H;line++)//存放边线中线数组清零
    {
        Bline_left[line]=V/2;
        Bline_right[line]=V/2;
        Pick_flag[line]=0;//赛道标志清零
        Deal_flag[line]=0;//处理标志清零        
        left_pos[line]=V/2;
        right_pos[line]=V/2;
        center_pos[line]=0;        
    }
    for(line=START_Y+2;line<IMG_H;line++) //提取各行中心点并处理意外
    //for(line=0;line<IMG_H;line++) //提取各行中心点并处理意外
    {
        if(line==START_Y+2)
        { 
            Near_lost=PickCenter_near();
        }
        else
        {
            PickCenter_diff(line);//寻找左右边线 （输入行的坐标，找到列的坐标） 
            //收集环路所需要的坐标
            left_pos[line]=Bline_left[line];//保存左边的坐标
            right_pos[line]=Bline_right[line];//保存右边的坐标
            fact_weight[line]=left_pos[line]-right_pos[line];//保存图像宽度
            center_pos[line]=(left_pos[line]+right_pos[line])/2;//合成中心坐标
        }
////////////////////////////////十字判别式///////////////////////////////////////
         if(Shi_zi_flag==0&&lost_w_count>=8)//在有效行丢失之前判断丢白线数判断十字道/8
        {           
              Shi_zi_flag=line;
              if(line>10)
                valid_line=line-10;
              else
                valid_line=0;
              break;
        }
///////////////////////////有效行判断//////////////////////////////////////////
        if((Pick_flag[line]&LEFT_LOST_B)||(Pick_flag[line]&RIGHT_LOST_B))
          {
              if((Pick_flag[line-1]&LEFT_LOST_B)||(Pick_flag[line-1]&RIGHT_LOST_B))
                  lost_b_count++;
          }
          if(line<25)
          {
              if(maxBline_diff<Bline_diff)
                  maxBline_diff=Bline_diff;
          }
          if(lost_already==0)            
          {
              if((lost_b_count>3||(Bline_left[line]<5)||(Bline_right[line]>(V-5)))&&valid_line==0)
              {
                  lost_already=1;
                  valid_line=line-3;
              }
          }
        if(line==IMG_H-1&&valid_line==0)
        {
            valid_line=IMG_H-1-lost_b_count;
        }
     }
     out_ring_under_find_line();
     stable_del();//处理图像

}   
