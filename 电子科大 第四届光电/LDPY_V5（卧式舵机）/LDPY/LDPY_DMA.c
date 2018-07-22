#include "LDPY_DMA.h"

  extern CCD_Info CCD1_info;
  extern CCD_Info CCD2_info;
  extern CCD_Info CCD3_info;
  
  extern Road_Condition Road_condition;
  extern Road_Condition Road_condition_Item;
  
  extern Gyro_Info Gyro_info;

  DMA_Required_Variable DMA_Variable_1 = {0};
  DMA_InitTypeDef dma_init_struct = {0};
  
/*************************************************************************
*				零	度	偏	移   
*  函数名称: LDPY_DMA_StructInit
*  功能说明: 结构体初始化
*  参数说明:	 配置DMA的初始化参数
*            
*  函数返回:
*  修改时间:2014-05-19
*  备	注:
*************************************************************************/

void LDPY_DMA_StructInit(DMA_InitTypeDef  *dma_init_struct,CCD_Info *CCD_info,DMA_Required_Variable *DMA_Required_Variable)
{
	dma_init_struct->DMA_Req=UART5_TRAN_DMAREQ;//选择DMA通道对应的请求源，外设DMA请求号
	dma_init_struct->DMA_DestAddr=(uint32_t)&UART5->D;//配置目的数据地址
	dma_init_struct->DMA_DestAddrOffset=0;
	dma_init_struct->DMA_DestDataSize=DMA_DST_8BIT;
	dma_init_struct->DMA_Isr=0;//DMA中断
	dma_init_struct->DMA_LastDestAddrAdj=0;
	dma_init_struct->DMA_MajorCompleteIntEnable=0;
	dma_init_struct->DMA_MajorHalfCompleteIntEnable=0;
	dma_init_struct->DMA_SourceDataSize=DMA_SRC_8BIT;//配置源数据大小
	dma_init_struct->DMA_SourceAddrOffset=1;//配置源数据地址偏移，不必须配置，默认为零
	dma_init_struct->DMA_PeriodicTriggerEnable=FALSE; 
	dma_init_struct->DMA_SourceModulo=0x01;
	/*------------------临时加--------------------*/
	dma_init_struct->DMA_Complete_Flag = FALSE;//主循环未完成数据传输
}


/*************************************************************************
*				零	度	偏	移   
*  函数名称: LDPY_DMA_Init
*  功能说明: 初始化eDMA模块
*  参数说明:	
*            
*  函数返回:
*  修改时间:2014-05-19
*  备	注:
*************************************************************************/
uint8_t LDPY_DMA_init(DMA_InitTypeDef *dma_init_struct)
{
  /*对结构体成员的重定义*/
  uint8_t chx = dma_init_struct->DMA_CHx;
  uint8_t req = dma_init_struct->DMA_Req;
  uint8_t periodic_trigg=dma_init_struct->DMA_PeriodicTriggerEnable;
  uint16_t major_cnt = dma_init_struct->DMA_MajorLoopCnt;
  uint32_t minor_cnt = dma_init_struct->DMA_MinorByteCnt;
  uint32_t src_addr = dma_init_struct->DMA_SourceAddr;
  uint8_t src_dsize = dma_init_struct->DMA_SourceDataSize;
  int16_t src_addroffset = dma_init_struct->DMA_SourceAddrOffset;
  int32_t src_lastadj = dma_init_struct->DMA_LastSourceAddrAdj;
  uint32_t dst_addr = dma_init_struct->DMA_DestAddr;
  uint8_t dst_dsize = dma_init_struct->DMA_DestDataSize;
  int16_t dst_addroffset = dma_init_struct->DMA_DestAddrOffset;
  int32_t dst_lastadj = dma_init_struct->DMA_LastDestAddrAdj;
  uint8_t auto_disable=dma_init_struct->DMA_AutoDisableReq;
	
	SIM->SCGC6|=SIM_SCGC6_DMAMUX_MASK;//打开DMA通道多路复用器的时钟
	SIM->SCGC7|=SIM_SCGC7_DMA_MASK;//打开DMA模块时钟
	
  /*先关闭通道X的外设DMA请求*/	
	DMA0->ERQ &= ~(1<<chx);

	/*选择通道X，配置外设DMA源请求编号*/
	if(req>15)
	{
	  return 0;
	}
	else 
	{
    DMAMUX->CHCFG[chx] |= DMAMUX_CHCFG_SOURCE(req);
	}
	
	/*是否使能周期触发功能*/
	if(periodic_trigg == TRUE)
  {
    DMAMUX->CHCFG[chx] |= DMAMUX_CHCFG_TRIG_MASK;//定期触发模式
  }
  else
  {
    DMAMUX->CHCFG[chx] &= ~(DMAMUX_CHCFG_TRIG_MASK);//正常模式
  }
	
	/*所有DMA通道中断禁止*/
	DMA0->EEI = 0; 
	
  // 设置源地址 
  DMA0->TCD[chx].SADDR = DMA_SADDR_SADDR(src_addr);
  //源地址是否增加/减少地址偏移量
  DMA0->TCD[chx].SOFF = DMA_SOFF_SOFF(src_addroffset);
  //源地址模式使能
  DMA0->TCD[chx].ATTR = 0|DMA_ATTR_SSIZE(src_dsize);
  //主的计数次数(major iteration count)到达后,是否重新调整源地址
  DMA0->TCD[chx].SLAST = DMA_SLAST_SLAST(src_lastadj);
  
	  //设置目的地址
	  DMA0->TCD[chx].DADDR = DMA_DADDR_DADDR(dst_addr);
	  //目的地址是否增加/减少地址偏移量
	  DMA0->TCD[chx].DOFF = DMA_DOFF_DOFF(dst_addroffset);
	  //设置目的地址的传输宽度
	  DMA0->TCD[chx].ATTR = 0|DMA_ATTR_DSIZE(dst_dsize);
	  //主的计数次数(major iteration count)到达后,重新调整源地址
	  DMA0->TCD[chx].DLAST_SGA = DMA_DLAST_SGA_DLASTSGA(dst_lastadj);
	  

	  if( 1 == 1)
	  {
		//设置主循环计数器 current major loop count的循环次数
		DMA0->TCD[chx].CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(major_cnt);
		//起始循环计数器，当主循环计数器减小为零的时候，重新加载起始循环计数器的值
		DMA0->TCD[chx].BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(major_cnt);
	  }
	  
	  if( 1 == 1)
	  {
		//设置每一次传输字节的个数，个数达到上限后，DMA便将数据存入内存
		DMA0->TCD[chx].NBYTES_MLNO = DMA_NBYTES_MLNO_NBYTES(minor_cnt);
	  }
	 
    DMA0->TCD[chx].CSR = 0;  //清空CSR控制器 
	
	DMA0->TCD[chx].CSR &= ~DMA_CSR_INTMAJOR_MASK;//禁用DMA主循环计数器减到0时产生中断

    /*--------------------临时加-----------------------*/
    DMA0->TCD[chx].CSR |= DMA_CSR_DREQ_MASK;//通道的ERQ位在每次传输完成后被清零
	DMA0->TCD[chx].ATTR=0;//8位传送，关闭模特性
    
  if(auto_disable == TRUE)
  {
     DMA0->TCD[chx].CSR |= DMA_CSR_DREQ_MASK; //主循环计数器减到零后，关闭DMA
  }
  else
  {
     DMA0->TCD[chx].CSR &= ~(DMA_CSR_DREQ_MASK); //主循环计数器减到零后，不关闭DMA
  } 
	
  //DMA通道使能
  DMAMUX->CHCFG[chx] |= DMAMUX_CHCFG_ENBL_MASK;
  return 1;		
}

