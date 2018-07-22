
#include "myOLED.h"

uint8_t OLED_GRAM[8][128];
extern uint8_t  ASCII_code[101][6];
extern uint16_t ADC_value[128];

/*************************************************************************
*				零	度	偏	移   
*  函数名称:  myOLED_GPIO_Init
*  功能说明:  OLED引脚初始化函数
*  参数说明:	
*            
*  函数返回:
*  修改时间:2014-05-09
*  备注:  
*************************************************************************/ 
void myOLED_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//OREST
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_PU;
	GPIO_InitStructure.GPIO_Direct=GPIO_Direct_Out;
	LDPY_GPIO_Init(PORTE,&GPIO_InitStructure);
	
	//ODC
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_PU;
	GPIO_InitStructure.GPIO_Direct=GPIO_Direct_Out;
	LDPY_GPIO_Init(PORTE,&GPIO_InitStructure);
	
	//OSCK
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_PU;
	GPIO_InitStructure.GPIO_Direct=GPIO_Direct_Out;
	LDPY_GPIO_Init(PORTE,&GPIO_InitStructure);
	
	//OSDA
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_PU;
	GPIO_InitStructure.GPIO_Direct=GPIO_Direct_Out;
	LDPY_GPIO_Init(PORTE,&GPIO_InitStructure);

}

/*************************************************************************
*				零	度	偏	移   
*  函数名称:  myOLED_Init
*  功能说明:  OLED初始化
*  参数说明:	
*            
*  函数返回:
*  修改时间:2014-05-09
*  备注:  
*************************************************************************/ 

