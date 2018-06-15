/*********************************************************************************************************************
 * 
 * @file       		main.c
 *  				主函数
 * @core			S9KEA128
 * @date       		2018
 ********************************************************************************************************************/

#include "main.h"
   
int main(void)
{
    get_clk();              //获取时钟频率
    DisableInterrupts;
    car_init();
//	param_in();             //读取参数
    EnableInterrupts; 
    while(1)
    {
                oled_print_16x8short(0 ,0,sensor[0]);//显示水平左电感
                oled_print_16x8short(80,0,sensor[1]);//显示水平右电感
		
		oled_print_16x8short(10,3,sensor[2]);//显示垂直左电感
		oled_print_16x8short(70,3,sensor[3]);//显示垂直右电感
    }
}
