//-------------------------------------------------------------------------*
//文件名: includes.h                                                       *
//说  明: 总头文件,本文件包含:                                             *
//        主函数(main)文件中用到的头文件、外部函数声明及有关常量命名       *
//-------------------------------------------------------------------------*
#ifndef INCLUDE_H_
#define INCLUDE_H_

//1 头文件
//1.1通用头文件
//#include "MK60DZ10.h"
#include "common.h"            //通用函数头文件
#include <stdio.h>
#include <string.h>
#include "my_cfg.h"
#include "fun.h"                           //功能函数头文件
struct Bline
{
  uint16 line;//行
  int coord;  //列坐标
};

typedef struct
{
  int    SET_FLAG;
  uint8 ZHIDAO_SPEED;
  uint8 CD_SPEED[8];
  uint8 DIS_IMG_FLAG;
  int BILI;
  float DJ_KP[6];
  
  float KP;
  float KD;
  float KI;
}ParValue;
#define byte uint8
#define word uint16
//2 宏定义
#define endlineROW 240     //OV7620，每场240行
//#define  H 60   //采集行数
#define  V 80  //每行采集点数
//#define Cmp 160//摄像头二值化阈值
#define left_way 0  //舵机方向左
#define right_way 2 //舵机方向右
#define center_way 1 //舵机方向中
#define dj_left_max  dj_center+DJ_DIFF   //舵机左向最大值
#define dj_right_max dj_center-DJ_DIFF //舵机右向最大值

#define IMG_DIFF 10  //图像跟踪寻线算法差值  10
#define BW_DIFF 15  //黑线阈值与白色灰度值差 15
#define JUDGE_DIFF 10	//判断黑线是否有效斜率判断差值

#define KEY1 0x01
#define KEY2 0x02
#define KEY3 0x03
#define KEY4 0x04
#define KEY5 0x05

#define FAR_FIND_MIDDLE 1//全搜索标志
#define FAR_FIND_DIFF   0//跟踪搜索标志
#define SD_MODE 1

#define ROAD_SHIZI 3		//赛道标志 十字道
////////////////////////////提取用标志///////////////////////////////
#define LEFT_LOST_W 0x01u		//
#define LEFT_LOST_B 0x02u		//
#define RIGHT_LOST_W 0x04u		//
#define RIGHT_LOST_B 0x08u		//
#define ALL_LOST_W 0x10u		//
#define ALL_LOST_B 0x20u		//
#define ALL_LOST 0x40u			//
///////////////////////////判断用标志//////////////////////////////////
#define INVALID_LINE 0x01u			//
#define NONE_LOST 0 				//
#define DEAL_LEFT_LOST 0x2u		    //初始时左边线丢失标志
#define DEAL_RIGHT_LOST 0x4u		//初始时右边线丢失标志

//////////////////////编译宏定义///////////////////////////
#define SAVE_FLASH_DATA 0	//	保存flash参数宏定义
#define CHANG_TEST 0		//  场中断中LED亮宏定义


#define TEST_FLASH_ADDR 202
#define TEST_SECTOR_SIZE 2048
#define TEST_SD_SECTOR_SIZE 512

#define LCD_HEIGHT 64
//3全局变量定义

//
#define TEST_IO_H GPIOC_PDOR |= GPIO_PDOR_PDO(GPIO_PIN(15))
#define TEST_IO_L GPIOC_PDOR &=~GPIO_PDOR_PDO(GPIO_PIN(15))

#define LED_H GPIOA_PDOR |= GPIO_PDOR_PDO(GPIO_PIN(15))
#define LED_L GPIOA_PDOR &=~GPIO_PDOR_PDO(GPIO_PIN(15))
#define LED_TOGGLE GPIOA_PDOR ^=GPIO_PDOR_PDO(GPIO_PIN(15))

#define GPIO_PIN_MASK      0x1Fu    //0x1f=31,限制位数为0--31有效
#define GPIO_PIN(x)        (((1)<<(x & GPIO_PIN_MASK)))  //把当前位置1



#endif
