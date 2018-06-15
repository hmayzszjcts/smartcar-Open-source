/*!*****************************************************************
 *  黑工院 飞思卡尔 串口调参程序历程
 *  功能 ：通过蓝牙串口设置参数，支持自定义个数的调参过程历史记录
*          可自定义保存调试好的参数表个数（参数结构体），并支持删除
 *  使用方式 ：通过指令控制参数，指令表看程序中的指令表输出函数
 *作者 ：电子协会飞思卡尔团队
********************************************************************/
#ifndef _INFO_DEAL_SAVE_H_
#define _INFO_DEAL_SAVE_H_
#include "includes.h"
#include "MK60_flash.h"
extern uint8_t cmd_buf[30];//用来储存串口接收到的字符串
extern uint8_t rec_cmd_flag;//出口接收到字符串后置1
//**************需要改的参数**********************
#define Par_DEBUG_ADDR 150 //调试的时候用的储存地址
#define MAX_Par_DEBUG_COUNT 10 //调试的参数历史记录保存个数
#define Par_SAVE_ADDR  151 //将调试好的参数保存到这个地址
#define MAX_Par_SAVE_COUNT  10 //保存最多的调试好的参数表个数
#define SECTOR_SIZE FLASH_SECTOR_SIZE//扇页大小 FN15是(4*1024)，DZ10是(2*1024)
#define PAR_STRUCT_TYPE_NAME ParValue//自己定义的结构体类型的名
#define PAR_STRUCT_NAME myPar_num//自己定义的结构体的名
#define FLASH_ALIGN FLASH_ALIGN_ADDR //flash一次要写多少个字节，FN15是8个，DZ10是4个
#define FLASH_WRITE_CMD  0x07     //FN15是0x07，DZ10是0x06  flash的命令，指定一次写几个字节
//******************下边不需要改动****************************************
#define PAR_STRUCT_SIZE (sizeof(PAR_STRUCT_TYPE_NAME)+(sizeof(PAR_STRUCT_TYPE_NAME)%FLASH_ALIGN))//保证写入的数据大小是地址对齐整数倍
#define Par_Debug_head_addr (FLASH_SECTOR_SIZE*Par_DEBUG_ADDR)//调试用的储存首地址
#define Par_Save_head_addr  (FLASH_SECTOR_SIZE*Par_SAVE_ADDR) //调试好的参数储存地址

//宏定义 FCCOB Number
#if defined(MK60DZ10)
#define     FCMD        FTFL_FCCOB0         //FTFL 命令

#define     FADDR2      FTFL_FCCOB1         //Flash address [23:16]
#define     FADDR1      FTFL_FCCOB2         //Flash address [15:8]
#define     FADDR0      FTFL_FCCOB3         //Flash address [7:0]

#define     FDATA0      FTFL_FCCOB4         //Data Byte 0       //注意一点，4字节排序，FDATA3是最低位，FDATA0是最高位
#define     FDATA1      FTFL_FCCOB5         //Data Byte 1
#define     FDATA2      FTFL_FCCOB6         //Data Byte 2
#define     FDATA3      FTFL_FCCOB7         //Data Byte 3
#define     FDATA4      FTFL_FCCOB8         //Data Byte 4
#define     FDATA5      FTFL_FCCOB9         //Data Byte 5
#define     FDATA6      FTFL_FCCOBA         //Data Byte 6
#define     FDATA7      FTFL_FCCOBB         //Data Byte 7

//FCMD 命令
#define     RD1BLK    0x00   // 读整块Flash
#define     RD1SEC    0x01   // 读整个扇区
#define     PGMCHK    0x02   // 写入检查
#define     RDRSRC    0x03   // 读目标数据(4字节)
#define     PGM4      0x06   // 写入长字(4字节)
#define     ERSBLK    0x08   // 擦除整块Flash
#define     ERSSCR    0x09   // 擦除Flash扇区
#define     PGMSEC    0x0B   // 写入扇区
#define     RD1ALL    0x40   // 读所有的块
#define     RDONCE    0x41   // 只读一次
#define     PGMONCE   0x43   // 只写一次
#define     ERSALL    0x44   // 擦除所有块
#define     VFYKEY    0x45   // 验证后门访问钥匙
#define     PGMPART   0x80   // 写入分区
#define     SETRAM    0x81   // 设定FlexRAM功能

#elif defined(MK60F15)
#define     FCMD        FTFE_FCCOB0         //FTFL 命令

#define     FADDR2      FTFE_FCCOB1         //Flash address [23:16]
#define     FADDR1      FTFE_FCCOB2         //Flash address [15:8]
#define     FADDR0      FTFE_FCCOB3         //Flash address [7:0]
//Must be 64-bit aligned (Flash address [2:0] = 000)  8字节对齐

#define     FDATA0      FTFE_FCCOB4         //Data Byte 0       //注意一点，4字节排序，FDATA3是最低位，FDATA0是最高位
#define     FDATA1      FTFE_FCCOB5         //Data Byte 1
#define     FDATA2      FTFE_FCCOB6         //Data Byte 2
#define     FDATA3      FTFE_FCCOB7         //Data Byte 3
#define     FDATA4      FTFE_FCCOB8         //Data Byte 4
#define     FDATA5      FTFE_FCCOB9         //Data Byte 5
#define     FDATA6      FTFE_FCCOBA         //Data Byte 6
#define     FDATA7      FTFE_FCCOBB         //Data Byte 7

//FCMD 命令
#define     RD1BLK    0x00   // 读整块Flash
#define     RD1SEC    0x01   // 读整个扇区
#define     PGMCHK    0x02   // 写入检查
#define     RDRSRC    0x03   // 读目标数据(8字节)
#define     PGM8      0x07   // 写入长字(8字节)
#define     ERSBLK    0x08   // 擦除整块Flash
#define     ERSSCR    0x09   // 擦除Flash扇区
#define     PGMSEC    0x0B   // 写入扇区
#define     RD1ALL    0x40   // 读所有的块
#define     RDONCE    0x41   // 只读一次
#define     PGMONCE   0x43   // 只写一次
#define     ERSALL    0x44   // 擦除所有块
#define     VFYKEY    0x45   // 验证后门访问钥匙
#define     PGMPART   0x80   // 写入分区
#define     SETRAM    0x81   // 设定FlexRAM功能

#endif
extern ParValue myPar_num;
void init_Par_from_FLASH();//在主函数调用的初始化
void usart_Par_cmd(uint8 *cmd);//串口接收完字符串后调用，传入接收的字符串
extern int max_speed;
extern int min_speed;

//****内部函数，外边文件可能用不到****
extern void init_myPar_num();
extern void get_debug_Par_from_FLASH(PAR_STRUCT_TYPE_NAME *pv,uint8 num);
extern void save_Save_par();
extern void get_savePar_to_debugPar(uint8 num);
extern float str2float(uint8 *cmd);
extern void get_debug_and_printf_Par(uint8 num);
extern void get_save_and_printf_Par(uint8 num);
extern void save_Bebug_par();
extern void del_save_par(uint8 num);
void cmd_deal();
void evaluate_all(ParValue pv);

#endif
