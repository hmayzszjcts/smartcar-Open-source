#ifndef __LDPY_DMA_H__
#define __LDPY_DMA_H__

#include "MK60DZ10.h"
#include "LDPY_DMA.h"
#include "LDPY_EXTI.h"
#include "LDPY_TypeChange.h"
#include "myStruct.h"

/*设置数据类型*/
typedef 	unsigned 	char		u8;	  //无符号型
typedef 	unsigned 	short int	u16;
typedef 	unsigned 	long  int	u32;

typedef 	char					s8;	  //有符号型
typedef 	short int				s16;
typedef 	long  int				s32;

//u8 periodic_trigg=FALSE;//是否使能周期触发

#define UART5_D_BASE  (UART5_BASE+0x07)


/*定义结构体*/
typedef struct
{
	uint8_t  DMA_CHx;//通道编号
    uint8_t  DMA_Req;	//选择DMA通道对应的请求源，外设DMA请求号
	uint8_t DMA_PeriodicTriggerEnable;//是否使能周期触发
	uint8_t  DMA_Isr;//DMA中断
	uint32_t DMA_MajorLoopCnt;//主循环计数，15位的无符号整形(0~0x7FFF)
	uint32_t DMA_MinorByteCnt;//一次循环传输字节数
	
	uint32_t DMA_SourceAddr;//配置源数据地址
	uint32_t DMA_SourceDataSize;//配置源数据大小
	int32_t DMA_SourceAddrOffset;//配置源数据地址偏移，不必须配置，默认为零
	int32_t DMA_LastSourceAddrAdj;//配置源数据地址在主循环完成后的调整量，默认为零，不必须配置
	
	uint32_t DMA_DestAddr;//配置目的数据地址
	uint8_t DMA_DestDataSize;
	int32_t DMA_DestAddrOffset;
	int32_t DMA_LastDestAddrAdj;
	
    uint8_t DMA_AutoDisableReq;//使能自动禁用请求，使能后通道请求将在主循环完成后禁用
	uint8_t DMA_MajorCompleteIntEnable;//使能主循环完成中断
 	uint8_t DMA_MajorHalfCompleteIntEnable;//使能主循环完成到一半中断

	uint32_t DMA_BufferSize;//缓冲区大小
    uint32_t DMA_Mode;//DMA模式：一次或循环
  //uint32_t DMA_Priority;//优先级
	uint8_t DMA_AutoClose;//当主循环计数器减少到零的时候，是否关闭DMA
	uint8_t DMA_SourceModulo;
	uint8_t DMA_Complete_Flag;
}DMA_InitTypeDef;


/*定义DMA的通道号*/
#define  DMA_CH0 0
#define  DMA_CH1 1
#define  DMA_CH2 2
#define  DMA_CH3 3
#define  DMA_CH4 4
#define  DMA_CH5 5
#define  DMA_CH6 6
#define  DMA_CH7 7
#define  DMA_CH8 8
#define  DMA_CH9 9
#define  DMA_CH10 10
#define  DMA_CH11 11
#define  DMA_CH12 12
#define  DMA_CH13 13
#define  DMA_CH14 14
#define  DMA_CH15 15

