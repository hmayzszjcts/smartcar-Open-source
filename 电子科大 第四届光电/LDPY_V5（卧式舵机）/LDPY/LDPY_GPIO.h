
#ifndef	__LDPY_GPIO_H__
#define	__LDPY_GPIO_H__

#include "MK60DZ10.h"

/************************位带操作**************************************
			
	位带操作以后就可以直接使用PAout(n)=1 或者 0  对单个管脚操作
			
***********************************************************************/
#define BITBAND(addr ,bitnum) ((addr & 0xF0000000)+0x2000000+((addr&0xFFFFF)<<5)+(bitnum<<2))  

#define MEM_ADDR(addr)   *((volatile unsigned long   *)(addr))

#define BIT_ADDR(addr,bitnum) MEM_ADDR(BITBAND(addr,bitnum)) 

//IO地址映射
#define GPIOA_ODR_Addr (PTA_BASE) //0x400FF000
#define GPIOB_ODR_Addr (PTB_BASE) //0x40010C0C 
#define GPIOC_ODR_Addr (PTC_BASE) //0x4001100C 
#define GPIOD_ODR_Addr (PTD_BASE) //0x4001140C 
#define GPIOE_ODR_Addr (PTE_BASE) //0x4001180C 

#define GPIOA_IDR_Addr (PTA_BASE+16) //0x40010808 
#define GPIOB_IDR_Addr (PTB_BASE+16) //0x40010C08 
#define GPIOC_IDR_Addr (PTC_BASE+16) //0x40011008 
#define GPIOD_IDR_Addr (PTD_BASE+16) //0x40011408 
#define GPIOE_IDR_Addr (PTE_BASE+16) //0x40011808

#define PAout(n) BIT_ADDR(GPIOA_ODR_Addr,n) //??
#define PAin(n)  BIT_ADDR(GPIOA_IDR_Addr,n) //??

#define PBout(n) BIT_ADDR(GPIOB_ODR_Addr,n) //??
#define PBin(n)  BIT_ADDR(GPIOB_IDR_Addr,n) //??

#define PCout(n) BIT_ADDR(GPIOC_ODR_Addr,n) //??
#define PCin(n)  BIT_ADDR(GPIOC_IDR_Addr,n) //??

#define PDout(n) BIT_ADDR(GPIOD_ODR_Addr,n) //??
#define PDin(n)  BIT_ADDR(GPIOD_IDR_Addr,n) //??

#define PEout(n) BIT_ADDR(GPIOE_ODR_Addr,n) //??
#define PEin(n)  BIT_ADDR(GPIOE_IDR_Addr,n) //??
/***************************************************************************/	

/***************************************************************************/	
	/** @defgroup GPIO_pins_define 
  * @{
  */

#define GPIO_Pin_0                 ((uint16_t)0x0000)  /*!< Pin 0 selected */
#define GPIO_Pin_1                 ((uint16_t)0x0001)  /*!< Pin 1 selected */
#define GPIO_Pin_2                 ((uint16_t)0x0002)  /*!< Pin 2 selected */
#define GPIO_Pin_3                 ((uint16_t)0x0003)  /*!< Pin 3 selected */
#define GPIO_Pin_4                 ((uint16_t)0x0004)  /*!< Pin 4 selected */
#define GPIO_Pin_5                 ((uint16_t)0x0005)  /*!< Pin 5 selected */
#define GPIO_Pin_6                 ((uint16_t)0x0006)  /*!< Pin 6 selected */
#define GPIO_Pin_7                 ((uint16_t)0x0007)  /*!< Pin 7 selected */
#define GPIO_Pin_8                 ((uint16_t)0x0008)  /*!< Pin 8 selected */
#define GPIO_Pin_9                 ((uint16_t)0x0009)  /*!< Pin 9 selected */
#define GPIO_Pin_10                ((uint16_t)0x000A)  /*!< Pin 10 selected */
#define GPIO_Pin_11                ((uint16_t)0x000B)  /*!< Pin 11 selected */
#define GPIO_Pin_12                ((uint16_t)0x000C)  /*!< Pin 12 selected */
#define GPIO_Pin_13                ((uint16_t)0x000D)  /*!< Pin 13 selected */
#define GPIO_Pin_14                ((uint16_t)0x000E)  /*!< Pin 14 selected */
#define GPIO_Pin_15                ((uint16_t)0x000F)  /*!< Pin 15 selected */

