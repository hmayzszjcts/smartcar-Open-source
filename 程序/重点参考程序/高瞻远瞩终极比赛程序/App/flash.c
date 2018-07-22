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


//包含头文件
#include "flash.h" 
#include "common.h"
#include "key.h"
//#include "gpio.h"




//Flash命令宏定义，内部使用
#define   RD1BLK    0x00   // 读整块Flash
#define   RD1SEC    0x01   // 读整个扇区
#define   PGMCHK    0x02   // 写入检查
#define   RDRSRC    0x03   // 读目标数据
#define   PGM4      0x06   // 写入长字
#define   ERSBLK    0x08   // 擦除整块Flash
#define   ERSSCR    0x09   // 擦除Flash扇区
#define   PGMSEC    0x0B   // 写入扇区
#define   RD1ALL    0x40   // 读所有的块
#define   RDONCE    0x41   // 只读一次
#define   PGMONCE   0x43   // 只写一次
#define   ERSALL    0x44   // 擦除所有块
#define   VFYKEY    0x45   // 验证后门访问钥匙
#define   PGMPART   0x80   // 写入分区
#define   SETRAM    0x81   // 设定FlexRAM功能
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
USHORT16 User_parameter[12]={49,19,27,43,63,1,0x0007,0x0008,0x0009,0x0010,0x0011,0x0012};
//User_parameter[0]:speed_set




//data memories
USHORT16 read_memory0[4]= {0x0000,0x0000,0x0000,0x0000};
USHORT16 read_memory1[4]= {0x0000,0x0000,0x0000,0x0000};
USHORT16 read_memory2[4]= {0x0000,0x0000,0x0000,0x0000};
//gear memories
USHORT16 gear_memory0[4]= {0x0000,0x0000,0x0000,0x0000};
USHORT16 gear_memory1[4]= {0x0000,0x0000,0x0000,0x0000};
USHORT16 gear_memory2[4]= {0x0000,0x0000,0x0000,0x0000};
//person control
//++++++++++++++++++++++++++++++++++++各个速度档位参数++++++++++++++++++++++++++++++++++++++++++++++++++
USHORT16 ZNC_GEAR1_0[4]= {49,19,27,43};//超级大神
USHORT16 ZNC_GEAR1_1[4]= {63,0,0x0000,0x0000};
USHORT16 ZNC_GEAR1_2[4]= {0x0000,0x0000,0x0000,0x0000};

USHORT16 ZNC_GEAR2_0[4]= {49,19,28,43};//大神
USHORT16 ZNC_GEAR2_1[4]= {63,1,0x0000,0x0000};
USHORT16 ZNC_GEAR2_2[4]= {0x0000,0x0000,0x0000,0x0000};

USHORT16 ZNC_GEAR3_0[4]= {45,19,28,48};//保底
USHORT16 ZNC_GEAR3_1[4]= {63,1,0x0000,0x0000};
USHORT16 ZNC_GEAR3_2[4]= {0x0000,0x0000,0x0000,0x0000};

USHORT16 ZNC_GEAR4_0[4]= {40,19,30,48};//悲催
USHORT16 ZNC_GEAR4_1[4]= {63,1,0x0000,0x0000};
USHORT16 ZNC_GEAR4_2[4]= {0x0000,0x0000,0x0000,0x0000};

USHORT16 ZNC_GEAR5_0[4]= {39,19,30,48};//补赛速度
USHORT16 ZNC_GEAR5_1[4]= {63,1,0x0000,0x0000};
USHORT16 ZNC_GEAR5_2[4]= {0x0000,0x0000,0x0000,0x0000};
//USHORT16 dangwei3_neicun[4]= {0x0000,0x0000,0x0000,0x0000};
int speed_level;
//=================内部函数实现=============================================

//==========================================================================
//函数名称：flash_cmd_launch
//函数返回：0-成功 1-失败
//参数说明：无
//功能概要：启动Flash命令
//==========================================================================
__RAMFUN static uint32 flash_cmd_launch(void)
{
    
    FTFL_FSTAT = 	(0
				  	|	FTFL_FSTAT_CCIF_MASK 		// 启动命令
					|	FTFL_FSTAT_ACCERR_MASK 		// 清除访问错误标志位
					| 	FTFL_FSTAT_FPVIOL_MASK		// 非法访问标志位
					);	
    									
    while(!(FTFL_FSTAT & FTFL_FSTAT_CCIF_MASK));	// 等待命令完成

    // 检查错误标志
    if( FTFL_FSTAT & (FTFL_FSTAT_ACCERR_MASK | FTFL_FSTAT_FPVIOL_MASK | FTFL_FSTAT_MGSTAT0_MASK))
        return 1 ; 				//执行命令出错
  
    return 0; 					//执行命令成功
}

