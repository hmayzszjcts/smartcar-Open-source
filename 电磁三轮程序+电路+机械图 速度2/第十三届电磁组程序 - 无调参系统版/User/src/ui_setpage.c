/*********************************************************************************************************************
 * 
 * @file       		ui_dsetpage.c
 *  				µ÷²ÎÒ³Ãæ
 * @date       		2018
 ********************************************************************************************************************/
#include "common.h"
#include "ui_debug_common.h"
#include "ui_debug.h"
#include "flash_param.h"
#include "bsp_my_oled.h" 
#include "ir.h"

uint8 set_page_1(void)
{
	uint8 page_num = PAGE_MIN;
	int8  exit_flag = 0;
	int8  button_value = 0;

	oled_fill(0x00);
    oled_display_16x8str(22,0,"Debug page");
    oled_display_16x8str(47,2,"home");
    oled_display_16x8str(14,4,"Left or right");

	while(!exit_flag)
	{
        button_value = GetIRKey();
        
        SetIRKey(0);
        
		switch(button_value)
		{
			case BT_UP_PRESS: break;
			case BT_DN_PRESS: break;
			case BT_RI_PRESS: page_num++;
			                  exit_flag = 1;
							  break;
			case BT_LE_PRESS: page_num = PAGE_MAX;
			                  exit_flag = 1;
							  break;
			default:          break;
		}
	}

    return page_num;  

}


uint8 set_page_2(struct PARAM *p_param)
{
	 return set_page_16x8common("dir_P",   &(p_param->float01),
							    "dir_D",   &(p_param->float02),
							    "float03",     &(p_param->float03),
							    "float04",        &(p_param->float04),
							    2
							    );
}

uint8 set_page_3(struct PARAM *p_param)
{
	 return set_page_16x8common("ExpectSpeed",   &(p_param->float05),
							    "speed_P",   &(p_param->float06),
							    "speed_I",     &(p_param->float07),
							    "float08",        &(p_param->float08),
							    3
							    );
}
uint8 set_page_4(struct PARAM *p_param)
{
	 return set_page_16x8common("float09",   &(p_param->float09),
							    "float10",   &(p_param->float10),
							    "float11",     &(p_param->float11),
							    "float12",        &(p_param->float12),
							    4
							    );
}

uint8 set_page_5(struct PARAM *p_param)
{
	uint8 page_num = PAGE_MAX;
	int8  exit_flag = 0;
	int8  button_value = 0;

	oled_fill(0x00);
	oled_display_16x8str(5,2,"Exit set param ?");

	while(!exit_flag)
	{
          button_value = GetIRKey();
          
          SetIRKey(0);
		switch(button_value)
		{
			case BT_UP_PRESS: break;
			case BT_DN_PRESS: break;
			case BT_RI_PRESS: page_num = PAGE_MIN;
			                  exit_flag = 1;
							  break;
			case BT_LE_PRESS: page_num--;
			                  exit_flag = 1;
							  break;
			case BT_OK_PRESS: page_num = EXIT_SET;
							  exit_flag = 1;
                              param_save(p_param);
							  break;
			default:          break;
		}
	}

    return page_num;  
    
}
