/******************** (C) COPYRIGHT 2016 泰庆电子科技 ********************
 * 文件名       ：flash.c
 * 描述         ：flash驱动函数
 *
 * 实验平台     ：火箭开发板
 * 库版本       ：火箭1.0版
 * 嵌入系统     ：恩智浦智能车
 *
 * 作者         ：DMX
 * 技术支持论坛 ：http://www.taiqdz.cn
 * 备注	       ：从网上例程修改而成
**********************************************************************************/


#ifndef _FLASH_H_
#define _FLASH_H_

//k60N512包含512K的程序Flash
//512K的程序Flash分为256个扇区，每个扇区2K大小
//K60X256包含256K的程序Flash
//512K的程序Flash分为128个扇区，每个扇区2K大小
//    sector（2K）为擦除最小单位
//    长字（32b）为写的最小单位

#include "common.h"
#include "include.h"

#define MEMORIES_ADDRESS0 253
#define MEMORIES_ADDRESS1 254
#define MEMORIES_ADDRESS2 255


__RAMFUN	void 	flash_init();													//擦除指定flash扇区

__RAMFUN 	uint8 	flash_erase_sector	(uint16 sectorNo);									//擦除指定flash扇区
__RAMFUN 	uint8 	flash_write		(uint16 sectorNo,uint16 offset,uint32 data);					        //写入flash操作
__RAMFUN 	uint8 	flash_write_buf		(uint16 sectorNo,uint16 offset,uint16 cnt,uint8 buf[]);					//从缓存区写入flash操作



__RAMFUN        uint8   Flash_read(uint16 sectNo,uint16 offset,uint16 cnt,uint8*bBuf);
extern int speed_level;
extern USHORT16 User_parameter[12];
void write_speed_gear1(void);
void write_speed_gear2(void);
void write_speed_gear3(void);
void write_speed_gear4(void);
void write_speed_gear5(void);


#define		flash_read(sectorNo,offset,type)		(*(type *)((uint32)(((sectorNo)<<11) + (offset))))			//读取扇区
#endif //_FLASH_H_ 