void myOLED_Init(void)
{
	myOLED_GPIO_Init();
	myOLED_REST();
	
	myOLED_Write_REG(0xAE);	//关闭显示
	
	myOLED_Write_REG(0xD5);	//时钟分频因子，振荡频率
	myOLED_Write_REG(0x80);	//[3:0]分频因子;[7:4]振荡频率
	
	myOLED_Write_REG(0xA8);	//驱动格数
	myOLED_Write_REG(0x3F);	//1/64
	
	myOLED_Write_REG(0xD3);	//显示偏移
	myOLED_Write_REG(0x02);	//0
	
	myOLED_Write_REG(0x40);	//显示开始行[5:0]行数
	
	myOLED_Write_REG(0x8D);	//电荷泵
	myOLED_Write_REG(0x14);	//bit2开启/关闭
	
	myOLED_Write_REG(0x20);	//内存地址模式
	myOLED_Write_REG(0x02);	//[1:0]00列模式;01行模式;10默认页地址模式
		
	myOLED_Write_REG(0xA1);	//段重定义,左右扫描
	
	myOLED_Write_REG(0xC0);	//COM扫描方向bit3:0普通模式;1重定义模式
	
	myOLED_Write_REG(0xDA);	//COM硬件引脚
	myOLED_Write_REG(0x10);	//[5:4]配置
	
	myOLED_Write_REG(0x81);	//对比度
	myOLED_Write_REG(0xEF);	//
	
	myOLED_Write_REG(0xD9);	//预充电周期
	myOLED_Write_REG(0xF1);	//[3:0]PHASE 1[7:4]PHASE 2
	
	myOLED_Write_REG(0xDB);	//VCOMH电压倍率
	myOLED_Write_REG(0x30);	//
	
	myOLED_Write_REG(0xA4);	//全局显示bit0:1开启，0关闭
	myOLED_Write_REG(0xA6);	//显示方式bit0:1反向，0正常
	
	myOLED_Write_REG(0xAF);	//开启显示
	
	myOLED_Clear();
	
}
/*************************************************************************
*				零	度	偏	移   
*  函数名称:  myOLED_OLEDInit
*  功能说明:  OLED写寄存器函数
*  参数说明:	
*            
*  函数返回:
*  修改时间:2014-05-09
*  备注:  
*************************************************************************/ 
void myOLED_Write_REG(uint8_t COM)
{
	uint8_t i = 0;
	OLED_DC=0;	//0:指令
	for(i=0;i<8;i++)
	{
			if(COM&0x80)
				OLED_DA=1;
			else
				OLED_DA=0;
				
			OLED_CK=0;
	//		Delay_us(10);
			OLED_CK=1;
	//		Delay_us(10);
			
			COM<<=1;
	}
}
/*************************************************************************
*				零	度	偏	移   
*  函数名称:  myOLED_Write_DAT
*  功能说明:  OLED写数据
*  参数说明:	
*            
*  函数返回:
*  修改时间:2014-05-09
*  备注:  
*************************************************************************/ 	
void myOLED_Write_DAT(uint8_t DAT)
{
	uint8_t i = 0;
	OLED_DC = 1;		//1数据
	for(i=0;i<8;i++)
	{
			if(DAT&0x01)		//本来应该是高位在前，但是显示出来时倒立的，所以选择低位在前
				OLED_DA=1;
			else 
				OLED_DA=0;
				
			OLED_CK=0;
	//		Delay_us(10);
			OLED_CK=1;
	//		Delay_us(10);
			
			DAT>>=1;			//低位在前
	}
}
/*************************************************************************
*				零	度	偏	移   
*  函数名称:  myOLED_REST
*  功能说明:  OLED复位
*  参数说明:	
*            
*  函数返回:
*  修改时间:2014-05-09
*  备注:  
*************************************************************************/
void myOLED_REST(void)
{
	OLED_RST=0;
	LDPY_Delay_ms(50);
	OLED_RST=1;
	LDPY_Delay_us(50);
}
/*************************************************************************
*				零	度	偏	移   
*  函数名称:  myOLED_Clear
*  功能说明:  OLED清屏
*  参数说明:	
*            
*  函数返回:
*  修改时间:2014-05-09
*  备注:  
*************************************************************************/
void myOLED_Clear(void)
{
	uint8_t H = 0,L = 0;
	for(H=0;H<8;H++)
	  for(L=0;L<128;L++)
			OLED_GRAM[H][L]=0x00;
	myOLED_RefreshGRAM();
}
/*************************************************************************
*				零	度	偏	移   
*  函数名称:  myOLED_RefreshGRAM
*  功能说明:  OLED写数据
*  参数说明:	
*            
*  函数返回:
*  修改时间:2014-05-09
*  备注:  	暂时没有用到
*************************************************************************/
void myOLED_RefreshGRAM(void)
{
		uint8_t i = 0,n = 0;
		for(i=0;i<8;i++)
		{
				myOLED_Write_REG(0xB0+i);	//
				myOLED_Write_REG(0x02);	  //
				myOLED_Write_REG(0x10);	  //
				for(n=0;n<128;n++)
					myOLED_Write_DAT(OLED_GRAM[i][n]);//
		}
}
/*************************************************************************
*				零	度	偏	移   
*  函数名称:  myOLED_String
*  功能说明:  OLED显示字符串
*  参数说明:	
*            
*  函数返回:
*  修改时间:2014-05-09
*  备注:  
*************************************************************************/
void myOLED_String(uint8_t x,uint8_t y,const char *P)
{
	uint8_t i = 0;
	myOLED_Set_XY(x,y);//
	for(i=0;i<strlen(P);i++)
		{
			myOLED_Char(P[i]-32);//
		}

}
/*************************************************************************
*				零	度	偏	移   
*  函数名称:  myOLED_Char
*  功能说明:  OLED显示字符
*  参数说明:	
*            
*  函数返回:
*  修改时间:2014-05-09
*  备注:  
*************************************************************************/
void myOLED_Char(uint8_t ch)
{
	uint8_t Lie = 0;//没个字符有6列
			  //字符在ASCII表中的位置
	
	for(Lie=0;Lie<6;Lie++) //分6列打印
	{
		myOLED_Write_DAT(ASCII_code[ch][Lie]);
	}
}
/*************************************************************************
*				零	度	偏	移   
*  函数名称:  
*  功能说明:  
*  参数说明:	
*            
*  函数返回:
*  修改时间:2014-05-09
*  备注:  在XY位置花点X：0~7(页方式);Y:0~128(列方式)
*********************************************************/
void myOLED_XY_point(uint8_t x,uint8_t y,uint8_t ch)
{
	myOLED_Set_XY( x, y);//
	myOLED_Write_DAT(ch);//
}
/*************************************************************************
*				零	度	偏	移   
*  函数名称:  myOLED_Set_XY
*  功能说明:  OLED设置页码
*  参数说明:	
*            
*  函数返回:
*  修改时间:2014-05-09
*  备注:  
*************************************************************************/
void myOLED_Set_XY(uint8_t x,uint8_t y)
{
	myOLED_Write_REG(0xB0+x);//
	myOLED_Write_REG(((y&0xF0)>>4)|0x10);//
	myOLED_Write_REG((y&0x0F)|0x01);//
}
/*************************************************************************
*				零	度	偏	移   
*  函数名称:  myOLED_Dec
*  功能说明:  OLED显示5位数据
*  参数说明:	
*            
*  函数返回:
*  修改时间:2014-05-09
*  备注:  
*************************************************************************/
void myOLED_Dec(uint8_t x,uint8_t y, uint16_t DEC)
{
	uint8_t x5 = 0,x4 = 0,x3 = 0,x2 = 0,x1 = 0,row = 0;
	
	x5=DEC/10000+16;//
	x4=DEC%10000/1000+16;//
	x3=DEC%1000/100+16;//
	x2=DEC%100/10+16;//
	x1=DEC%10+16;//
	myOLED_Set_XY( x, y);//
	for(row=0;row<6;row++)
		myOLED_Write_DAT(ASCII_code[x5][row]);
	for(row=0;row<6;row++)
		myOLED_Write_DAT(ASCII_code[x4][row]);
	for(row=0;row<6;row++)
		myOLED_Write_DAT(ASCII_code[x3][row]);
	for(row=0;row<6;row++)
		myOLED_Write_DAT(ASCII_code[x2][row]);
	for(row=0;row<6;row++)
		myOLED_Write_DAT(ASCII_code[x1][row]);
}

