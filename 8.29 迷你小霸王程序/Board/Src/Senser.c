/*
* @file         Sensor.c
  * @brief      传感器采集相关函数实现
  * @author     xiao_fang
  * @version    v1.0
  * @date       2017-3-12
*/

#include "common.h"
#include "Senser.h"
#include "MKL_adc.h"
#include "Switch.h"
#include "VCAN_LED.H"
#include "24C02.h"
#include "MKL_gpio.h"

//                              左1，左2，左3，右3，右2，右1
uint16 sensor_value[6] =        {0,   0,   0,   0,   0,   0};  //储存传感器值
uint16 normalization_threshold[6] = {0,0,0,0,0,0};              ///储存传感器归一阀值
extern uint8 loss_line;                 //丢线指示，1左丢，2右丢，0不丢
extern uint8 loss_line_lock;            //丢线锁，丢线后自动上锁
extern uint8 circle_sign;
float position_accumulative[15] = {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f}; 
/*
 *  @brief      初始化传感器
 *  @since      v1.0
*/
void Sensor_init(void)
{
  adc_init(ADC0_SE0);
  adc_init(ADC0_SE1);
  adc_init(ADC0_SE2);
  adc_init(ADC0_SE3);
  adc_init(ADC0_SE5a);
  adc_init(ADC0_SE6a);
}

/*
 *  @brief      读取一次传感器值并存入数组
 *  @since      v1.0
 * sensor_value         数据保存位置
*/
void Sensor_value_get(uint16 * sensor_value)
{
      sensor_value[3] = adc_once( ADC0_SE0, ADC_ACCURACY);   //AD5 左2 PTE20
      sensor_value[2] = adc_once( ADC0_SE6a, ADC_ACCURACY);   //AD6 左1 PTE22
      sensor_value[1] = adc_once( ADC0_SE2, ADC_ACCURACY);   //AD2 左3 PTE19
  
      sensor_value[5] = adc_once( ADC0_SE1, ADC_ACCURACY);  //AD1 右3 PTE17
      sensor_value[4] = adc_once( ADC0_SE3, ADC_ACCURACY);   //AD4 右2 PTE18
      sensor_value[0] = adc_once( ADC0_SE5a, ADC_ACCURACY);   //AD3 右1 PTE16  
}

/*
 *  @brief      传感器数据归一化
 *  @since      v1.0
 *   sensor_value_normal   归一化后数据
 *   sensor_value       归一化前数据
*/
void Senser_normalization(uint16 * sensor_value)
{
  sensor_value[0] = (int)((sensor_value[0] / (float)(normalization_threshold[0])) * 100);
  sensor_value[1] = (int)((sensor_value[1] / (float)(normalization_threshold[1])) * 100);
  sensor_value[2] = (int)((sensor_value[2] / (float)(normalization_threshold[2])) * 100);
  sensor_value[3] = (int)((sensor_value[3] / (float)(normalization_threshold[3])) * 100);
  sensor_value[4] = (int)((sensor_value[4] / (float)(normalization_threshold[4])) * 100);
  sensor_value[5] = (int)((sensor_value[5] / (float)(normalization_threshold[5])) * 100);
  if(sensor_value[1]>100)
    sensor_value[1]=100;
  if(sensor_value[4]>100)
    sensor_value[4]=100;
}