/*DMA外设的请求号*/
#define UART0_REV_DMAREQ  2
#define UART0_TRAN_DMAREQ 3
#define UART1_REV_DMAREQ  4
#define UART1_TRAN_DMAREQ 5
#define UART2_REV_DMAREQ  6
#define UART2_TRAN_DMAREQ 7
#define UART3_REV_DMAREQ  8
#define UART3_TRAN_DMAREQ 9
#define UART4_REV_DMAREQ  10
#define UART4_TRAN_DMAREQ 11
#define UART5_REV_DMAREQ  12
#define UART5_TRAN_DMAREQ 13
#define I2S0_REV_DMAREQ   14
#define I2S0_TRAN_DMAREQ  15
#define SPI0_REV_DMAREQ   16
#define SPI0_TRAN_DMAREQ  17
#define SPI1_REV_DMAREQ   18
#define SPI1_TRAN_DMAREQ  19
#define I2C0_DMAREQ       22
#define I2C1_DMAREQ       23
#define FTM0_CH0_DMAREQ   24
#define FTM0_CH1_DMAREQ   25
#define FTM0_CH2_DMAREQ   26
#define FTM0_CH3_DMAREQ   27
#define FTM0_CH4_DMAREQ   28
#define FTM0_CH5_DMAREQ   29
#define FTM0_CH6_DMAREQ   30
#define FTM0_CH7_DMAREQ   31
#define FTM1_CH0_DMAREQ   32
#define FTM1_CH1_DMAREQ   33
#define FTM2_CH0_DMAREQ   34
#define FTM2_CH1_DMAREQ   35
#define FTM3_CH0_DMAREQ   36
#define FTM3_CH1_DMAREQ   37
#define FTM3_CH2_DMAREQ   38
#define FTM1_CH3_DMAREQ   39  
#define ADC0_DMAREQ       40
#define ADC1_DMAREQ       41
#define CMP0_DMAREQ       42
#define CMP1_DMAREQ       43
#define CMP2_DMAREQ       44
#define DAC0_DMAREQ       45
#define DAC1_DMAREQ       46
#define CMT_DMAREQ        47
#define PDB_DMAREQ        48
#define PORTA_DMAREQ      49
#define PORTB_DMAREQ      50
#define PORTC_DMAREQ      51
#define PORTD_DMAREQ      52
#define PORTE_DMAREQ      53
#define FTM3_CH4_DMAREQ   54
#define FTM3_CH5_DMAREQ   55
#define FTM3_CH6_DMAREQ   56
#define FTM3_CH7_DMAREQ   57
#define DMA_MUX_58        58
#define DMA_MUX_59        59
#define DMA_MUX_60        60
#define DMA_MUX_61        61
#define DMA_MUX_62        62
#define DMA_MUX_63        63

/********************************************************************************
**定义数据传输的长度
**SRC，DST分别为源地址和目的地址
********************************************************************************/
#define DMA_SRC_8BIT    0
#define DMA_SRC_16BIT   1
#define DMA_SRC_32BIT   2
#define DMA_SRC_16BYTE  4
#define DMA_DST_8BIT    0
#define DMA_DST_16BIT   1
#define DMA_DST_32BIT   2
#define DMA_DST_16BYTE  4


#define DMA_DIR_PeripheralDST              ((uint32_t)0x00000010)
#define DMA_DIR_PeripheralSRC              ((uint32_t)0x00000000)
#define IS_DMA_DIR(DIR) (((DIR) == DMA_DIR_PeripheralDST) || \
                         ((DIR) == DMA_DIR_PeripheralSRC))
												 
#define DMA_PeripheralInc_Enable           ((uint32_t)0x00000001)
#define DMA_PeripheralInc_Disable          ((uint32_t)0x00000000)
#define IS_DMA_PERIPHERAL_INC_STATE(STATE) (((STATE) == DMA_PeripheralInc_Enable) || \
                                            ((STATE) == DMA_PeripheralInc_Disable))												 

#define DMA_MemoryInc_Enable               ((uint32_t)0x00000001)
#define DMA_MemoryInc_Disable              ((uint32_t)0x00000000)
#define IS_DMA_MEMORY_INC_STATE(STATE) (((STATE) == DMA_MemoryInc_Enable) || \
                                        ((STATE) == DMA_MemoryInc_Disable))
																				
#define DMA_PeripheralDataSize_Byte        ((uint32_t)0x00000000)
#define DMA_PeripheralDataSize_HalfWord    ((uint32_t)0x00000100)
#define DMA_PeripheralDataSize_Word        ((uint32_t)0x00000200)
#define IS_DMA_PERIPHERAL_DATA_SIZE(SIZE) (((SIZE) == DMA_PeripheralDataSize_Byte) || \
                                           ((SIZE) == DMA_PeripheralDataSize_HalfWord) || \
                                           ((SIZE) == DMA_PeripheralDataSize_Word))																				