//===========================================================================

//=================外部接口函数==============================================
//==========================================================================
//函数名称：flash_init
//函数返回：无
//参数说明：无
//功能概要：初始化flash模块
//==========================================================================
__RAMFUN void flash_init(void)
{
	// 清除Flash预读取缓冲区
    FMC_PFB0CR |= FMC_PFB0CR_S_B_INV_MASK;
    FMC_PFB1CR |= FMC_PFB0CR_S_B_INV_MASK;
    
    while(!(FTFL_FSTAT & FTFL_FSTAT_CCIF_MASK));	// 等待命令完成
    
	FTFL_FSTAT = 	(0
					|	FTFL_FSTAT_ACCERR_MASK 		// 清除访问错误标志位
					| 	FTFL_FSTAT_FPVIOL_MASK		// 非法访问标志位
					);	
}

//==========================================================================
//函数名称：flash_erase_sector
//函数返回：函数执行执行状态：0=正常；非0=异常。
//参数说明：sectorNo：扇区号（K60N512实际使用0~255）
//功能概要：擦除指定flash扇区
//==========================================================================
__RAMFUN uint8 flash_erase_sector(uint16 sectorNo)
{
    Dtype	addr;	
	addr.DW = (uint32)(sectorNo<<11);	//每个扇区2KB，所以扇区地址=扇区号*2K ，这里用 <<11 表示 *2*1024 
    
    //dest.word    = (uint32)(sectorNo<<11);	//每个扇区2KB，所以扇区地址=扇区号*2K ，这里用 <<11 表示 *2*1024 

    // 设置擦除命令
    FTFL_FCCOB0 = ERSSCR; // 擦除扇区命令
    
    // 设置目标地址
    FTFL_FCCOB1 = addr.B[2];
    FTFL_FCCOB2 = addr.B[1];
    FTFL_FCCOB3 = addr.B[0];
    
    // 执行命令序列
    if(1 == flash_cmd_launch())    //若执行命令出现错误
        return 1;     //擦除命令错误
   
    // 若擦除sector0时，则解锁设备
    if(sectorNo ==0)
    {
        // 写入4字节
        FTFL_FCCOB0 = PGM4; 
        // 设置目标地址
        FTFL_FCCOB1 = 0x00;
        FTFL_FCCOB2 = 0x04;
        FTFL_FCCOB3 = 0x0C;
        // 数据
        FTFL_FCCOB4 = 0xFF;
        FTFL_FCCOB5 = 0xFF;
        FTFL_FCCOB6 = 0xFF;
        FTFL_FCCOB7 = 0xFE;
        // 执行命令序列
        if(1 == flash_cmd_launch())  //若执行命令出现错误
            return 2;   //解锁命令错误
    }  
    
    return 0;  //成功返回
}

//==========================================================================
//函数名称：flash_write
//函数返回：函数执行状态：0=正常；非0=异常。
//参数说明：sectorNo：目标扇区号 （K60N512实际使用0~255）
//         offset:写入扇区内部偏移地址（0~2043）
//         cnt：写入字节数目（0~2043）
//         buf：源数据缓冲区首地址
//功能概要：flash写入操作
//==========================================================================
__RAMFUN uint8 flash_write(uint16 sectorNo,uint16 offset,uint32 data) 
{
	Dtype	addr;	
    Dtype	Data;
	
	ASSERT(offset%4 == 0);			//偏移量必须为4的倍数
									//此处提示警告，但是安全的……
	ASSERT(offset <= 0x800);		//扇区大小为2K，即偏移量必须不大于 0x800
									//此处提示警告，但是安全的……
	
    addr.DW = (uint32)((sectorNo<<11) + offset);		//计算地址
	Data.DW	= data;	
	    
    FTFL_FCCOB0 = PGM4;				// 设置写入命令

	FTFL_FCCOB1 = addr.B[2];		// 设置目标地址
	FTFL_FCCOB2 = addr.B[1];
	FTFL_FCCOB3 = addr.B[0];


	FTFL_FCCOB4 = Data.B[3];        // 设置写入数据
	FTFL_FCCOB5 = Data.B[2];
	FTFL_FCCOB6 = Data.B[1];
	FTFL_FCCOB7 = Data.B[0];
	
	if(1 == flash_cmd_launch()) 	return 2;  //写入命令错误

    return 0;  //成功执行
}