/*************************************************************************
*				零	度	偏	移   
*  函数名称: LDPY_DMA_EnableIrq
*  功能说明: 初始化缓冲区
*  参数说明:	 在DMA发送之前给数组中加入‘帧头’
*            
*  函数返回:
*  修改时间:2014-05-19
*  备	注:
*************************************************************************/

uint8_t LDPY_DMA_EnableIrq(DMA_InitTypeDef dma_init_struct)
{
  ENABLE_IRQ((IRQn_Type)(dma_init_struct.DMA_CHx + DMA0_IRQn));  
  return 1;
}

/**********************************************************
**LDPY_DMA_DisableIrq
**禁用eDMA中断
**********************************************************/
uint8_t LDPY_DMA_DisableIrq(DMA_InitTypeDef dma_init_struct)
{
  Un_ENABLE_IRQ((IRQn_Type)(dma_init_struct.DMA_CHx + DMA0_IRQn));
  return 1;
}


/*DMA服务请求软件开始*/
__INLINE void LDPY_DMA_SoftwareStartService(DMA_InitTypeDef dma_init_struct)
{
  DMA0->TCD[dma_init_struct.DMA_CHx].CSR |= DMA_CSR_START_MASK; 
}

/*************************************************************************
*				零	度	偏	移   
*  函数名称: FrameHeader_Config
*  功能说明: DMA帧头设置
*  参数说明:	 在DMA发送之前给数组中加入‘帧头’
*            
*  函数返回: 无
*  修改时间: 2014-05-19
*  备	注:
*************************************************************************/
uint8_t FrameHeader_Config(DMA_Required_Variable *DMA_Variable_1)
{
    DMA_Variable_1->CCD_Image[0] = 'C';//CCD1的图像,1~128
	DMA_Variable_1->CCD_Image[129] = 'c';//CCD2的图像130~257		
	DMA_Variable_1->CCD_Image[258] = 'R';//259~263,左中右边界点及道路偏差
	DMA_Variable_1->CCD_Image[264] = 'r';//265~269,左中右边界点及道路偏差
	
	DMA_Variable_1->CCD_Image[270] = 'S';//271~280,舵机相关参数
	DMA_Variable_1->CCD_Image[283] = 'F';//陀螺仪采集的的AD值
	DMA_Variable_1->CCD_Image[288] = 'M';//289~296,电机相关参数
	DMA_Variable_1->CCD_Image[295] = 'w';//289~289,路况信息
	DMA_Variable_1->CCD_Image[297] = 'O';//290~290
	DMA_Variable_1->CCD_Image[298] = 'o';//291~291,O,o用来保存数据	
		
	#ifdef DMA_CH0_Enable//发送3个CCD的原始图像和偏差
	
		DMA_Variable_1->ThreeCCDs_Image[0] = 'C';//CCD1图像
		DMA_Variable_1->ThreeCCDs_Image[129] = 'c';//CCD2图像
		DMA_Variable_1->ThreeCCDs_Image[258] = 'W';//CCD3图像
		
		DMA_Variable_1->ThreeCCDs_Image[387] = 'R';//CCD1边界点和偏差
		DMA_Variable_1->ThreeCCDs_Image[396] = 'r';//CCD1边界点和偏差
		DMA_Variable_1->ThreeCCDs_Image[405] = 'I';//CCD1边界点和偏差
		
		//预留空间
		DMA_Variable_1->ThreeCCDs_Image[392] = 'x';
		DMA_Variable_1->ThreeCCDs_Image[393] = 'x';
		DMA_Variable_1->ThreeCCDs_Image[394] = 'x';
		DMA_Variable_1->ThreeCCDs_Image[395] = 'x';
		
		DMA_Variable_1->ThreeCCDs_Image[401] = 'x';
		DMA_Variable_1->ThreeCCDs_Image[402] = 'x';
		DMA_Variable_1->ThreeCCDs_Image[403] = 'x';
		DMA_Variable_1->ThreeCCDs_Image[404] = 'x';
		
		DMA_Variable_1->ThreeCCDs_Image[410] = 'x';
		DMA_Variable_1->ThreeCCDs_Image[411] = 'x';
		DMA_Variable_1->ThreeCCDs_Image[412] = 'x';
		DMA_Variable_1->ThreeCCDs_Image[413] = 'x';
		DMA_Variable_1->ThreeCCDs_Image[414] = 'x';
		DMA_Variable_1->ThreeCCDs_Image[415] = 'x';
		DMA_Variable_1->ThreeCCDs_Image[416] = 'x';
		DMA_Variable_1->ThreeCCDs_Image[417] = 'x';
		DMA_Variable_1->ThreeCCDs_Image[418] = 'x';
		DMA_Variable_1->ThreeCCDs_Image[419] = 'x';

	#endif
	
	//三个CCD的压缩图像帧头
	#ifdef DMA_CH2_SendImage_Enable
		#ifndef DMA_CH2_SendLabVIEW_Info_Enable
		
			DMA_Variable_1->CCD_Image[302] = 'G';//293~308,CCD1图像的压缩数据
			DMA_Variable_1->CCD_Image[319] = 'g';//310~325,CCD2图像的压缩数据
			DMA_Variable_1->CCD_Image[336] = 'H';//327~342,CCD3图像的压缩数据
	
			//以下为预留空间
			DMA_Variable_1->CCD_Image[353] = 'x';
			DMA_Variable_1->CCD_Image[354] = 'x';
		
		#endif
	#endif
	
	//一些地方需要发送的临时变量
	#ifdef DMA_CH2_SendLabVIEW_Info_Enable
		#ifndef DMA_CH2_SendImage_Enable
		
		//以下为预留空间
		DMA_Variable_1->CCD_Image[301] = 'M';
		DMA_Variable_1->CCD_Image[308] = 'h';
		DMA_Variable_1->CCD_Image[319] = 'd';//转向舵机临时变量帧头
		
		//以下为预留空间
		DMA_Variable_1->CCD_Image[331] = 'x';
		DMA_Variable_1->CCD_Image[332] = 'x';
		DMA_Variable_1->CCD_Image[333] = 'x';
		DMA_Variable_1->CCD_Image[334] = 'x';
		DMA_Variable_1->CCD_Image[335] = 'x';
		DMA_Variable_1->CCD_Image[336] = 'x';
		DMA_Variable_1->CCD_Image[337] = 'x';
		
		DMA_Variable_1->CCD_Image[338] = 'm';//电机临时变量帧头
		
		DMA_Variable_1->CCD_Image[350] = 'w';//路况信息
		
		//以下为预留空间
		DMA_Variable_1->CCD_Image[351] = 'x';
		DMA_Variable_1->CCD_Image[352] = 'x';
		DMA_Variable_1->CCD_Image[353] = 'x';
		DMA_Variable_1->CCD_Image[354] = 'x';
		
		#endif
	#endif
	
	DMA_Variable_1->CCD_Image[286] = 'x';
	DMA_Variable_1->CCD_Image[287] = 'x';
	
	DMA_Variable_1->CCD_Image[355] = 'O';
	DMA_Variable_1->CCD_Image[356] = 'o';

	DMA_Variable_1->CCD_Image[357] = 'x';
	
	return 1;
}

