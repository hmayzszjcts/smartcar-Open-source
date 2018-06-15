#ifndef _ABOUT_IMAGE_H
#define _ABOUT_IMAGE_H


void Send_Image_to_LCD(unsigned char img[CAMERA_H][CAMERA_DMA_NUM]);
void Send_ImageLine_to_LCD(unsigned char Pixel[],unsigned hang);
void Send_Image_to_Demok(unsigned int hang);

#endif