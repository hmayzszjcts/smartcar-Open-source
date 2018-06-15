/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名       ：assert.c
 * 描述         ：断言条件不成立的执行函数
 *
 * 实验平台     ：野火kinetis开发板
 * 库版本       ：
 * 嵌入系统     ：
 * 备    注     ：
 *
 * 作者         ：
 * 淘宝店       ：http://firestm32.taobao.com
 * 技术支持论坛 ：http://www.ourdev.cn/bbs/bbs_list.jsp?bbs_id=1008
**********************************************************************************/

/*
 * File:        assert.c
 * Purpose:     Provide macro for software assertions
 *
 * Notes:       ASSERT macro defined in assert.h calls assert_failed()
 */

#include "common.h"
#include "led.h"

//const char ASSERT_FAILED_STR[] = "Assertion failed in %s at line %d\n";
const char ASSERT_FAILED_STR[] = "断言发生在%s文件的第%d行！\n断言条件不成立，请检测错误原因。\t—— by 野火\n\n";

/********************************************************************/
//如果断言条件不成立，进入了错误状态，就会打印错误信息和用LED来显示状态
void assert_failed(char *file, int line)
{
    //int i;
    LED_init();
    while (1)
    {
#ifdef DEBUG_PRINT
        printf(ASSERT_FAILED_STR, file, line);  //打印错误信息
#endif
        //for (i = 0xffffff; i; i--) ;
        water_lights();                         //用流水灯来指示进入错误状态
        //for (i = 0xffffff; i; i--) ;
    }
}
/********************************************************************/
