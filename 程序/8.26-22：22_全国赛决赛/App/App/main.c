#include "common.h"
#include "include.h"

#define SECTOR_NUM  (FLASH_SECTOR_NUM-1)         //尽量用最后面的扇区，确保安全

uint8 *imgbuff = (uint8 *)(((uint8 *)&nrf_tx_buff) + COM_LEN);     //图像地址

/*** 调试参数 ***/
uint8  var1, var2;
uint16 var3, var4;
uint32 var5, var6;
extern uchar Annulus_Flag;
uint8 display_temp[20];
/*********** main函数***********/
  
//aaa

void  main(void)
{	
    uchar i;
    Point = 40;   //中心线，人认为40，单片机认为是39  //先不改  
    
//    /************************ flash数据定义  ***********************/
//    uint32  data32;
//    uint16  data16;
//    uint8   data8;
    
    DisableInterrupts;                 //禁止全部中断 
    System_Init();		              //初始化所有模块		
    SCCB_WriteByte (OV7725_CNST, CNST_Value);	  //改变图像阈值	白天50	晚上60
    EnableInterrupts;				  //使能全部中断
    ov7725_eagle_img_flag = IMG_START;//开始采集图像
	enable_irq(PORTA_IRQn);           //允许PTA的中断
//    /************************ 初始化flash  ***********************/
//    flash_init();                                       //初始化flash
//    flash_erase_sector(SECTOR_NUM);                     //擦除扇区
//    if( 1 == flash_write(SECTOR_NUM, 0, 0x12345678) )   //写入数据到扇区，偏移地址为0，必须一次写入4字节
//        //if是用来检测是否写入成功，写入成功了就读取
//    {
//        data32 =    flash_read(SECTOR_NUM, 0, uint32);  //读取4字节
//        printf("一次读取32位的数据为：0x%08x\n", data32);
//
//        data16 =    flash_read(SECTOR_NUM, 0, uint16);  //读取2字节
//        printf("一次读取16位的数据为：0x%04x\n", data16);
//
//        data8  =    flash_read(SECTOR_NUM, 0, uint8);   //读取1字节
//        printf("一次读取8位的数据为：0x%02x\n", data8);
//    }
  	while(1)
  	{
        /************************ 图像数据显示   ***********************/  
        if (Img_Display)
        {
                Display_img();
                OLED_ShowLongNum(100,0,Line_Count);
                OLED_ShowLongNum(100,1,Point);
                OLED_ShowLongNum(100,2,Foresight);
                OLED_ShowLongNum(100,3,Annulus_Nums);
                OLED_ShowLongNum(100,4,Annulus_Small);
                OLED_ShowLongNum(100,5,Annulus_Mode);

                sprintf(display_temp,"%4d",Fictitious_Beeline_Num);
                OLED_ShowStr(90,6,display_temp, 1);
                /************************ 图像数据发送   ***********************/  
//                if(Image_Send_Flag && Send_OK) //选择是否发送图像信息至上位机
//                {
//                    Send_OK = 0;
//                                if (Annulus_Flag)
//                                {
//                                    for(i=0;i<79;i++)
//                                    {
//                                        img[1*80+i]=0;
//                                        img[3*80+i]=0;
//                                        img[5*80+i]=0;
//                                    }
//                                }
//                    img_recontract(img, imgbuff,CAMERA_SIZE); //图像压缩
//        //             save_var(VAR3, Track_Line_Num);
//                    data_send();							   //发送至上位机
//        //            Data_Send_Scope();
//        //            vcan_sendimg(imgbuff,CAMERA_SIZE);			//发送至上位机
//                }
                /************************ 图像数据发送结束   ***********************/ 
               
        }
        //OLED_ShowLongNum(100,2,Foresight);
		/************************ 无线数据发送   ***********************/        
		

	}
}