//==========================================================================
//函数名称：flash_write
//函数返回：函数执行状态：0=正常；非0=异常。
//参数说明：sectorNo：目标扇区号 （K60N512实际使用0~255）
//         offset:写入扇区内部偏移地址（0~2043）
//         cnt：写入字节数目（0~2043）
//         buf：源数据缓冲区首地址
//功能概要：flash写入操作
//==========================================================================
__RAMFUN uint8 flash_write_buf(uint16 sectorNo,uint16 offset,uint16 cnt,uint8 buf[])//扇区号，扇区偏移地址 i*4  
{
    uint32 size;
 
	Dtype	addr;	
    
	ASSERT(offset%4 == 0);			//偏移量必须为4的倍数
									//此处提示警告，但是安全的……
	ASSERT(offset <= 0x800);		//扇区大小为2K，即偏移量必须不大于 0x800
									//此处提示警告，但是安全的……
	
    addr.DW = (uint32)((sectorNo<<11) + offset);		//计算地址
	
    // 设置写入命令
    FTFL_FCCOB0 = PGM4;

    for(size=0; size<cnt; size+=4, addr.DW+=4, buf+=4)
    {
        
        FTFL_FCCOB1 = addr.B[2];						// 设置目标地址
        FTFL_FCCOB2 = addr.B[1];
        FTFL_FCCOB3 = addr.B[0];
 
        FTFL_FCCOB4 = buf[3];							// 拷贝数据
        FTFL_FCCOB5 = buf[2];
        FTFL_FCCOB6 = buf[1];
        FTFL_FCCOB7 = buf[0];
        
        if(1 == flash_cmd_launch()) 
            return 2;  									//写入命令错误
    }
    
    return 0;  //成功执行
}






__RAMFUN uint8 Flash_read(uint16 sectNo,uint16 offset,uint16 cnt,uint8*bBuf)
{
    uint32 wAddr = 0;
    wAddr = sectNo<<11 + offset;
    while (cnt--)
        *bBuf++=*(uint8*)wAddr++;
   return TRUE;
}


void flash_erase_memories(void)
  {
   flash_erase_sector(MEMORIES_ADDRESS0);       
   flash_erase_sector(MEMORIES_ADDRESS1);       
   flash_erase_sector(MEMORIES_ADDRESS2);       
  }
  void flash_write_memories(void)
 {
   int flash_i;
   for(flash_i=0;flash_i<4;flash_i++)
  {
     flash_write(MEMORIES_ADDRESS0,flash_i*4,gear_memory0[flash_i]);  
     flash_write(MEMORIES_ADDRESS1,flash_i*4,gear_memory1[flash_i]); 
     flash_write(MEMORIES_ADDRESS2,flash_i*4,gear_memory2[flash_i]); 
   }
 }
