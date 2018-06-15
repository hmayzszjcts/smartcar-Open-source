#include"include.h"
/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名       ：OwnSD_Driver.c
 * 描述         ：所有模块初始化代码
 * 实验平台     ：凌立印象开发板
 * 库版本       ：基于野火库
 * 嵌入系统     ：
 * 作者         ：xuxu
**********************************************************************************/ 


FIL fdst;  //文件
FATFS fs;  //文件系统
UINT bw;
unsigned char Send_Img[81];
unsigned int add=0;
static unsigned char Sd_writeOnce=0;
unsigned char SD_enable=0;
static unsigned int flash_write_count=0;
#define write_count_max 2240  //2240/75=29.8s 75为1s帧数 


/************************************************
*  函数名称：SDcard_Init
*  功能说明：SD卡初始化（使用K60自带的SD模块）
*  参数说明：无
*  函数返回：无
*  修改时间：2014-1-14    已经测试
*************************************************/
void SDcard_Init()
{
    FIL fdst;  //文件
    FATFS fs;  //文件系统
    int res;
    char *str = "感谢您选用 野火Kinetis开发板 ！^_^ ";
    f_mount(0, &fs);	//SD卡系统初始化
    res = f_open(&fdst, "\\FireDemo.txt", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);  //打开文件，如果没有就创建，带读写打开
    if( res == FR_DISK_ERR)
    {
        printf( "\n没插入SD卡？？\n" );
    }
    else if ( res == FR_OK )
    {
        printf( "\n文件打开成功 \n" );
    }
    else
    {
        printf("\n返回值异常");
    }
    f_puts(str, &fdst);
    f_close(&fdst);//关闭文件 
}



   
   
/*************************************************************************
*  函数名称：SD_Writedata
*  功能说明：
*  参数说明：
*  函数返回：无
*  修改时间：2012-2-14    已测试
*************************************************************************/
void SD_Writedata()
{
    //存单个图像
    static int SECTOR_NUM=0;
    static unsigned int H,L,write_data = 0;
    static unsigned char data8; 
    static unsigned int data32;
    static short int wH,wL;
    
    //仅仅存一幅图像
    /*if(Sd_writeOnce==0)//write flash
    {
        LCD_PutString(0,60,"SD start write!!!",Red,Green);
        //停车
        Sd_writeOnce = 1;
        //操作
        f_mount(0, &fs);	//SD卡系统初始化
        f_open(&fdst, "\\data.txt", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);  //打开文件，如果没有就创建，带读写打开
    }
    if(Sd_writeOnce == 1)
    {
        Sd_writeOnce = 2;
        for(int j=0;j<240;j++)
        {
            for(int i=0,ii=0;i<40;i++)
            {
                Send_Img[ii++] = ((((unsigned int)(Image_fire[j][i]))&0xf0)>>4) + 'a';
                Send_Img[ii++] = (((unsigned int)(Image_fire[j][i]))&0x0f) + 'a';
            }
            Send_Img[80]='\n';
            f_write(&fdst,Send_Img,81,&bw);
        }
        LCD_PutString(0,60,"SD write finish!!!",Red,Green);
        f_close(&fdst);//关闭文件 
    }
     return;  */
        
        
    //初始化
    if(Sd_writeOnce==0)//write flash
    {
        #define Send_hang 150
        //Image_fire[Send_hang]
        SECTOR_NUM = flash_write_count/32;
        add = (flash_write_count%32)*64;
        flash_write_buf(186 + SECTOR_NUM,add,40,Image_fire[Send_hang]);
        
        //Time_1ms
        //centre[Send_hang]
        H = (unsigned int)(Time_1ms);
        L = (unsigned int)(centre[Send_hang]);
        write_data = (H<<16) + L;
        flash_write(186 + SECTOR_NUM,add + 40,write_data);
        
        //black_L[Send_hang]
        //black_R[Send_hang]
        H = (unsigned int)(black_L[Send_hang]);
        L = (unsigned int)(black_R[Send_hang]);
        write_data = (H<<16) + L;
        flash_write(186 + SECTOR_NUM,add + 44,write_data);
        
        //halfwidth[Send_hang]
        //Angle.m_angle
        H = (unsigned int)(halfwidth[Send_hang]);
        L = (unsigned int)((int)(Angle.m_angle*10));
        write_data = (H<<16) + L;
        flash_write(186 + SECTOR_NUM,add + 48,write_data);
        
        //Angle.G_angle
        //Speed.Car
        H = (unsigned int)((int)(Angle.G_angle*10));
        L = (unsigned int)((int)(Speed.Car));
        write_data = (H<<16) + L;
        flash_write(186 + SECTOR_NUM,add + 52,write_data);
        
        //Direction.Gyro
        //Angle.PWM
        H = (unsigned int)((int)(Direction.Gyro));
        L = (unsigned int)((int)(Angle.PWM));//all_PWM_L
        write_data = (H<<16) + L;
        flash_write(186 + SECTOR_NUM,add + 56,write_data);
        
        //Speed.PWM
        //Speed.PWM_Integral
        H = (unsigned int)((int)(Speed.PWM));//all_PWM_R
        L = (unsigned int)((int)(Speed.PWM_Integral));
        write_data = (H<<16) + L;
        flash_write(186 + SECTOR_NUM,add + 60,write_data);
        
        flash_write_count++;
        if(flash_write_count >= write_count_max)
           Sd_writeOnce = 1;
    }
    else if(Sd_writeOnce==1 && StartLine.flag==1 && Time_1ms>StartLine.time_record + StartLine.Delaytime + 1)//read flash and write SD
    {
        //界面
        _LED4 = 0;
        LCD_PutString(0,80,"SD start write!!!",Red,Green);
        //操作
        f_mount(0, &fs);	//SD卡系统初始化
        f_open(&fdst, "\\data.txt", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);  //打开文件，如果没有就创建，带读写打开
        for(int SECTOR_count = 186;SECTOR_count <= 255;SECTOR_count++)
        {
            for(int in_count = 0;in_count < 32;in_count++)
            {
                int read_add = in_count * 64;
                for(int i=0,ii=0;i<40;i++)
                {
                    data8 = flash_read(SECTOR_count, read_add + i,uint8);
                    Send_Img[ii++] = ((((unsigned int)(data8))&0xf0)>>4) + 'a';
                    Send_Img[ii++] = (((unsigned int)(data8))&0x0f) + 'a';
                }
                f_write(&fdst,Send_Img,80,&bw);
                for(int i=40;i<64;i+=4)
                {
                    data32 = flash_read(SECTOR_count,read_add + i,uint32);	//读取4字节
                    wH = (short int)(data32>>16);
                    wL = (short int)(data32&0x0000ffff);
                    f_printf(&fdst, " %d", wH);
                    if(i==60)
                        f_printf(&fdst, " %d\n", wL);
                    else
                        f_printf(&fdst, " %d", wL);
                }
            } 
        }
        f_close(&fdst);//关闭文件 
        Sd_writeOnce = 2;
        LCD_PutString(0,80,"SD write finish!!!",Red,Green);
    }
    else if(Sd_writeOnce ==2)
    {
        _LED4 = (Time_1ms/500)%2;
        return;
    }
}