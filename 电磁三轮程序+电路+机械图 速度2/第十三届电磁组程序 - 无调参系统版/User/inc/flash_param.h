/*********************************************************************************************************************
 * 
 * @file       		flash_param.h
 *  				flash参数存取
 * @core			S9KEA128
 * @date       		2018
 ********************************************************************************************************************/
#ifndef _flash_param_
#define _flash_param_


#include "bsp_global_var.h"

#define PARAM_INIT_FLAG     (159)   //重载标志位

extern void param_in(void);   //从flash中读参数设置到变量中
extern void param_load(struct PARAM * p_param);
extern void set_param(struct PARAM *p_param);   //结构体给变量
extern void param_save(struct PARAM * p_param); //结构体保存参数给flash

#endif  /*_flash_param_*/