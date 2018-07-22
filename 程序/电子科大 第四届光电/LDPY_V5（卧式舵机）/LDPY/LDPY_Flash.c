
#include "LDPY_Flash.h"

/*************************************************************************
*				零	度	偏	移                             
*
*  函数名称:LDPY_FlashInit
*  功能说明:Flash模块 初始化
*  参数说明:
*  函数返回:
*  修改时间:2014-05-12
*  备    注: 
*************************************************************************/	
void LDPY_Flash_Init(void)
{
	  //检查Flash访问错误
	  if (FTFL->FSTAT & FTFL_FSTAT_ACCERR_MASK)
	  {
	    FTFL->FSTAT |= FTFL_FSTAT_ACCERR_MASK;       //清除错误标志
	  }
	  //检查保护错误
	  else if (FTFL->FSTAT & FTFL_FSTAT_FPVIOL_MASK)
	  {
	    FTFL->FSTAT |= FTFL_FSTAT_FPVIOL_MASK;
	  }
	  //检查读冲突错误
	  else if (FTFL->FSTAT & FTFL_FSTAT_RDCOLERR_MASK)
	  {
	    FTFL->FSTAT |= FTFL_FSTAT_RDCOLERR_MASK;
	  }
	  
	  //禁用Flash模块的数据缓存
	  FMC->PFB0CR &= ~FMC_PFB0CR_B0DCE_MASK;
	  FMC->PFB1CR &= ~FMC_PFB1CR_B1DCE_MASK;
} 

/*************************************************************************
*				零	度	偏	移                             
*
*  函数名称:LDPY_Flash_SectorErase
*  功能说明:擦除Flash扇区
*  参数说明:FlashPtr--扇区地址指针，即扇区号乘以2048
*  函数返回:错误代码
*  修改时间:2014-05-14
*  备    注: 
*************************************************************************/
uint8 LDPY_Flash_SectorErase(uint32 FlashPtr)
{
  uint8 Return = FLASH_OK;
  
  //等待CCIF置1
  while (!(FTFL->FSTAT & FTFL_FSTAT_CCIF_MASK)){};
  //写入命令道FCCOB寄存器
  FTFL->FCCOB0 = FLASH_CMD_ERSSCR;
  FTFL->FCCOB1 = (uint8) (FlashPtr >> 16);
  FTFL->FCCOB2 = (uint8)((FlashPtr >> 8 ) & 0xFF);
  FTFL->FCCOB3 = (uint8)( FlashPtr & 0xFF);
  
  //执行命令
  FTFL->FSTAT |= FTFL_FSTAT_CCIF_MASK;    
  //等待命令完成
  while (!(FTFL->FSTAT & FTFL_FSTAT_CCIF_MASK)) {};
  
  //检查Flash访问错误
  if (FTFL->FSTAT & FTFL_FSTAT_ACCERR_MASK)
  {
    FTFL->FSTAT |= FTFL_FSTAT_ACCERR_MASK;       //清除错误标志
    Return |= FLASH_FACCERR;                    //更新返回值
  }
  //检查Flash保护标志
  else if (FTFL->FSTAT & FTFL_FSTAT_FPVIOL_MASK)
  {
    FTFL->FSTAT |= FTFL_FSTAT_FPVIOL_MASK;
    Return |= FLASH_FPVIOL;
  }
  //检查Flash读冲突错误
  else if (FTFL->FSTAT & FTFL_FSTAT_RDCOLERR_MASK)
  {
    FTFL->FSTAT |= FTFL_FSTAT_RDCOLERR_MASK;
    Return |= FLASH_RDCOLERR;
  }
  //检查MGSTAT0非可纠正错误
  else if (FTFL->FSTAT & FTFL_FSTAT_MGSTAT0_MASK)
  {
    Return |= FLASH_MGSTAT0;
  } 
  
  return  Return;
}

