#ifndef __OV7725_H
#define __OV7725_H 

#define	CAMERA_DMA_CH 	DMA_CH0		//定义摄像头的DMA采集通道
extern  volatile u8	IMG_BUFF[60][320][2];	//图像数组


#define OV7725_ID       0x21



typedef struct
{
	u8 Address;			       /*寄存器地址*/
	u8 Value;		           /*寄存器值*/
}Register_Info;

extern u8 Ov7725_vsync;

u8 		Ov7725_Init(void);
void 	Ov7725_exti_Init();
int  	OV7725_ReadReg(u8 LCD_Reg,u16 LCD_RegValue);
int  	OV7725_WriteReg(u8 LCD_Reg,u16 LCD_RegValue);



#endif























