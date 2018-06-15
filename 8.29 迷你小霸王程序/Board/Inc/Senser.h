/*
* @file         Sensor.h
  * @brief      传感器采集相关函数实现
  * @author     xiao_fang
  * @version    v1.0
  * @date       2017-3-12
*/
#ifndef __SENSER_H__
#define __SENSER_H__

#define ADC_ACCURACY ADC_8bit   //定义AD采值精度

#define LEFT_LOSE_LINE_THRESHOLD    35  //定义左传感器丢线阈值
#define RIGHT_LOSE_LINE_THRESHOLD    35  //定义右传感器丢线阈值

#define LEFT_PISITION_GAIN           1.0        //定义左偏差调平值
#define RIGHT_PISITION_GAIN           1.0       //定义右偏差调平值

#define POSITION_BOUND               200        //定义偏差限幅值


void Sensor_init(void);         //初始化传感器
void Sensor_value_get(uint16 * sensor_value);   //读取一次传感器值并存入数组
void Senser_normalization(uint16 * sensor_value);        // 传感器数据归一化
float cal_deviation(uint16 * sensor_value);           //偏差计算
void loss_line_deal(uint16 * sensor_value , float position ,uint16 left_lose_threshold ,uint16 right_lose_threshold);        //丢线处理
uint8 out_line_deal(uint16 * sensor_value , float position);         //窜道处理
float position_filter(float position);                          //偏差滤波
void Senser_normalization_threshold_get(void);                   //传感器归一化阈值采集
void Senser_normalization_threshold_read(uint16 * normalization_threshold);       //传感器归一化阈值读取
float position_record(float position);                                           //偏差记录
#endif
