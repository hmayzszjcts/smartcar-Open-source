#include "OLED.h"
#include "OLED_Font.h"

/*************************************OLED**********************************

*************************************OLED**********************************/


void OLED_IIC_Init(void)
{
	gpio_init(PTC14, GPO,1);
	gpio_init(PTC15, GPO,1);
	
	port_init_NoALT (PTC14, ODO | PULLUP);//ODO
	port_init_NoALT (PTC15, ODO | PULLUP);
}


//开始信号
void OLED_IIC_Start(void)
{
 
    OLED_IIC_SDA1();
    OLED_IIC_SCL1();
    simiic_delay();
    OLED_IIC_SDA0();
    simiic_delay();
    OLED_IIC_SCL0();
}

void OLED_IIC_Stop(void)
{
  
    OLED_IIC_SDA0();
    OLED_IIC_SCL1();
    simiic_delay();
    OLED_IIC_SDA1();
}



static int I2C_WaitAck(void)
{
    OLED_IIC_SCL0();
	OLED_SDA_IN();
	simiic_delay();
	
	OLED_IIC_SCL1();
    simiic_delay();
	
    if(READ_SDA)           //应答为高电平，异常，通信失败
    {
        SDA_OUT();
        OLED_IIC_SCL0();
        return 0;
    }
    SDA_OUT();
    OLED_IIC_SCL0();
	simiic_delay();
    return 1;
}

//写一个字节
void Write_OneByte(unsigned char Dat)
{
  	uint8 i = 8;
    while(i--)
    {
        if(Dat & 0x80)	OLED_IIC_SDA1();//SDA 输出数据
        else			OLED_IIC_SDA0();
        Dat <<= 1;
        OLED_IIC_SCL1();                //SCL 拉高，采集信号
        OLED_IIC_SCL0();                //SCL 时钟线拉低
    }
	I2C_WaitAck();
}	


//------------------------------------------------------
//写命令
//------------------------------------------------------
void WriteCmd(unsigned char IIC_Cmd)
{
  OLED_IIC_Start();
  Write_OneByte(0x78);
	Write_OneByte(0x00);
	Write_OneByte(IIC_Cmd);
	OLED_IIC_Stop();

}
//------------------------------------------------------
//写数据
//------------------------------------------------------
void WriteDat(unsigned char IIC_Dat)
{
  OLED_IIC_Start();
  Write_OneByte(0x78);
	Write_OneByte(0x40);
	Write_OneByte(IIC_Dat);
	OLED_IIC_Stop();
}
//-----------------------------------------------------
//OLED初始化
//-----------------------------------------------------
void OLED_Init(void)
{
	DELAY_MS(100); //这里的延时很重要
	
	WriteCmd(0xAE); //display off
	WriteCmd(0x20);	//Set Memory Addressing Mode	
	WriteCmd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	WriteCmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	WriteCmd(0xc8);	//Set COM Output Scan Direction
	WriteCmd(0x00); //---set low column address
	WriteCmd(0x10); //---set high column address
	WriteCmd(0x40); //--set start line address
	WriteCmd(0x81); //--set contrast control register
	WriteCmd(0xff); //亮度调节 0x00~0xff
	WriteCmd(0xa1); //--set segment re-map 0 to 127
	WriteCmd(0xa6); //--set normal display
	WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
	WriteCmd(0x3F); //
	WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	WriteCmd(0xd3); //-set display offset
	WriteCmd(0x00); //-not offset
	WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
	WriteCmd(0xf0); //--set divide ratio
	WriteCmd(0xd9); //--set pre-charge period
	WriteCmd(0x22); //
	WriteCmd(0xda); //--set com pins hardware configuration
	WriteCmd(0x12);
	WriteCmd(0xdb); //--set vcomh
	WriteCmd(0x20); //0x20,0.77xVcc
	WriteCmd(0x8d); //--set DC-DC enable
	WriteCmd(0x14); //
	WriteCmd(0xaf); //--turn on oled panel
}

void OLED_SetPos(unsigned char x, unsigned char y) //设置起始点坐标
{ 
	WriteCmd(0xb0+y);
	WriteCmd(((x&0xf0)>>4)|0x10);
	WriteCmd((x&0x0f)|0x01);
}

void OLED_Fill(unsigned char fill_Data)//全屏填充
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		WriteCmd(0xb0+m);		//page0-page1
		WriteCmd(0x00);		//low column start address
		WriteCmd(0x10);		//high column start address
		for(n=0;n<128;n++)
			{
				WriteDat(fill_Data);
			}
	}
}

void OLED_CLS(void)//清屏
{
	OLED_Fill(0x00);
}

