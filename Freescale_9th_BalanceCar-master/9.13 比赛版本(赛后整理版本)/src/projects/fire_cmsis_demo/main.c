/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名       ：main.c
 * 描述         ：DSP操作实验
 *
 * 实验平台     ：野火kinetis开发板
 * 库版本       ：
 * 嵌入系统     ：
 *
 * 作者         ：野火嵌入式开发工作室
 * 淘宝店       ：http://firestm32.taobao.com
 * 技术支持论坛 ：http://www.ourdev.cn/bbs/bbs_list.jsp?bbs_id=1008
**********************************************************************************/



#include "common.h"
#include "include.h"

#include "arm_math.h"



/*************************************************************************
*                             野火嵌入式开发工作室
*                            CMSIS库cos、sin测试实验
*
*  实验说明：野火CMSIS库cos、sin测试实验
*
*  实验操作：无
*
*  实验效果：测试成功，则LED流水灯
*
*  实验目的：熟悉CMSIS库函数的调用
*
*  修改时间：2012-3-26     已测试
*
*  备    注：
*************************************************************************/
#define DELTA           (0.000001f)             //浮点数精度

float32_t a = 0.45454;                          //乱来的数据
float32_t cosa, sina, cos2a, sin2a;             //a 的余弦值、正弦值、余弦值平方、正弦值平方、
float32_t test;                                 //测试结果


void main(void)
{
    //float32_t b=1,c=2,d;
    q7_t b = 2, c = 2, d;
    float32_t diff;                             //差值

    //arm_mult_f32(&b,&c,&d,1);
    arm_mult_q7(&b, &c, &d, 1);

    LED_init();                                 //初始化LED

    cosa = arm_cos_f32(a);                      //求cos值，cosa = cos(a);
    sina = arm_sin_f32(a);                      //求sin值，sina = sin(a);

    arm_mult_f32(&cosa, &cosa, &cos2a, 1);      //乘法，cos2a = cosa * cosa ，即平方
    arm_mult_f32(&sina, &sina, &sin2a, 1);      //乘法，sin2a = sina * sina ，即平方

    arm_add_f32(&cos2a, &sin2a, &test, 1);      //加法，test = cos2a + sin2a = cosa * cosa  + sina * sina = 1

    diff = fabsf( 1.000000000 - test);          //求绝对值

    if( diff <= DELTA)                          //如果测试结果在精度范围之内
    {
        while(1)
        {
            water_lights();                     //流水灯表示成功
        }
    }
}