#define GPIO_Pin_16                ((uint16_t)0x0010)  /*!< Pin 16 selected */
#define GPIO_Pin_17                ((uint16_t)0x0011)  /*!< Pin 17 selected */
#define GPIO_Pin_18                ((uint16_t)0x0012)  /*!< Pin 18 selected */
#define GPIO_Pin_19                ((uint16_t)0x0013)  /*!< Pin 19 selected */
#define GPIO_Pin_20                ((uint16_t)0x0014)  /*!< Pin 20 selected */
#define GPIO_Pin_21                ((uint16_t)0x0015)  /*!< Pin 21 selected */
#define GPIO_Pin_22                ((uint16_t)0x0016)  /*!< Pin 22 selected */
#define GPIO_Pin_23                ((uint16_t)0x0017)  /*!< Pin 23 selected */
#define GPIO_Pin_24                ((uint16_t)0x0018)  /*!< Pin 24 selected */
#define GPIO_Pin_25                ((uint16_t)0x0019)  /*!< Pin 25 selected */
#define GPIO_Pin_26                ((uint16_t)0x001A)  /*!< Pin 26 selected */
#define GPIO_Pin_27                ((uint16_t)0x001B)  /*!< Pin 27 selected */
#define GPIO_Pin_28                ((uint16_t)0x001C)  /*!< Pin 28 selected */
#define GPIO_Pin_29                ((uint16_t)0x001D)  /*!< Pin 29 selected */
#define GPIO_Pin_30                ((uint16_t)0x001E)  /*!< Pin 30 selected */
#define GPIO_Pin_31                ((uint16_t)0x001F)  /*!< Pin 31 selected */

//#define GPIO_Pin_All               ((uint16_t)0xFFFF)  /*!< All pins selected */


/** 
* @brief  Configuration Mode enumeration  
*/
typedef enum
{ 
	GPIO_Mode_AIN = 0x0000,							//模拟
	
	GPIO_Mode_PD = 0x0102,							//GPIO下拉
	GPIO_Mode_PU = 0x0103,							//GPIO上拉

}GPIOMode_TypeDef;  
	
	
/** 
* @brief  Configuration Direction enumeration  
*/
typedef enum
{ 
	GPIO_Direct_In = 0x00,						//输入	
	GPIO_Direct_Out =0x01,						//输出
}GPIODirect_TypeDef; 
	
	
/** 
  * @brief  GPIO Init structure definition  
  */	
typedef struct
{
	uint16_t GPIO_Pin;             /*!< Specifies the GPIO pins to be configured.
																			This parameter can be any value of @ref GPIO_pins_define */
	GPIODirect_TypeDef  GPIO_Direct;
	
	GPIOMode_TypeDef GPIO_Mode;    /*!< Specifies the operating mode for the selected pins.
																			This parameter can be a value of @ref GPIOMode_TypeDef */	
}GPIO_InitTypeDef;
	
	
/** 
  * @brief  Bit_SET and Bit_RESET enumeration  
  */

	
typedef enum
{ 
	Bit_RESET = 0,
	Bit_SET
}BitAction;
	
	
void LDPY_GPIO_Init(PORT_Type * PORTx, GPIO_InitTypeDef *GPIO_InitStruct);
void LDPY_GPIO_SetBits(GPIO_Type * GPIOx,uint32_t GPIO_Pin);
void LDPY_GPIO_ResetBits(GPIO_Type * GPIOx,uint32_t GPIO_Pin);
void LDPY_GPIO_WriteBit(GPIO_Type* GPIOx, uint32_t GPIO_Pin, BitAction BitVal);
void LDPY_GPIO_Write(GPIO_Type* GPIOx, uint32_t PortVal);
uint8_t LDPY_GPIO_ReadInputDataBit(GPIO_Type* GPIOx, uint32_t GPIO_Pin);
uint32_t LDPY_GPIO_ReadInputData(GPIO_Type* GPIOx);


#endif
	
	
	
	
	
	
	
	
	
