/*********************************************************************************************************************
 * 
 * @file       		ui_debug.c
 *  				调参界面
 * @date       		2018
 ********************************************************************************************************************/
#include "common.h"
#include "ui_debug.h"



void UI_debug(void)
{
	uint8 page_num = 1;
    
    while(page_num != EXIT_SET)
    {
      
            switch(page_num)
        {
            case 1: page_num = set_page_1();
                    break;
            case 2: page_num = set_page_2(&parameter);
                    break;
            case 3: page_num = set_page_3(&parameter);
                    break;
            case 4: page_num = set_page_4(&parameter);
                    break;
            case 5: page_num = set_page_5(&parameter);
                    break;                  
            default:break;
        }
    }
  
}