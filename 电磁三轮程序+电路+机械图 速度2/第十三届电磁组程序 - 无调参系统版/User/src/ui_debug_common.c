/*********************************************************************************************************************
 * 
 * @file       		ui_debug_common.c
 *  				通用调参界面
 * @date       		2018
 ********************************************************************************************************************/
#include "ui_debug_common.h"
#include "string.h"


uint8 set_page_16x8common(char *pstr_a, int16 *param_a,
				          char *pstr_b, int16 *param_b,
				          char *pstr_c, int16 *param_c,
				          char *pstr_d, int16 *param_d,
					      uint8 page_num)
{
	int8 head_num = 1;
	int8 bits_num = 1;
	int8 exit_flag = 0;
        int8 button_value = 0;
	int8 set_flag = 0;

	char pstr[4][40];
	int16 param[4];

	param[0] = *param_a;
	param[1] = *param_b;
	param[2] = *param_c;
	param[3] = *param_d;

	strcpy(pstr[0],pstr_a);
	strcpy(pstr[1],pstr_b);
	strcpy(pstr[2],pstr_c);
	strcpy(pstr[3],pstr_d); 
        
	oled_fill(0x00);
	oled_display_16x8str(0,0,pstr[0]);
	oled_print_16x8short(80,0,param[0]);
	oled_display_16x8str(0,2,pstr[1]);
	oled_print_16x8short(80,2,param[1]);
	oled_display_16x8str(0,4,pstr[2]);
	oled_print_16x8short(80,4,param[2]);
	oled_display_16x8str(0,6,pstr[3]);
	oled_print_16x8short(80,6,param[3]);    
        
	oled_display_16x8str_hl(0,(uint8)(2*(head_num-1)),pstr[head_num-1]);   
	while(!exit_flag)
	{

        //button_value = get_button_value();
        button_value = GetIRKey();

        SetIRKey(0);
		if(set_flag == 0)
		{
		    switch(button_value)
		    {

			     case BT_UP_PRESS:
				                  oled_display_16x8str(0,(uint8)(2*(head_num-1)),pstr[head_num-1]);  //上次选中菜单去掉反白显示
			                      if(head_num > 1)
	   	                          {
	   	  	                          head_num--;                                        //如果不是第一行则向上移动
	   	                          }
	   	                          else
	   	                          {
	   	  	                          head_num = 4;                                      //如果是第一行则移动到最后一行
	   	                          }
								  oled_display_16x8str_hl(0,(uint8)(2*(head_num-1)),pstr[head_num-1]); //本次选中菜单添加反白显示
							      break;
				case BT_DN_PRESS:
				                  oled_display_16x8str(0,(uint8)(2*(head_num-1)),pstr[head_num-1]);  //上次选中菜单去掉反白显示
				                  if(head_num < 4)
	   	                          {
	   	  	                          head_num++;                                        //如果不是最后一行则向下移动
	   	                          }
	   	                          else
	   	                          {
	   	  	                          head_num = 1;                                      //如果是最后一行则移动到第一行
	   	                          }
								  oled_display_16x8str_hl(0,(uint8)(2*(head_num-1)),pstr[head_num-1]); //本次选中菜单添加反白显示
								  break;
				case BT_LE_PRESS:
				                  page_num--;        //切换到上一页
								  exit_flag = 1;
								  break;
				case BT_RI_PRESS:
				                  page_num++;        //切换到下一页
								  exit_flag = 1;
								  break;
				case BT_OK_PRESS:
				                  oled_display_16x8str(0,(uint8)(2*(head_num-1)),pstr[head_num-1]);   //去掉菜单栏的反白显示
								  bits_num = 1;
								  oled_print_16x8short_hl(80,(uint8)(2*(head_num-1)),param[head_num-1],bits_num);
				                  set_flag = 1;      //按下确认键进入编辑模式
				                  break;
				default :         break;
		    }
		}
		else if(set_flag == 1)
		{
			switch(button_value)
			{
				case BT_LE_PRESS:
				                 if(bits_num > 1)
		    			         {
		    				         bits_num--;
		    			         }
		    			         else
		    			         {
		    				         bits_num = 5;
		    			         }
		    			         oled_print_16x8short_hl(80,(uint8)(2*(head_num-1)),param[head_num-1],bits_num);
								 break;
				case BT_RI_PRESS:
				                 if(bits_num < 5)
								 {
									 bits_num++;
								 }
								 else
								 {
									 bits_num = 1;
								 }
								 oled_print_16x8short_hl(80,(uint8)(2*(head_num-1)),param[head_num-1],bits_num);
				                 break;
				case BT_UP_PRESS:
				                 switch(bits_num)
								 {
									 case 1: param[head_num-1] += 10000;
											 break;
									 case 2: param[head_num-1] += 1000;
											 break;
									 case 3: param[head_num-1] += 100;
											 break;
									 case 4: param[head_num-1] += 10;
											 break;
									 case 5: param[head_num-1] += 1;
											 break;
									 default: break;
								 }
								 oled_print_16x8short_hl(80,(uint8)(2*(head_num-1)),param[head_num-1],bits_num);
								 break;
				case BT_DN_PRESS:
				                 switch(bits_num)
		   	                     {
		   	            	         case 1: param[head_num-1] -= 10000;
		   	            	                 break;
		   	            	         case 2: param[head_num-1] -= 1000;
		   	            	                 break;
		   	            	         case 3: param[head_num-1] -= 100;
		   	            	                 break;
		   	            	         case 4: param[head_num-1] -= 10;
		   	            	                 break;
		   	            	         case 5: param[head_num-1] -= 1;
		   	            	                 break;
		   	            	         default: break;
		   	                     }
		   	                     oled_print_16x8short_hl(80,(uint8)(2*(head_num-1)),param[head_num-1],bits_num);
								 break;
				case BT_OK_PRESS:
				                 oled_print_16x8short(80,(uint8)(2*(head_num-1)),param[head_num-1]);
								 oled_display_16x8str_hl(0,(uint8)(2*(head_num-1)),pstr[head_num-1]);
				                 set_flag = 0;      //按下确认键退出编辑模式
				                 break;
				default :        break;

			}
		}
    }

	*param_a = param[0];
	*param_b = param[1];
	*param_c = param[2];
	*param_d = param[3];

	//strcpy(pstr_a,pstr[0]);
	//strcpy(pstr_b,pstr[1]);
	//strcpy(pstr_c,pstr[2]);
	//strcpy(pstr_d,pstr[3]);

    return page_num;
}

