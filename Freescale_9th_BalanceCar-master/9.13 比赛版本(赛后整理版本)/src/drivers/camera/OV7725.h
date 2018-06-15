#ifndef __OV7725_H
#define __OV7725_H 


#define OV7725_Delay_ms(time)  DELAY_MS(time)
#define OV7725_ID           0x21

#define	CAMERA_DMA_CH 	    DMA_CH0	//定义摄像头的DMA采集通道
#define CAMERA_W            320         //定义摄像头图像宽度
#define CAMERA_H            240		//定义摄像头图像高度
#define CAMERA_INTERLACE    1           //摄像头间隔采集行数 n - 1,这里1表示不隔行采集，2表示隔行采集
#define CAMERA_DMA_NUM      (CAMERA_W /8 )    //DMA采集次数
#define CAMERA_SIZE         (CAMERA_W * CAMERA_H /8)        //图像占用空间大小
#define CAMERA_DMA_LINE     (CAMERA_H/CAMERA_INTERLACE)     //实际采集行数


extern unsigned char Image_fire[CAMERA_H+1][CAMERA_DMA_NUM];
extern volatile unsigned char img_flag;//图像状态


//定义图像采集状态
typedef enum 
{
    IMG_NOTINIT=0,
    IMG_FINISH,			        //图像采集完毕
    IMG_FAIL,				//图像采集失败(采集行数少了)
    IMG_GATHER,				//图像采集中
    IMG_START,				//开始采集图像
    IMG_STOP,				//禁止图像采集
    
}IMG_STATE;


typedef struct
{
    u8 Address;			       /*寄存器地址*/
    u8 Value;		           /*寄存器值*/
}Register_Info;



unsigned char Ov7725_reg_Init(void);
extern unsigned char Ov7725_Init();
extern void Ov7725_exti_Init();
extern void ov7725_get_img();
extern int OV7725_ReadReg(u8 LCD_Reg,u16 LCD_RegValue);
extern int OV7725_WriteReg(u8 LCD_Reg,u16 LCD_RegValue);


#endif