/*************************************************************************
*				零	度	偏	移  
*
*  函数名称：LDPY_DMA_SendThreeCCDsImage
*  功能说明：发送3个CCD的原始图像和偏差
*  参数说明：
*  函数返回：无
*  修改时间：2014-07-25
*  备    注：
*************************************************************************/
#ifdef DMA_CH0_Enable

void LDPY_DMA_SendThreeCCDsImage(DMA_Required_Variable *DMA_Variable,CCD_Info *CCD1_info,CCD_Info *CCD2_info,CCD_Info *CCD3_info)
{
	uint8_t ii = 0;
	
	//---------------------------------CCD图像------------------------------//	
	for(ii = 0;ii < 128; ii++)
	{
		DMA_Variable->ThreeCCDs_Image[ii + 1] = (uint8)(CCD1_info->Pixel[ii] >> 4);
		DMA_Variable->ThreeCCDs_Image[ii + 130] = (uint8)(CCD2_info->Pixel[ii] >> 4);
		DMA_Variable->ThreeCCDs_Image[ii + 259] = (uint8)(CCD3_info->Pixel[ii] >> 4);
	}

	//-------------------------------帧头R------------------------------------------//
	DMA_Variable->ThreeCCDs_Image[388] = (uint8_t)(CCD1_info->LeftLine[0] + 64);
	DMA_Variable->ThreeCCDs_Image[389] = (uint8_t)(CCD1_info->RightLine[0] + 64);
	DMA_Variable->ThreeCCDs_Image[390] = (uint8_t)(CCD1_info->CentralLine[0] + 64);
	DMA_Variable->ThreeCCDs_Image[391] = (uint8_t)(CCD1_info->LineError[0] + 64);
	
	//--------------------------------帧头r------------------------------------------------//
	DMA_Variable->ThreeCCDs_Image[397] = (uint8_t)(CCD2_info->LeftLine[0] + 64);
	DMA_Variable->ThreeCCDs_Image[398] = (uint8_t)(CCD2_info->RightLine[0] + 64);
	DMA_Variable->ThreeCCDs_Image[399] = (uint8_t)(CCD2_info->CentralLine[0] + 64);
	DMA_Variable->ThreeCCDs_Image[400] = (uint8_t)(CCD2_info->LineError[0] + 64);
	
	//--------------------------------帧头I------------------------------------------------//
	DMA_Variable->ThreeCCDs_Image[406] = (uint8_t)(CCD3_info->LeftLine[0] + 64);
	DMA_Variable->ThreeCCDs_Image[407] = (uint8_t)(CCD3_info->RightLine[0] + 64);
	DMA_Variable->ThreeCCDs_Image[408] = (uint8_t)(CCD3_info->CentralLine[0] + 64);
	DMA_Variable->ThreeCCDs_Image[409] = (uint8_t)(CCD3_info->LineError[0] + 64);

}	