/*
 *  @brief      偏差计算
 *  @since      v1.0
 *  ad_normal   归一化前数据
*/
float cal_deviation(uint16 * sensor_value)
{
	float amp = 10000.0f;		//放大倍数
	float pwr_total = 0.0f;
	float position = 0.0f;
	float ad_sum = 0.0f;
	float sensor_value_copy[6] = {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f};  
	float ad_cal[4] = {0.0f,0.0f,0.0f,0.0f};
	float minus[3] = {0.0f,0.0f,0.0f};
	float multiply[3] = {0.0f,0.0f,0.0f};
	float line[3] = {0.0f,0.0f,0.0f};
	float pwr[3] = {0.0f,0.0f,0.0f};
	float kp[3] = {0.0f,0.0f,0.0f};
	
	for(uint8 i = 0 ; i<6 ; i++)
	{
		sensor_value_copy[i] = (float)(sensor_value[i]);		//将原数组复制一份，以便处理 
	}
	 
	sensor_value_copy[0] += 2.5f;		//输入有值为0时会发生计算错误，所以每个值都加上1 
	sensor_value_copy[1] += 2.5f;
	sensor_value_copy[2] += 2.5f;
	sensor_value_copy[3] += 2.5f;
	sensor_value_copy[4] += 2.5f;
	sensor_value_copy[5] += 2.5f;
	
	ad_cal[0] = sensor_value_copy[0];
        ad_cal[1] = sensor_value_copy[2];
	ad_cal[2] = sensor_value_copy[3];
        ad_cal[3] = sensor_value_copy[5];		

//        ad_sum = ad_cal[0] + ad_cal[1] + ad_cal[2] + ad_cal[3];
        
//	ad_cal[0] = (ad_cal[0] / ad_sum) * 295.0f ;		//这个不懂
//	ad_cal[1] = (ad_cal[1] / ad_sum) * 295.0f ;
//	ad_cal[2] = (ad_cal[2] / ad_sum) * 295.0f ;
//	ad_cal[3] = (ad_cal[3] / ad_sum) * 295.0f ;
	
	minus[0] = (int)((ad_cal[1] - ad_cal[0]) * amp);		//相邻两个电感做差
	minus[1] = (int)((ad_cal[2] - ad_cal[1]) * amp);
	minus[2] = (int)((ad_cal[3] - ad_cal[2]) * amp);

	multiply[0] =((int)(ad_cal[0] + 10.0f)) * ((int)(ad_cal[1] + 10.0f));			//相邻两个电感相乘
	multiply[1] =((int)(ad_cal[1] + 10.0f)) * ((int)(ad_cal[2] + 10.0f));
	multiply[2] =((int)(ad_cal[2] + 10.0f)) * ((int)(ad_cal[3] + 10.0f));

	line[0] = minus[0] / multiply[0];			//差比积算出三条中线
	line[1] = minus[1] / multiply[1];
	line[2] = minus[2] / multiply[2];
        
//        printf("%5d %5d %5d\r\n",(int)(line[0]*100),(int)(line[1]*100),(int)(line[2]*100));
        
	line[0] = line[0] - 48.77;				//以第二条中线为基准做中线漂移，三线合一
	line[1] = line[1];
	line[2] = line[2] + 45.60f;

	pwr[0] = (int)(ad_cal[0] + ad_cal[1]);			//相邻两个电感做和
	pwr[1] = (int)(ad_cal[1] + ad_cal[2]);
	pwr[2] = (int)(ad_cal[2] + ad_cal[3]);

	pwr[0] = pwr[0] * pwr[0];				//上一步做出的和值再平方
	pwr[1] = pwr[1] * pwr[1];
	pwr[2] = pwr[2] * pwr[2];

	pwr_total = pwr[0] +pwr[1] + pwr[2];		//算出上一步三个平方的和

	kp[0] =((float)(pwr[0])) / ((float)(pwr_total));			//三个差分别比上总和得出三个比例
	kp[1] =((float)(pwr[1])) / ((float)(pwr_total));			//这个比例用于动态拟合最终的中线
	kp[2] =((float)(pwr[2])) / ((float)(pwr_total));			//为啥这么算我也不知道

	position = ((kp[0] * line[0]) + (kp[1] * line[1]) + (kp[2] * line[2])) * 1;
        
	return position;
}

/*
 *  @brief         丢线处理
 *  @since         v1.0
 *  sensor_value   传感器值
 *　position       偏差值       
*/
void loss_line_deal(uint16 * sensor_value , float position ,uint16 left_lose_threshold ,uint16 right_lose_threshold)
{
        static int32 error_add[3];
        int32 error_total = 0;
        uint16 sensor_value_total = 0;
        static uint8 loss_line_inc;
        
        for(uint8 i = 2 ; i>0 ; i--)
        {
            error_add[i] = error_add[i-1];              //数组循环左移
        }
        error_add[0] = (int)(position * 100);           //将当次的偏差放到数组头部
        error_total = error_add[0]  + error_add[1] + error_add[2];      //最近三次偏差求和，用于判断是哪边丢线
        sensor_value_total = sensor_value[0] + sensor_value[2] + sensor_value[3] + sensor_value[5];                        //6个传感器值相加 
        
        if((sensor_value_total < left_lose_threshold) || (sensor_value_total < right_lose_threshold))            //根据6个传感器的相加值判断丢线
        {
            if((error_total > 0) && (loss_line == 0) && (sensor_value_total < left_lose_threshold))
            {
              loss_line = 1;                     //左丢线时返回1
            }
            else if((error_total < 0) && (loss_line == 0) && (sensor_value_total < right_lose_threshold)) 
            {
              loss_line = 2;                     //右丢线时返回2
            }
            else
            {;;}
        }
        else
        {
            if(loss_line_lock == 1)             //滤波
            {
                loss_line_inc ++;
                if(loss_line_inc > 7)
                {
                    loss_line_lock = 0;
                    loss_line_inc =0;
                }
            }
            if(loss_line_lock == 0)
            {
                loss_line = 0;
            }
        }
}

/*
 *  @brief         窜道处理
 *  @note          实验函数，效果未知
 *  @since         v1.0
 *　position       偏差值
*/
uint8 out_line_deal(uint16 * sensor_value , float position)
{
    uint16 sensor_value_add[3];
    int16 sensor_value_minus;
    sensor_value_add[0] = sensor_value[0] + sensor_value[2] + sensor_value[3] + sensor_value[5];
    sensor_value_add[1] = sensor_value[0] + sensor_value[2];
    sensor_value_add[2] = sensor_value[3] + sensor_value[5];
    sensor_value_minus = sensor_value_add[1] - sensor_value_add[2];
    if((int)(position) > 0)
    {
        if(sensor_value_add[0] < 10)
        {
            if(ABS(sensor_value_minus) < 2)
            {
                return 1;                       //左丢线返回1
            }
        }
    }
    if((int)(position) < 0)
    {
        if(sensor_value_add[0] < 10)
        {
            if(ABS(sensor_value_minus) < 2)
            {
                return 2;                       //右丢线返回2
            }
        }
    } 
    return 0 ;                                   //不丢线返回0
}

