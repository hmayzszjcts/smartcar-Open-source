#include "include.h"


/*************************************************************************
*  函数名称：Send_Image_to_LCD
*  功能说明：在显示屏上显示鹰眼图像
*  参数说明：img为指针
*  函数返回：无
*  修改时间：2012-2-14    已测试
*************************************************************************/
void Send_Image_to_LCD(unsigned char img[CAMERA_H][CAMERA_DMA_NUM])
{
    int i,j,k;
    LCD_SetPos(0,CAMERA_W-1,0,CAMERA_H-1);
    for(i=0;i < CAMERA_H - 20;i++)
    {
        for(j = 0;j < CAMERA_DMA_NUM; j++)
            for(k = 0;k < 8; k++)
                if(img[i][j]&(0x80>>k))              
                    write_word(Black);	//写图像数据
                else
                    write_word(White);		//写图像数据     
    }
}

/*************************************************************************
*  函数名称：LCD_Draw_Imageline
*  功能说明：画Image的一行，显示在显示屏上
*  参数说明：Pixel[]为数组指针，hang为要显示第几行
*  函数返回：无
*  修改时间：2012-2-14    已测试
*************************************************************************/
void Send_ImageLine_to_LCD(unsigned char Pixel[],unsigned hang)
{
    /*static unsigned char ImageLine[320];
    unsigned int i;
    static int centre_line[Process_Hang+2],black_L_line[Process_Hang+2],black_R_line[Process_Hang+2];

    //画左线
    Draw_single_line('L',black_L_line[hang],Black);//还原
    black_L_line[hang]= (int)(black_L[hang]/2.0);
    if(black_L_line[hang] < 0)
        black_L_line[hang] = 0;
    else if(black_L_line[hang]  > 320)
        black_L_line[hang] = 319;
    Draw_single_line('L',black_L_line[hang],White);//上色

    //画中心线
    Draw_single_line('L',centre_line[hang],Black);//还原
    centre_line[hang] = (int)(centre[hang]/2.0);
    if(centre_line[hang] < 0)
        centre_line[hang] = 0;
    else if(centre_line[hang] > 320)
        centre_line[hang] = 319;
    Draw_single_line('L',centre_line[hang],White);//上色

    //画右线
    Draw_single_line('L',black_R_line[hang],Black);//还原
    black_R_line[hang] = (int)(black_R[hang]/2.0);
    if(black_R_line[hang] < 0)
        black_R_line[hang] = 0;
    else if(black_R_line[hang] > 320)
        black_R_line[hang] = 319;
    Draw_single_line('L',black_R_line[hang],White);//上色

    //画当前的一行线
    for(i=0;i<320;i++)
    {
      LCD_SetPos(i,i,239 - ImageLine[i],239 - ImageLine[i]);
      write_word(Black);//还原
      if(Pixel[i*2] > 239)
          ImageLine[i] = 239;
      else
          ImageLine[i] = Pixel[i*2];
      LCD_SetPos(i,i,239 - ImageLine[i],239 -ImageLine[i]);
      write_word(White);//上色
    }*/
}


/*************************************************************************
*  函数名称：LCD_Draw_Imageline
*  功能说明：往demok上位机发送整幅图像数据
*  参数说明：hang为要发送多少行
*  函数返回：无
*  修改时间：2012-2-14    已测试
*************************************************************************/
void Send_Image_to_Demok(unsigned int hang)
{
    /*int i,j;
    uart_putchar(UART3,0xff);
    for(i=0;i < hang;i++)
    {
        for(j = 0;j < Lie; j++) //demok tools 看图模式
        {
            if(Image[i][j]==255)
                uart_putchar(UART3,254);
            else
                uart_putchar(UART3,Image[i][j]);
        }
    }*/
}