//--------------------------------------------------------------
// Prototype      : void OLED_ON(void)
// Calls          : 
// Parameters     : none
// Description    : 将OLED从休眠中唤醒
//--------------------------------------------------------------
void OLED_ON(void)
{
	WriteCmd(0X8D);  //设置电荷泵
	WriteCmd(0X14);  //开启电荷泵
	WriteCmd(0XAF);  //OLED唤醒
}

//--------------------------------------------------------------
// Prototype      : void OLED_OFF(void)
// Calls          : 
// Parameters     : none
// Description    : 让OLED休眠 -- 休眠模式下,OLED功耗不到10uA
//--------------------------------------------------------------
void OLED_OFF(void)
{
	WriteCmd(0X8D);  //设置电荷泵
	WriteCmd(0X10);  //关闭电荷泵
	WriteCmd(0XAE);  //OLED休眠
}

//--------------------------------------------------------------
// Prototype      : void OLED_ShowChar(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
// Calls          : 
// Parameters     : x,y -- 起始点坐标(x:0~127, y:0~7); ch[] -- 要显示的字符串; TextSize -- 字符大小(1:6*8 ; 2:8*16)
// Description    : 显示codetab.h中的ASCII字符,有6*8和8*16可选择
//--------------------------------------------------------------
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
{
	unsigned char c = 0,i = 0,j = 0;
	switch(TextSize)
	{
		case 1:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 126)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<6;i++)
					WriteDat(F6x8[c][i]);
				x += 6;
				j++;
			}
		}break;
		case 2:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 120)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i]);
				OLED_SetPos(x,y+1);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i+8]);
				x += 8;
				j++;
			}
		}break;
	}
}
void OLED_ShowNum(uint8 x,uint8 y,uint8 num)
{         	
	unsigned char i = 0,j = 0;
        num=num+16;
        if(x > 126)
	{
          x = 0;
          y++;
        }
        OLED_SetPos(x,y);
        for(i=0;i<6;i++)
          WriteDat(F6x8[num][i]);
        x += 6;
        j++;
	
} 

void OLED_ShowLongNum(uint8 x,uint8 y,uint16 num)
{
  uint8 ge=0,shi=0,bai=0;
  bai = num/100;
  shi = num%100/10;
  ge  = num %10;
  OLED_ShowStr(x,y,"    ",1);
  if(bai)
  {
      OLED_ShowNum(x,y,bai);
      OLED_ShowNum(x+6,y,shi);
      OLED_ShowNum(x+12,y,ge);
  }
   else if(shi)
  {
      //OLED_ShowNum(x,y,bai);
      OLED_ShowNum(x,y,shi);
      OLED_ShowNum(x+6,y,ge);
  }
  else
  {
    OLED_ShowNum(x,y,ge);
  }
}


//--------------------------------------------------------------
// Prototype      : void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
// Calls          : 
// Parameters     : x0,y0 -- 起始点坐标(x0:0~127, y0:0~7); x1,y1 -- 起点对角线(结束点)的坐标(x1:1~128,y1:1~8)
// Description    : 显示BMP位图
//--------------------------------------------------------------
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])
{
	unsigned int j=0;
	unsigned char x,y;

  if(y1%8==0)
		y = y1/8;
  else
		y = y1/8 + 1;
	for(y=y0;y<y1;y++)
	{
		OLED_SetPos(x0,y);
    for(x=x0;x<x1;x++)
		{
			WriteDat(BMP[j++]);
		}
	}
}

/*******************************************************************************
 *函数名： Draw_Road();
 *功能描述：将路径图像打印到OLED上
 *说明： 用于IIC 0.96oled显示图像及重要参数
*******************************************************************************/

extern uint8 img[CAMERA_H * CAMERA_W];
void Display_img(void)
{ 	
    int i = 0, j = 0,temp=0;
	for(i=0;i<56;i+=8)
	{
	  	OLED_SetPos(0,i/8);   //24 i/8
	  	for(j=0;j<80;j++) 
	  	{ 
	  	  temp=0;
	  	  if(img[(0+i)*80 + j]) temp|=0x01;
	  	  if(img[(1+i)*80 + j]) temp|=0x02;
	  	  if(img[(2+i)*80 + j]) temp|=0x04;
	  	  if(img[(3+i)*80 + j]) temp|=0x08;
	  	  if(img[(4+i)*80 + j]) temp|=0x10;
	  	  if(img[(5+i)*80 + j]) temp|=0x20;
	  	  if(img[(6+i)*80 + j]) temp|=0x40;
	  	  if(img[(7+i)*80 + j]) temp|=0x80;
          WriteDat(temp);
	  	}       
	}
}