#endif

/*************************************************************************
*				零	度	偏	移  
*
*  函数名称：LDPY_DMA_SendTempVar
*  功能说明：发送一些临时变量给上位机
*  参数说明：
*  函数返回：无
*  修改时间：2014-07-24
*  备    注：
*************************************************************************/
#ifdef DMA_CH2_SendLabVIEW_Info_Enable

void LDPY_DMA_SendTempVar(	LabVIEW_Info *LabVIEW_info,
							DMA_Required_Variable *DMA_Variable,
							CCD_Info *CCD1_info,
							CCD_Info *CCD2_info,
							CCD_Info *CCD3_info,
							HeadSteer_Info *HeadSteer_info,
							Steer_Info *Steer_info,
							Motor_Info *Motor_info,
							Speed_Info *Speed_info
						 )
{
	uint16_t LineError = 0;
	uint16_t HeadSteer_KP_Item = (uint16_t)(10000 + 10*LabVIEW_info->HeadSteer_KP_Item);
	uint16_t HeadSteer_KI_Item = (uint16_t)(10000 + 10*LabVIEW_info->HeadSteer_KI_Item);
	uint16_t Steer_Head_Item = (uint16_t)(10000 + 10*LabVIEW_info->Steer_Head_Item);
	uint16_t Steer_Err_Item = (uint16_t)(10000 + 10*LabVIEW_info->Steer_Err_Item);
	
	uint16_t Motor_KP_Item = (uint16_t)(10000 + 10*LabVIEW_info->Motor_KP_Item);
	uint16_t Motor_KI_Item = (uint16_t)(10000 + 10*LabVIEW_info->Motor_KI_Item);
	uint16_t Motor_KD_Item = (uint16_t)(10000 + 10*LabVIEW_info->Motor_KD_Item);
	
	switch(Speed_info->SpeedMode)
	{
		case 1:
			{
				LineError = (uint16_t)(10000 + 10*CCD1_info->LineError[0]);
			}
			break;
		case 2:
			{
				LineError = (uint16_t)(10000 + 10*CCD2_info->LineError[0]);
			}
			break;
		case 3:
			{
				LineError = (uint16_t)(10000 + 10*CCD2_info->LineError[0]);
			}
			break;
		default:
			break;
	}
	
	/*-------------------------------帧头'M'----------------7-----------------*/
	DMA_Variable->CCD_Image[302] = (uint8_t)(((uint16)(Speed_info->Speed_Now + 1000))&0x00ff);
	DMA_Variable->CCD_Image[303] = (uint8_t)((((uint16)(Speed_info->Speed_Now + 1000))>>8)&0x00ff);
	DMA_Variable->CCD_Image[304] = (uint8_t)(((uint16)(Speed_info->TargetSpeed[0] + 1000))&0x00ff);
	DMA_Variable->CCD_Image[305] = (uint8_t)((((uint16)(Speed_info->TargetSpeed[0] + 1000))>>8)&0x00ff);
	DMA_Variable->CCD_Image[306] = (uint8_t)(((uint16)(Motor_info->PWMOutputDuty + 10000))&0x00ff);
	DMA_Variable->CCD_Image[307] = (uint8_t)((((uint16)(Motor_info->PWMOutputDuty + 10000))>>8)&0x00ff);
	
	//----------------------------帧头‘h’---------11--------------//
	DMA_Variable->CCD_Image[309] = (uint8_t)(LineError & 0x00ff);
	DMA_Variable->CCD_Image[310] = (uint8_t)((LineError >> 8) & 0x00ff);
	
	DMA_Variable->CCD_Image[311] = (uint8_t)(HeadSteer_KP_Item & 0x00ff);
	DMA_Variable->CCD_Image[312] = (uint8_t)((HeadSteer_KP_Item >> 8) & 0x00ff);
	DMA_Variable->CCD_Image[313] = 10;			//放大倍数为10
	
	DMA_Variable->CCD_Image[314] = (uint8_t)(HeadSteer_KI_Item & 0x00ff);
	DMA_Variable->CCD_Image[315] = (uint8_t)((HeadSteer_KI_Item >> 8) & 0x00ff);
	DMA_Variable->CCD_Image[316] = 10;			//放大倍数为10
	
	DMA_Variable->CCD_Image[317] = (uint8_t)((uint16_t)HeadSteer_info->Steer_PWMOutputDuty & 0x00ff);
	DMA_Variable->CCD_Image[318] = (uint8_t)(((uint16_t)HeadSteer_info->Steer_PWMOutputDuty >> 8) & 0x00ff);
	
	//----------------------------帧头‘d’--------------12---------//
	DMA_Variable->CCD_Image[320] = (uint8_t)(Speed_info->SpeedMode);//发送当前档位
	
	DMA_Variable->CCD_Image[321] = (uint8_t)(LineError & 0x00ff);
	DMA_Variable->CCD_Image[322] = (uint8_t)((LineError >> 8) & 0x00ff);
	
	DMA_Variable->CCD_Image[323] = (uint8_t)(Steer_Head_Item & 0x00ff);
	DMA_Variable->CCD_Image[324] = (uint8_t)((Steer_Head_Item >> 8) & 0x00ff);
	DMA_Variable->CCD_Image[325] = 10;			//放大倍数为10
	
	DMA_Variable->CCD_Image[326] = (uint8_t)(Steer_Err_Item & 0x00ff);
	DMA_Variable->CCD_Image[327] = (uint8_t)((Steer_Err_Item >> 8) & 0x00ff);
	DMA_Variable->CCD_Image[328] = 10;			//放大倍数为10
	
	DMA_Variable->CCD_Image[329] = (uint8_t)((uint16_t)Steer_info->Steer_PWMOutputDuty & 0x00ff);
	DMA_Variable->CCD_Image[330] = (uint8_t)(((uint16_t)Steer_info->Steer_PWMOutputDuty >> 8) & 0x00ff);
	
	//----------------------------帧头‘m’----------12-------------//
	DMA_Variable->CCD_Image[339] = (uint8_t)(Motor_KP_Item & 0x00ff);
	DMA_Variable->CCD_Image[340] = (uint8_t)((Motor_KP_Item >> 8) & 0x00ff);
	DMA_Variable->CCD_Image[341] = 10;			//放大倍数为10
	
	DMA_Variable->CCD_Image[342] = (uint8_t)(Motor_KI_Item & 0x00ff);
	DMA_Variable->CCD_Image[343] = (uint8_t)((Motor_KI_Item >> 8) & 0x00ff);
	DMA_Variable->CCD_Image[344] = 10;			//放大倍数为10
	
	DMA_Variable->CCD_Image[345] = (uint8_t)(Motor_KD_Item & 0x00ff);
	DMA_Variable->CCD_Image[346] = (uint8_t)((Motor_KD_Item >> 8) & 0x00ff);
	DMA_Variable->CCD_Image[347] = 10;			//放大倍数为10
	
	DMA_Variable->CCD_Image[348] = (uint8_t)((uint16_t)(10000 + Motor_info->PWMOutputDuty) & 0x00ff);
	DMA_Variable->CCD_Image[349] = (uint8_t)(((uint16_t)(10000 + Motor_info->PWMOutputDuty) >> 8) & 0x00ff);
	
	/*-------------------------------帧头'w'----------------2-----------------*/
	DMA_Variable->CCD_Image[351] = (uint8_t)(Road_condition_Item);
}