/*************************************************************************
*				零	度	偏	移                             
*
*  函数名称:LPLD_Flash_ByteProgram
*  功能说明:编程Flash
*  参数说明:	FlashStartAdd--Flash编程起始地址
*			*DataSrcPtr--数据源指针
*			NumberOfBytes--数据字节长度
*  函数返回:错误代码
*  修改时间:2014-05-14
*  备    注: 
*************************************************************************/
uint8 LDPY_Flash_ByteProgram(uint32 FlashStartAdd, uint32 *DataSrcPtr, uint32 NumberOfBytes)
{
  uint8_t Return = FLASH_OK;
  
  uint32_t size_buffer;
  
  if (NumberOfBytes == 0)
  {
    return FLASH_CONTENTERR;
  }
  else
  {
    size_buffer = (NumberOfBytes - 1)/4 + 1;
  }
  //等待直到CCIF设置
  while (!(FTFL->FSTAT & FTFL_FSTAT_CCIF_MASK)){};

  while ((size_buffer) && (Return == FLASH_OK))
  {
    //写FLASH命令到FCCOB
    FTFL->FCCOB0 = FLASH_CMD_PGM4;
    FTFL->FCCOB1 = (uint8_t)(FlashStartAdd >> 16);
    FTFL->FCCOB2 = (uint8_t)((FlashStartAdd >> 8) & 0xFF);
    FTFL->FCCOB3 = (uint8_t)(FlashStartAdd & 0xFF);
    //如果K60设置成为小端格式，进行如下操作
    FTFL->FCCOB4 = (uint8_t)(*((uint8_t*)DataSrcPtr+3));
    FTFL->FCCOB5 = (uint8_t)(*((uint8_t*)DataSrcPtr+2));
    FTFL->FCCOB6 = (uint8_t)(*((uint8_t*)DataSrcPtr+1));
    FTFL->FCCOB7 = (uint8_t)(*((uint8_t*)DataSrcPtr+0));
    //如果K60设置成为大端格式，进行如下操作
    //FTFL_FCCOB4 = (uint8_t)(*((uint_8*)DataSrcPtr+0));
    //FTFL_FCCOB5 = (uint8_t)(*((uint_8*)DataSrcPtr+1));
    //FTFL_FCCOB6 = (uint8_t)(*((uint_8*)DataSrcPtr+2));
    //FTFL_FCCOB7 = (uint8_t)(*((uint_8*)DataSrcPtr+3));
    //设置FLASH加载命令
    FTFL->FSTAT |= FTFL_FSTAT_CCIF_MASK;    
    //等待命令完成
    while (!(FTFL->FSTAT & FTFL_FSTAT_CCIF_MASK)) {};
    
    //检测FLASH访问错误
    if (FTFL->FSTAT & FTFL_FSTAT_ACCERR_MASK)
    {
        
        FTFL->FSTAT |= FTFL_FSTAT_ACCERR_MASK;
    
        Return |= FLASH_FACCERR;
    }
    //检测FLASH保护错误
    else if (FTFL->FSTAT & FTFL_FSTAT_FPVIOL_MASK)
    {
        FTFL->FSTAT |= FTFL_FSTAT_FPVIOL_MASK;
    
        Return |= FLASH_FPVIOL;
    }
    
    else if (FTFL->FSTAT & FTFL_FSTAT_RDCOLERR_MASK)
    {
        FTFL->FSTAT |= FTFL_FSTAT_RDCOLERR_MASK;
    
        Return |= FLASH_RDCOLERR;
    }
    else if (FTFL->FSTAT & FTFL_FSTAT_MGSTAT0_MASK)
    {
        Return |= FLASH_MGSTAT0;
    } 
    size_buffer --;
    (uint32_t*)DataSrcPtr++;
    FlashStartAdd +=4;
  } 
  return  Return;
} 

/*-----------------------end of LDPY_Flash.h--------------------------*/
