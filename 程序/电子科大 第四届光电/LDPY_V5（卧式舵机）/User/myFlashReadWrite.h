#ifndef MY_FLASH_READ_WRITE
#define MY_FLASH_READ_WRITE

#include "MK60DZ10.h"
#include "myStruct.h"
#include "LDPY_Flash.h"
#include "myLED.h"

#define FLASH_ARRAY_SIZE            (400)
#define FLASH_ARRAY_ADDR_1          (0x0007F400)	//第509k的地址
#define FLASH_ARRAY_ADDR_2          (0x0007F800)	//第510k的地址
#define FLASH_ARRAY_ADDR_3          (0x0007FC00)	//第511k的地址

void write_Data2Flash(Parameter_Info * Parameter_info,
                      CCD_Info * CCD1_info,
                      CCD_Info * CCD2_info,
					  HeadSteer_Info *HeadSteer_info,
                      Steer_Info * Steer_info,
                      Speed_Info * Speed_info,
                      Flash_Info * Flash_info,
                      Motor_Info * Motor_info
                      );

void read_DataFromFlash(Parameter_Info * Parameter_info,
					  CCD_Info * CCD1_info,
					  CCD_Info * CCD2_info,
					  HeadSteer_Info *HeadSteer_info,
					  Steer_Info * Steer_info,
					  Speed_Info * Speed_info,
					  Flash_Info * Flash_info,
					  Motor_Info * Motor_info
					  );


#endif
