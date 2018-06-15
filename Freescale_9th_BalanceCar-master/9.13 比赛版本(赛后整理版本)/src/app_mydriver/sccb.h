#ifndef __SCCB_H__
#define __SCCB_H__


//--- 默认SCL接PE11   SDA接PE12------//
void sccb_init(void);                           //初始化SCCB端口为GPIO
void sccb_wait(void);                           //SCCB时序延时
void sccb_start(void);                          //起始标志
void sccb_stop(void);                           //停止标志
unsigned char sccb_sendByte(unsigned char data);
unsigned char sccb_regWrite(unsigned char device,unsigned char address,unsigned char data);
unsigned char sccb_refresh(void);


#define SCL_OUT   DDRE11=1     
#define SCL_HIGH  PTE11_OUT=1
#define SCL_LOW   PTE11_OUT=0

#define SDA_OUT   DDRE12=1      
#define SDA_HIGH  PTE12_OUT=1
#define SDA_LOW   PTE12_OUT=0
#define SDA_IN    DDRE12=0      
#define SDA_DATA  PTE12_IN



#endif