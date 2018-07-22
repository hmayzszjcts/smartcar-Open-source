#ifndef __MYFLASH_H__
#define __MYFLASH_H__

#include "myStruct.h"
#include "LDPY_Flash.h"

#define FLASH_ARRAY_SIZE            (40)
#define FLASH_ARRAY_ADDR_1          (0x0007F400)//第510k的地址,存放设置参数，速度，舵机PID等
#define FLASH_ARRAY_ADDR_2          (0x0007F800)//第510k的地址,存放设置参数，速度，舵机PID等
#define FLASH_ARRAY_ADDR_3          (0x0007FC00)//第511k的地址，

void myFlash_WritePara2Flash(
                      Car_State * Car_state,
                      Parameter_Info * Parameter_info,
                      CCD_Info * CCD1_info,
                      CCD_Info * CCD2_info,
                      Steer_Info * Steer1_info,
                      Steer_Info * Steer2_info,
                      Speed_Info * Speed1_info,
                      Speed_Info * Speed2_info,
                      Flash_Info * Flash_info,
                      Motor_Info * Motor1_info,
                      Motor_Info * Motor2_info
                      );
//void read_max_AD_from_flash(Sensor * sensor,Para  para[]);
void myFlash_ReadFlash(
                  Car_State * Car_state,
                  Parameter_Info * Parameter_info,
                  CCD_Info * CCD1_info,
                  CCD_Info * CCD2_info,
                  Steer_Info * Steer1_info,
                  Steer_Info * Steer2_info,
                  Speed_Info * Speed1_info,
                  Speed_Info * Speed2_info,
                  Flash_Info * Flash_info,
                  Motor_Info * Motor1_info,
                  Motor_Info * Motor2_info
                  );


#endif

/*-----------------------------end of myFlash.h--------------------------------*/
