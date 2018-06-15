#include "include.h"


/*************************************************************************
*  模块名称：没有名称
*  功能说明：各种全局变量的定义以及初始化
*  参数说明：无
*  函数返回：无
*  修改时间：2012-2-14
*************************************************************************/
struct CarAngle Angle = {0,0,0,0,0,0,0,0,0,0};
struct CarSpeed Speed = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
struct CarDirection Direction = {0,0,0,0,0,0};
struct CarStartLine StartLine = {0,0,0,0};
struct CarRen Ren = {0,0,0,0,0,0,0,0,{0,0}};
struct CarBlock Block = {0,0,0,0};
long int Time_1ms=0;



/*************************************************************************
*  函数名称：main
*  功能说明：主函数
*  参数说明：无
*  函数返回：无r
*  修改时间：2012-2-14    一直在测试
*************************************************************************/
void main()
{
    DisableInterrupts;
    delayms(100);
    System_Init();
    delayms(100);
    EnableInterrupts;
    
    while(1)
    {
        while(1)
        {
            //测试时间,方波测试
            #if 0
            static int Test_num=0;
            if(Test_num==1)
            {
                Test_IO = 1;         
                Test_num = 0;
                //测量运行时间的函数放在这里
            }
            else
            {
                Test_IO = 0;
                Test_num = 1;
                //测量运行时间的函数放在这里
            }
            #endif
            
            
            //图像采集和处理和方向控制
            ov7725_get_img();
            Process_Image();
            Direction.output_enable = 1;
            

            //send infomation
            #if 0
            if(StartLine.flag==0)
            {   
                SCISend_to_Own(UART3);
            }
            #endif
        }
    }
}