#endif

/*************************************************************************
*				零	度	偏	移  
*
*  函数名称：LDPY_DMA_UpdateFifo_Parameters
*  功能说明：发送CCD的相关数据给上位机,发送速度，舵机参数，电机参数，不发送波形
*  参数说明：
*  函数返回：无
*  修改时间：2014-05-24
*  备    注：
*************************************************************************/

void LDPY_DMA_UpdateFifo_Parameters(CCD_Info *CCD1_info,Steer_Info *Steer_info,\
									Motor_Info *Motor_info,Speed_Info *Speed_info,\
									Road_Condition *Road_condition)
{
		/*--------------------------帧头'S'-----------------------------------*/
		DMA_Variable_1.CCD_Image[271] = (uint8_t)(((uint16)Steer_info->Steer_PWMOutputDuty)&0xff);
		DMA_Variable_1.CCD_Image[272] = (uint8_t)((((uint16)Steer_info->Steer_PWMOutputDuty)>>8)&0x00ff);
		DMA_Variable_1.CCD_Image[273] = (uint8_t)(((uint16)(CCD1_info->LineError[0] + 1000))&0x00ff);
		DMA_Variable_1.CCD_Image[274] = (uint8_t)((((uint16)(CCD1_info->LineError[0] + 1000))>>8)&0x00ff);
	
	    DMA_Variable_1.CCD_Image[275] = (uint8_t)(((uint16)CCD1_info->LeftLine[0] + 1000)&0xff);
		DMA_Variable_1.CCD_Image[276] = (uint8_t)((((uint16)CCD1_info->LeftLine[0] + 1000)>>8)&0x00ff);
	    DMA_Variable_1.CCD_Image[277] = (uint8_t)(((uint16)CCD1_info->CentralLine[0] + 1000)&0xff);
		DMA_Variable_1.CCD_Image[278] = (uint8_t)((((uint16)CCD1_info->CentralLine[0] + 1000)>>8)&0x00ff);	
	    DMA_Variable_1.CCD_Image[279] = (uint8_t)(((uint16)CCD1_info->RightLine[0] + 1000)&0xff);
		DMA_Variable_1.CCD_Image[280] = (uint8_t)((((uint16)CCD1_info->RightLine[0] + 1000)>>8)&0x00ff);	
	
		DMA_Variable_1.CCD_Image[281] = 'x';
		DMA_Variable_1.CCD_Image[282] = 'x';
	
		/*--------------------------帧头'F'-----------------------------------*/
		DMA_Variable_1.CCD_Image[284] = (uint8_t)((Gyro_info.Gyroscope_Fifo[0])&0xff);
		DMA_Variable_1.CCD_Image[285] = (uint8_t)(((Gyro_info.Gyroscope_Fifo[0])>>8)&0x00ff);
	
		/*-------------------------------帧头'M'---------------------------------*/
		DMA_Variable_1.CCD_Image[289] = (uint8_t)(((uint16)(Speed_info->Speed_Now + 1000))&0x00ff);
		DMA_Variable_1.CCD_Image[290] = (uint8_t)((((uint16)(Speed_info->Speed_Now + 1000))>>8)&0x00ff);
		DMA_Variable_1.CCD_Image[291] = (uint8_t)(((uint16)(Speed_info->TargetSpeed[0] + 1000))&0x00ff);
		DMA_Variable_1.CCD_Image[292] = (uint8_t)((((uint16)(Speed_info->TargetSpeed[0] + 1000))>>8)&0x00ff);
		DMA_Variable_1.CCD_Image[293] = (uint8_t)(((uint16)(Motor_info->PWMOutputDuty + 10000))&0x00ff);
		DMA_Variable_1.CCD_Image[294] = (uint8_t)((((uint16)(Motor_info->PWMOutputDuty + 10000))>>8)&0x00ff);
		
		/*-----------------------------帧头'w'-------------------------------------*/
		DMA_Variable_1.CCD_Image[296] = *Road_condition;
}