/*************************************************************************
*				零	度	偏	移   
*  函数名称:  myOLED_Dec
*  功能说明:  OLED显示5位数据
*  参数说明:	
*            
*  函数返回:
*  修改时间:2014-05-09
*  备注:  
*************************************************************************/
void myOLED_Dec4(uint8_t x,uint8_t y, int16_t DEC)
{
	uint8_t x5 = 0,x4 = 0,x3 = 0,x2 = 0,x1 = 0,row = 0;
	uint8_t i = 0;
	
	if( DEC < 0)
	{
		DEC = 0 - DEC;
		i = 1;
	}
	x5=DEC/10000+16;//
	x4=DEC%10000/1000+16;//
	x3=DEC%1000/100+16;//
	x2=DEC%100/10+16;//
	x1=DEC%10+16;//
	myOLED_Set_XY( x, y);//
	if(i == 0)
	{
		for(row=0;row<6;row++)
			myOLED_Write_DAT(ASCII_code[x5][row]);
	}
	else
	{
		myOLED_Char( 13 );
	}
	for(row=0;row<6;row++)
		myOLED_Write_DAT(ASCII_code[x4][row]);
	for(row=0;row<6;row++)
		myOLED_Write_DAT(ASCII_code[x3][row]);
	for(row=0;row<6;row++)
		myOLED_Write_DAT(ASCII_code[x2][row]);
	for(row=0;row<6;row++)
		myOLED_Write_DAT(ASCII_code[x1][row]);
}