void flash_read_memories(void)
{
   int flash_i;
   for(flash_i=0;flash_i<4;flash_i++)
  {
     read_memory0[flash_i]=flash_read(MEMORIES_ADDRESS0,flash_i*4,int16);
     read_memory1[flash_i]=flash_read(MEMORIES_ADDRESS1,flash_i*4,int16);
     read_memory2[flash_i]=flash_read(MEMORIES_ADDRESS2,flash_i*4,int16);
  }
}
void read_parameter(void)
{
                    User_parameter[0]=read_memory0[0];
                    User_parameter[1]=read_memory0[1];
                    User_parameter[2]=read_memory0[2];
                    User_parameter[3]=read_memory0[3];
                    User_parameter[4]=read_memory1[0];
                    User_parameter[5]=read_memory1[1];
                    User_parameter[6]=read_memory1[2];
                    User_parameter[7]=read_memory1[3];
                    User_parameter[8]=read_memory2[0];
                    User_parameter[9]=read_memory2[1];
                    User_parameter[10]=read_memory2[2];
                    User_parameter[11]=read_memory2[3];
}
void write_parameter(void)
{
                    gear_memory0[0]=User_parameter[0];
                    gear_memory0[1]=User_parameter[1];
                    gear_memory0[2]=User_parameter[2];
                    gear_memory0[3]=User_parameter[3];
                    gear_memory1[0]=User_parameter[4];
                    gear_memory1[1]=User_parameter[5];
                    gear_memory1[2]=User_parameter[6];
                    gear_memory1[3]=User_parameter[7];
                    gear_memory2[0]=User_parameter[8];
                    gear_memory2[1]=User_parameter[9];
                    gear_memory2[2]=User_parameter[10];
                    gear_memory2[3]=User_parameter[11];
}
void write_speed_gear1(void)
{
int flash_i;
for(flash_i=0;flash_i<4;flash_i++)
      {
       gear_memory0[flash_i]=ZNC_GEAR1_0[flash_i];
       gear_memory1[flash_i]=ZNC_GEAR1_1[flash_i];
       gear_memory2[flash_i]=ZNC_GEAR1_2[flash_i];
       }
}
void write_speed_gear2(void)
{
int flash_i;
for(flash_i=0;flash_i<4;flash_i++)
      {
       gear_memory0[flash_i]=ZNC_GEAR2_0[flash_i];
       gear_memory1[flash_i]=ZNC_GEAR2_1[flash_i];
       gear_memory2[flash_i]=ZNC_GEAR2_2[flash_i];
       }
} 
void write_speed_gear3(void)
{
int flash_i;
for(flash_i=0;flash_i<4;flash_i++)
      {
       gear_memory0[flash_i]=ZNC_GEAR3_0[flash_i];
       gear_memory1[flash_i]=ZNC_GEAR3_1[flash_i];
       gear_memory2[flash_i]=ZNC_GEAR3_2[flash_i];
       }
} 
void write_speed_gear4(void)
{
int flash_i;
for(flash_i=0;flash_i<4;flash_i++)
      {
       gear_memory0[flash_i]=ZNC_GEAR4_0[flash_i];
       gear_memory1[flash_i]=ZNC_GEAR4_1[flash_i];
       gear_memory2[flash_i]=ZNC_GEAR4_2[flash_i];
       }
} 
void write_speed_gear5(void)
{
int flash_i;
for(flash_i=0;flash_i<4;flash_i++)
      {
       gear_memory0[flash_i]=ZNC_GEAR5_0[flash_i];
       gear_memory1[flash_i]=ZNC_GEAR5_1[flash_i];
       gear_memory2[flash_i]=ZNC_GEAR5_2[flash_i];
       }
}                     
                    
                    


  
void ZNC_DATA_Init(void)
{int flash_i;
  //OLED_flash_memories();
  OLED_starting_car();
  systick_delay_ms(500);
  
  keyright_=keyRight;
  if(!keyright_) //down  key3
             {
                systick_delay_ms(10);
                keyright_=keyRight;
                if(!keyright_)
                {   keyright_=keyRight; 
                    OLED_flash_memories();
                    systick_delay_ms(500);
                    
                    //默认档位第二档
                    /*for(flash_i=0;flash_i<4;flash_i++)
                    {
                      gear_memory0[flash_i]=ZNC_GEAR2_0[flash_i];
                      gear_memory1[flash_i]=ZNC_GEAR2_1[flash_i];
                      gear_memory2[flash_i]=ZNC_GEAR2_2[flash_i];
                    }*/
                    switch(speed_level)
                    {
                      case 1:write_speed_gear1();break;
                      case 2:write_speed_gear2();break;
                      case 3:write_speed_gear3();break;
                      case 4:write_speed_gear4();break;
                      case 5:write_speed_gear5();break;
                    }
                    flash_erase_memories();
                    flash_write_memories();
                    flash_read_memories();
                    read_parameter();
                }
                OLED_flash_memories_queren();
                systick_delay_ms(500);
             }
  else
            {
                flash_read_memories();
                read_parameter();
             }
                OLED_start_finish();
                systick_delay_ms(500);
}

void ZNC_DATA_Update(void)
{
  write_parameter();
  flash_erase_memories();
  flash_write_memories();
  flash_read_memories();
  read_parameter();
}

//==========================================================================