/*************************************************************************
*				零	度	偏	移  
*
*  函数名称：LDPY_DMA_UpdateFifo_PhotoValue
*  功能说明：发送三个CCD压缩后的图像数据
*  参数说明：
*  函数返回：无
*  修改时间：2014-06-08
*  备    注：
*************************************************************************/

void LDPY_DMA_UpdateFifo_PhotoValue(CCD_Info *CCD1_info,CCD_Info *CCD2_info,CCD_Info *CCD3_info)
{
	uint8 ii = 0;
	
	//压缩CCD二值化后的数据
	for(ii = 0;ii < 16;ii ++)
	{
		CCD1_info->CCD_PhotoValue[ii] = 	(CCD1_info->PixelBinary[7 + 8*ii] << 7)
										  | (CCD1_info->PixelBinary[6 + 8*ii] << 6)
										  | (CCD1_info->PixelBinary[5 + 8*ii] << 5)
										  | (CCD1_info->PixelBinary[4 + 8*ii] << 4)
										  | (CCD1_info->PixelBinary[3 + 8*ii] << 3)
										  | (CCD1_info->PixelBinary[2 + 8*ii] << 2)
										  | (CCD1_info->PixelBinary[1 + 8*ii] << 1)
										  | (CCD1_info->PixelBinary[0 + 8*ii])
										;
	
		CCD2_info->CCD_PhotoValue[ii] = 	(CCD2_info->PixelBinary[7 + 8*ii] << 7)
										  | (CCD2_info->PixelBinary[6 + 8*ii] << 6)
										  | (CCD2_info->PixelBinary[5 + 8*ii] << 5)
										  | (CCD2_info->PixelBinary[4 + 8*ii] << 4)
										  | (CCD2_info->PixelBinary[3 + 8*ii] << 3)
										  | (CCD2_info->PixelBinary[2 + 8*ii] << 2)
										  | (CCD2_info->PixelBinary[1 + 8*ii] << 1)
										  | (CCD2_info->PixelBinary[0 + 8*ii])
										;
		
		CCD3_info->CCD_PhotoValue[ii] = 	(CCD3_info->PixelBinary[7 + 8*ii] << 7)
										  | (CCD3_info->PixelBinary[6 + 8*ii] << 6)
										  | (CCD3_info->PixelBinary[5 + 8*ii] << 5)
										  | (CCD3_info->PixelBinary[4 + 8*ii] << 4)
										  | (CCD3_info->PixelBinary[3 + 8*ii] << 3)
										  | (CCD3_info->PixelBinary[2 + 8*ii] << 2)
										  | (CCD3_info->PixelBinary[1 + 8*ii] << 1)
										  | (CCD3_info->PixelBinary[0 + 8*ii])
										;
	}
	
	/*---------------帧头‘G’---------------------*/
	DMA_Variable_1.CCD_Image[303] = CCD1_info->CCD_PhotoValue[0];
	DMA_Variable_1.CCD_Image[304] = CCD1_info->CCD_PhotoValue[1];
	DMA_Variable_1.CCD_Image[305] = CCD1_info->CCD_PhotoValue[2];
	DMA_Variable_1.CCD_Image[306] = CCD1_info->CCD_PhotoValue[3];
	DMA_Variable_1.CCD_Image[307] = CCD1_info->CCD_PhotoValue[4];
	DMA_Variable_1.CCD_Image[308] = CCD1_info->CCD_PhotoValue[5];
	DMA_Variable_1.CCD_Image[309] = CCD1_info->CCD_PhotoValue[6];
	DMA_Variable_1.CCD_Image[310] = CCD1_info->CCD_PhotoValue[7];
	DMA_Variable_1.CCD_Image[311] = CCD1_info->CCD_PhotoValue[8];
	DMA_Variable_1.CCD_Image[312] = CCD1_info->CCD_PhotoValue[9];
	DMA_Variable_1.CCD_Image[313] = CCD1_info->CCD_PhotoValue[10];
	DMA_Variable_1.CCD_Image[314] = CCD1_info->CCD_PhotoValue[11];
	DMA_Variable_1.CCD_Image[315] = CCD1_info->CCD_PhotoValue[12];
	DMA_Variable_1.CCD_Image[316] = CCD1_info->CCD_PhotoValue[13];
	DMA_Variable_1.CCD_Image[317] = CCD1_info->CCD_PhotoValue[14];
	DMA_Variable_1.CCD_Image[318] = CCD1_info->CCD_PhotoValue[15];	
	
	/*---------------帧头‘g’---------------------*/
	DMA_Variable_1.CCD_Image[320] = CCD2_info->CCD_PhotoValue[0];
	DMA_Variable_1.CCD_Image[321] = CCD2_info->CCD_PhotoValue[1];
	DMA_Variable_1.CCD_Image[322] = CCD2_info->CCD_PhotoValue[2];
	DMA_Variable_1.CCD_Image[323] = CCD2_info->CCD_PhotoValue[3];
	DMA_Variable_1.CCD_Image[324] = CCD2_info->CCD_PhotoValue[4];
	DMA_Variable_1.CCD_Image[325] = CCD2_info->CCD_PhotoValue[5];
	DMA_Variable_1.CCD_Image[326] = CCD2_info->CCD_PhotoValue[6];
	DMA_Variable_1.CCD_Image[327] = CCD2_info->CCD_PhotoValue[7];
	DMA_Variable_1.CCD_Image[328] = CCD2_info->CCD_PhotoValue[8];
	DMA_Variable_1.CCD_Image[329] = CCD2_info->CCD_PhotoValue[9];
	DMA_Variable_1.CCD_Image[330] = CCD2_info->CCD_PhotoValue[10];
	DMA_Variable_1.CCD_Image[331] = CCD2_info->CCD_PhotoValue[11];
	DMA_Variable_1.CCD_Image[332] = CCD2_info->CCD_PhotoValue[12];
	DMA_Variable_1.CCD_Image[333] = CCD2_info->CCD_PhotoValue[13];
	DMA_Variable_1.CCD_Image[334] = CCD2_info->CCD_PhotoValue[14];
	DMA_Variable_1.CCD_Image[335] = CCD2_info->CCD_PhotoValue[15];
	
	/*---------------帧头‘H’---------------------*/
	DMA_Variable_1.CCD_Image[337] = CCD3_info->CCD_PhotoValue[0];
	DMA_Variable_1.CCD_Image[338] = CCD3_info->CCD_PhotoValue[1];
	DMA_Variable_1.CCD_Image[339] = CCD3_info->CCD_PhotoValue[2];
	DMA_Variable_1.CCD_Image[340] = CCD3_info->CCD_PhotoValue[3];
	DMA_Variable_1.CCD_Image[341] = CCD3_info->CCD_PhotoValue[4];
	DMA_Variable_1.CCD_Image[342] = CCD3_info->CCD_PhotoValue[5];
	DMA_Variable_1.CCD_Image[343] = CCD3_info->CCD_PhotoValue[6];
	DMA_Variable_1.CCD_Image[344] = CCD3_info->CCD_PhotoValue[7];
	DMA_Variable_1.CCD_Image[345] = CCD3_info->CCD_PhotoValue[8];
	DMA_Variable_1.CCD_Image[346] = CCD3_info->CCD_PhotoValue[9];
	DMA_Variable_1.CCD_Image[347] = CCD3_info->CCD_PhotoValue[10];
	DMA_Variable_1.CCD_Image[348] = CCD3_info->CCD_PhotoValue[11];
	DMA_Variable_1.CCD_Image[349] = CCD3_info->CCD_PhotoValue[12];
	DMA_Variable_1.CCD_Image[350] = CCD3_info->CCD_PhotoValue[13];
	DMA_Variable_1.CCD_Image[351] = CCD3_info->CCD_PhotoValue[14];
	DMA_Variable_1.CCD_Image[352] = CCD3_info->CCD_PhotoValue[15];
}