/*
 *  @brief         偏差滤波
 *  @since         v1.0
 *　position       偏差值
*/
float position_filter(float position)
{
        static float position_add[10];
        float position_add_copy[10];
        int32 position_total = 0;
        float position_average;
        float temp = 0.0f;
        uint8 i = 0;
        uint8 j = 0;
        
        for( i = 9 ; i>0 ; i--)
        {
            position_add[i] = position_add[i-1];              //数组循环左移
        }
        position_add[0] = position;                           //将当次的偏差放到数组头部 
        
        for(i=0;i<10;i++)
        {
            position_add_copy[i] = position_add[i];
        }  
        
        for( i=0;i<9;i++)                   //冒泡排序
        {
            for( j=0;j<(9-i);j++)
            {
                    if(position_add_copy[j] > position_add_copy[j+1])
                    {
                            temp = position_add_copy[j];
                            position_add_copy[j] = position_add_copy[j+1];
                            position_add_copy[j+1] = temp;
                    }
            }
        }
        
        for( i=3 ; i<7 ;i++) 
        {
            position_total += position_add_copy[i];                        //去掉最小的3个和最大的3个，其余值相加
        } 
        position_average = position_total / 4.0f;
        
        if(position_average > 0.0f)                              //对左右偏差进行调平
        {
          position_average *= LEFT_PISITION_GAIN;
        }
        if(position_average < 0.0f)
        {
          position_average *= RIGHT_PISITION_GAIN;
        }
        
        if(position_average > POSITION_BOUND)                              //对偏差进行限幅
        {
          position_average = POSITION_BOUND;
        }
        if(position_average < (-(POSITION_BOUND)))
        {
          position_average = (-(POSITION_BOUND));
        }
        return position_average;  
}



/*
 *  @brief      传感器归一化阈值采集
 *  @since      v1.0
*/
void Senser_normalization_threshold_get(void)
{
  uint8 collect_sign = 0;
  uint16 sensor_value_now[6] = {0,0,0,0,0,0};
  uint16 sensor_value_last[6] = {0,0,0,0,0,0};
  uint16 sensor_value_max[6] = {0,0,0,0,0,0};
  
  while(((switch_read())&(0x01)) == 0)
  {
    if(collect_sign == 0)
    {
      led(LED0,LED_ON);
      led(LED1,LED_ON);
      systick_delay_ms(200);
      led(LED0,LED_OFF);
      led(LED1,LED_OFF);
    }
    collect_sign = 1;
    
    Sensor_value_get(sensor_value_now);
    for(uint8 i = 0 ; i<6 ; i++)
    {
      if(sensor_value_max[i] < MAX(sensor_value_now[i] , sensor_value_last[i]))
      {
        sensor_value_max[i] = MAX(sensor_value_now[i] , sensor_value_last[i]);
      }
    }
    for(uint8 i = 0 ; i<6 ; i++)
    {
      sensor_value_last[i] = sensor_value_now[i];
    }      
   }
    if(collect_sign == 1)
    {
      flash_24c02_write_byte((uint8)sensor_value_max[0], 0);
      flash_24c02_write_byte((uint8)sensor_value_max[1], 1);
      flash_24c02_write_byte((uint8)sensor_value_max[2], 2);
      flash_24c02_write_byte((uint8)sensor_value_max[3], 3);
      flash_24c02_write_byte((uint8)sensor_value_max[4], 4);
      flash_24c02_write_byte((uint8)sensor_value_max[5], 5);
      led(LED0,LED_ON);
      led(LED1,LED_ON);
      systick_delay_ms(200);
      led(LED0,LED_OFF);
      led(LED1,LED_OFF);
    }    
}
/*
 *  @brief      传感器归一化阈值读取
 *  @since      v1.0
*/
void Senser_normalization_threshold_read(uint16 * normalization_threshold)
{
  for(uint8 i=0;i<6;i++)
  {
    normalization_threshold[i] = (uint16)flash_24c02_read_byte(i);
  }
  printf("%5d %5d %5d %5d %5d %5d\r\n",normalization_threshold[0],normalization_threshold[1],normalization_threshold[2],normalization_threshold[3],normalization_threshold[4],normalization_threshold[5]);
}

/*
 *  @brief      偏差记录
 *  @since      v1.0
 *  记录的偏差数据储存在数组position_accumulative[]中
*/
float position_record(float position)
{
    uint8 i = 0;
    float position_weighted = 0.0f;    
    for( i = 14 ; i>0 ; i--)
    {
        position_accumulative[i] = position_accumulative[i-1];              //数组循环左移
    }
    position_accumulative[0] = position;
    position_weighted = (0.6*position_accumulative[0]) + (0.25*position_accumulative[1]) + (0.15*position_accumulative[2]);
    return position_weighted;
}