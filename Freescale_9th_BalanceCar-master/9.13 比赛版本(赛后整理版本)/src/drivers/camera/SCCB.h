#ifndef __SCCB_H
#define __SCCB_H


#define SCL_H()         PTE12_OUT = 1
#define SCL_L()         PTE12_OUT = 0
#define	SCL_DDR_OUT() 	DDRE12 = 1
#define	SCL_DDR_IN() 	DDRE12 = 0

#define SDA_H()         PTE11_OUT = 1
#define SDA_L()         PTE11_OUT = 0
#define SDA_IN()      	PTE11_IN
#define SDA_DDR_OUT()	DDRE11 = 1
#define SDA_DDR_IN()	DDRE11 = 0

#define ADDR_OV7725   0x42

#define SCCB_DELAY()	SCCB_delay(100)	


void SCCB_GPIO_init(void);
int SCCB_WriteByte( u16 WriteAddress , u8 SendByte);
int SCCB_ReadByte(u8* pBuffer,   u16 length,   u8 ReadAddress);
static void SCCB_delay(u16 i);


#endif 