/*************************************************************************
*				零	度	偏	移  
*
*  函数名称：DMA_InitAndChoice
*  功能说明：发送方式选择
*  参数说明：CH0发送波形,CH1发送参数,CH3发送压缩图像
*  函数返回：1
*  修改时间：2014-06-08
*  备    注：CH0发送波形,CH1发送参数,CH3发送压缩图像
*************************************************************************/
uint8_t LDPY_DMA_InitAndChoice(DMA_InitTypeDef  *dma_init_struct,DMA_Required_Variable *DMA_Required_Variable,\
                          CCD_Info *CCD_info)
{
	#ifdef  DMA_CH0_Enable
	
		dma_init_struct->DMA_CHx = DMA_CH0; 	
		dma_init_struct->DMA_SourceAddr = (uint32_t)&DMA_Required_Variable->ThreeCCDs_Image[0];
		dma_init_struct->DMA_LastSourceAddrAdj = -420; 
		dma_init_struct->DMA_MajorLoopCnt = 420;
		dma_init_struct->DMA_MinorByteCnt = 1;//副循环传输字节数
		dma_init_struct->DMA_BufferSize = 420;

	    /*----------------临时加-------------------*/
	    dma_init_struct->DMA_AutoClose = TRUE;
		dma_init_struct->DMA_AutoDisableReq = TRUE;
	
		FrameHeader_Config(&DMA_Variable_1);
        LDPY_DMA_StructInit(dma_init_struct,CCD_info,DMA_Required_Variable);
        LDPY_DMA_init(dma_init_struct);
	
	#endif
	
	#ifdef	 DMA_CH1_Enable
	
	    dma_init_struct->DMA_CHx = DMA_CH1;
		dma_init_struct->DMA_SourceAddr = (uint32_t)&DMA_Required_Variable->CCD_Image[270];
		dma_init_struct->DMA_LastSourceAddrAdj = -27;
		dma_init_struct->DMA_MajorLoopCnt = 27;
		dma_init_struct->DMA_MinorByteCnt = 1;//副循环传输字节数
		dma_init_struct->DMA_BufferSize = 27;
		
		/*----------------临时加-------------------*/
		dma_init_struct->DMA_AutoClose = TRUE;//主循环计数器减少到零时关闭DMA
		dma_init_struct->DMA_AutoDisableReq = TRUE;

		FrameHeader_Config(&DMA_Variable_1);
        LDPY_DMA_StructInit(dma_init_struct,CCD_info,DMA_Required_Variable);
        LDPY_DMA_init(dma_init_struct);
	
	#endif
    
    #ifdef  DMA_CH2_Enable
		
	    dma_init_struct->DMA_CHx = DMA_CH2;

		dma_init_struct->DMA_SourceAddr = (uint32_t)&DMA_Required_Variable->CCD_Image[301];
		dma_init_struct->DMA_LastSourceAddrAdj = -51;
		dma_init_struct->DMA_MajorLoopCnt = 51;
		dma_init_struct->DMA_MinorByteCnt = 1;//副循环传输字节数
		dma_init_struct->DMA_BufferSize = 51;

		
		/*----------------临时加-------------------*/
		dma_init_struct->DMA_AutoClose = TRUE;//主循环计数器减少到零时关闭DMA
		dma_init_struct->DMA_AutoDisableReq = TRUE;

		FrameHeader_Config(&DMA_Variable_1);
        LDPY_DMA_StructInit(dma_init_struct,CCD_info,DMA_Required_Variable);
        LDPY_DMA_init(dma_init_struct);
		
	#endif
	
    return 1;
}