/*************************************************************************
*				零	度	偏	移   
*  函数名称:  myOLED_Write_DAT
*  功能说明:  显示ADC的值
*  参数说明:	
*            
*  函数返回:
*  修改时间:2014-05-09
*  备注:  	显示ADC的值
		ADC的值为16位数据，分为8段，分别显示在0~7页
		128个数据刚好显示完，每一列只打印0x18，形成画线
*********************************************************/
void myOLED_ADC_value(uint16 * value)
{
	uint8_t n = 0,PA = 0;
	uint16_t temp = 0;
	myOLED_Set_XY(0,0);
	for(n=0;n<128;n++)
	{
		temp=*(value+n);//
		
		if(temp>0x00&&temp<0x01F4)
				PA=0;
		else if((temp>=0x01F5)&&(temp<=0x03F8))
				PA=1;
		else if((temp>=0x03F9)&&(temp<=0x05DC))
				PA=2;
		else if((temp>=0x05DD)&&(temp<=0x07D0))
				PA=3;
		else if((temp>=0x07D1)&&(temp<=0x09C4))
				PA=4;
		else if((temp>=0x09C5)&&(temp<=0x0BB8))
				PA=5;
		else if((temp>=0x0BB9)&&(temp<=0x0DAC))
				PA=6;
		else if((temp>=0x0DAD)&&(temp<=0x1004))
				PA=7;
		
		myOLED_XY_point( PA, n,0x18);//	

	}
	myOLED_Clear();
}
/***********************************************************/


/*************************************************************************
*                             零度偏移
*  函数名称:myOLED_Decimals
*  功能说明:显示小数  格式 _._ _ _
*  参数说明:
*            
*  函数返回:
*  修改时间:
*  备		 注:
*************************************************************************/ 
void myOLED_Decimals(uint8_t x,uint8_t y, float als)
{
	uint16_t temp0 = 0,temp1 = 0,temp2 = 0 , temp3 = 0;
	uint16_t temp16 = 0;
	
	temp16 = (uint16_t)(als * 100);
	temp0 = (uint16_t)(temp16 / 1000);
	temp1 = (uint16_t)(temp16 % 1000 / 100);
	temp2 = (uint16_t)(temp16 % 100 / 10);
	temp3 = (uint16_t)(temp16 % 10 / 1);
	
	
	myOLED_Set_XY( x, y);

	myOLED_Char( temp0 + 16 );
	myOLED_Char( temp1 + 16 );
	myOLED_Char( 14 );
	myOLED_Char(temp2 + 16);
	myOLED_Char(temp3 + 16 );
	
}
/*************************************************************************
*                             零度偏移
*  函数名称:myOLED_
*  功能说明:
*  参数说明:
*            
*  函数返回:
*  修改时间:
*  备		 注:
*************************************************************************/ 
void myOLED_CCDwave(CCD_Info * CCD1_info,CCD_Info * CCD2_info, CCD_Info * CCD3_info)
{
	
	uint8_t i = 0, PA1 = 0, PA2 = 0, PA3 = 0;

	myOLED_Set_XY( 0, 0);

	for( i=0; i<128; i++)
	{
		PA1 = CCD1_info->PixelBinary[i];
		PA2 = CCD2_info->PixelBinary[i];
		PA3 = CCD3_info->PixelBinary[i];
		
		if(PA1 == 1)
		{
			myOLED_XY_point( 1, i,0x02);
		}
		else
		{
			myOLED_XY_point( 1, i,0x40);
		}
		if(PA2 == 1)
		{
			myOLED_XY_point( 2, i,0x02);
		}
		else
		{
			myOLED_XY_point( 2, i,0x40);
		}
		if(PA3 == 1)
		{
			myOLED_XY_point( 3, i,0x02);
		}
		else
		{
			myOLED_XY_point( 3, i,0x40);
		}
		
	}
	
	myOLED_XY_point( 1, CCD1_info->CentralLine[0],7);
	myOLED_XY_point( 2, CCD2_info->CentralLine[0],7);
	myOLED_XY_point( 3, CCD3_info->CentralLine[0],7);
	
}









