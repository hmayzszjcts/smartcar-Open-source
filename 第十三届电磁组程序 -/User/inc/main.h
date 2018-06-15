/*********************************************************************************************************************
 * 
 * @file       		includefile.h
 *  				头文件
 * @core			S9KEA128
 * @date       		2018
 ********************************************************************************************************************/
#include "common.h"
#include "KEA128_port_cfg.h"
#include "gpio_icf.h"
#include "stdio.h"

//--------函数库--------//
#include "KEA128_ftm.h"
#include "KEA128_pit.h"
#include "KEA128_gpio.h"
#include "KEA128_port.h"
#include "KEA128_adc.h"
#include "KEA128_irq.h"
#include "KEA128_uart.h"
#include "KEA128_kbi.h"
#include "KEA128_flash.h"
#include "KEA128_systick.h"


//--------用户函数--------//
#include "isr.h"
#include "car_init.h"
#include "bsp_oled.h"
#include "bsp_my_oled.h"
#include "ui_debug.h"
#include "ui_debug_common.h"
#include "ui_setpage.h"
#include "bsp_global_var.h"
#include "flash_param.h"
#include "ir.h"
#include "speed.h"
#include "scope.h"
#include "direction.h"