#define DMA_MemoryDataSize_Byte            ((uint32_t)0x00000000)
#define DMA_MemoryDataSize_HalfWord        ((uint32_t)0x00000400)
#define DMA_MemoryDataSize_Word            ((uint32_t)0x00000800)
#define IS_DMA_MEMORY_DATA_SIZE(SIZE) (((SIZE) == DMA_MemoryDataSize_Byte) || \
                                       ((SIZE) == DMA_MemoryDataSize_HalfWord) || \
                                       ((SIZE) == DMA_MemoryDataSize_Word))
#define DMA_Mode_Circular                  ((uint32_t)0x00000020)
#define DMA_Mode_Normal                    ((uint32_t)0x00000000)
#define IS_DMA_MODE(MODE) (((MODE) == DMA_Mode_Circular) || ((MODE) == DMA_Mode_Normal))

#define DMA_M2M_Enable                     ((uint32_t)0x00004000)
#define DMA_M2M_Disable                    ((uint32_t)0x00000000)
#define IS_DMA_M2M_STATE(STATE) (((STATE) == DMA_M2M_Enable) || ((STATE) == DMA_M2M_Disable))


//使能通道X的DMA请求
#define LPLD_DMA_EnableReq(CHX)  (DMA0->ERQ|=(1<<CHX))
//禁用通道X的DMA请求
#define LPLD_DMA_DisableReq(CHX) (DMA0->ERQ&=~(1<<CHX))
//加载源地址
#define LPLD_DMA_LoadSrcAddr(CHX, ADDR)   (DMA0->TCD[CHX].SADDR=DMA_SADDR_SADDR(ADDR))
//加载目的地址
#define LPLD_DMA_LoadDstAddr(CHX, ADDR)   (DMA0->TCD[CHX].DADDR=DMA_DADDR_DADDR(ADDR))

void LDPY_DMA_StructInit(DMA_InitTypeDef  *dma_init_struct,CCD_Info *CCD_info,DMA_Required_Variable *DMA_Required_Variable);
uint8_t LDPY_DMA_init(DMA_InitTypeDef *dma_init_struct);
void LDPY_DMA_SoftwareStartService(DMA_InitTypeDef);
uint8_t LDPY_DMA_EnableIrq(DMA_InitTypeDef);
uint8_t LDPY_DMA_DisableIrq(DMA_InitTypeDef);
uint8_t FrameHeader_Config(DMA_Required_Variable *DMA_Variable_1);
uint8_t LDPY_DMA_InitAndChoice(DMA_InitTypeDef  *dma_init_struct,DMA_Required_Variable *DMA_Required_Variable,\
                          CCD_Info *CCD_info);
void LDPY_DMA_UpdateFifo_PhotoValue(CCD_Info *CCD1_info,CCD_Info *CCD2_info,CCD_Info *CCD3_info);
void LDPY_DMA_UpdateFifo_Parameters(CCD_Info *CCD1_info,Steer_Info *Steer_info,\
									Motor_Info *Motor_info,Speed_Info *Speed_info,\
									Road_Condition *Road_condition);

#ifdef DMA_CH2_SendLabVIEW_Info_Enable

void LDPY_DMA_SendTempVar(	LabVIEW_Info *LabVIEW_info,
							DMA_Required_Variable *DMA_Variable,
							CCD_Info *CCD1_info,
							CCD_Info *CCD2_info,
							CCD_Info *CCD3_info,
							HeadSteer_Info *HeadSteer_info,
							Steer_Info *Steer_info,
							Motor_Info *Motor_info,
							Speed_Info *Speed_info
						 );
#endif

#ifdef DMA_CH0_Enable//发送3个CCD的原始图像和偏差

void LDPY_DMA_SendThreeCCDsImage(DMA_Required_Variable *DMA_Variable,CCD_Info *CCD1_info,CCD_Info *CCD2_info,CCD_Info *CCD3_info);

#endif

#endif
