
#include "myFlash.h"

Flash_Info Flash_info={0};

/*************************************************************************
*				零	度	偏	移   
*  函数名称: myFlash_WritePara2Flash
*  功能说明: 写入Flash
*  参数说明:	
*            
*  函数返回:
*  修改时间:2014-05-14
*  备	注:	该函数会将更改的参数写入flash
*		由于flash写前要擦除，所以先读出参数和传感数据再写入
***************************************************************************/
void myFlash_WritePara2Flash(
							  Car_State * Car_state,
							  Parameter_Info * Parameter_info,
							  CCD_Info * CCD1_info,
							  CCD_Info * CCD2_info,
							  Steer_Info * Steer1_info,
							  Steer_Info * Steer2_info,
							  Speed_Info * Speed1_info,
							  Speed_Info * Speed2_info,
							  Flash_Info * Flash_info,
							  Motor_Info * Motor1_info,
							  Motor_Info * Motor2_info
							  )

{
  uint8 i=0;
  float Flash_Array_1[10]={0};
  float Flash_Array_2[10]={0};
  float Flash_Array_3[10]={0};
  
  LDPY_Flash_SectorErase(FLASH_ARRAY_ADDR_1);    //依次擦除相关地址Flash
  LDPY_Flash_SectorErase(FLASH_ARRAY_ADDR_2);
  LDPY_Flash_SectorErase(FLASH_ARRAY_ADDR_3);
  
  Flash_info->Flash_Array_1[0]=Parameter_info->CCD_IntegrationTime;
//  Flash_info->Flash_Array_1[1]=CCD1_info->SearchIntervalNum;
//  Flash_info->Flash_Array_1[2]=CCD2_info->SearchIntervalNum;
  Flash_info->Flash_Array_1[3]=Steer1_info->Steer_Center;
  Flash_info->Flash_Array_1[4]=Steer2_info->Steer_Center;
  
  
  Flash_info->Flash_Array_1[5]=Speed1_info->Straight_Speed;
  Flash_info->Flash_Array_1[6]=Speed2_info->Straight_Speed;
  Flash_info->Flash_Array_1[7]=Speed1_info->Cur_Speed;
  Flash_info->Flash_Array_1[8]=Speed2_info->Cur_Speed;
  
  Flash_info->Flash_Array_1[9]=Speed1_info->Ramp_Down_Speed;
  Flash_info->Flash_Array_2[0]=Speed1_info->Snake_Speed;
  Flash_info->Flash_Array_2[1]=Speed1_info->TurnRound_Speed;
  Flash_info->Flash_Array_2[2]=Speed2_info->Ramp_Down_Speed;
  Flash_info->Flash_Array_2[3]=Speed2_info->Snake_Speed;
  Flash_info->Flash_Array_2[4]=Speed2_info->TurnRound_Speed;
  
  
                        
  Flash_info->Flash_Array_2[5]=CCD1_info->RoadWidth;  //存入道路宽度    gwd 2014-04-12
  Flash_info->Flash_Array_2[6]=CCD2_info->RoadWidth;

  Flash_info->Flash_Array_2[7]=Steer1_info->KP;
  Flash_info->Flash_Array_2[8]=Steer1_info->KD;
  Flash_info->Flash_Array_2[9]=Motor1_info->KP;
  Flash_info->Flash_Array_3[0]=Motor1_info->KI;
  Flash_info->Flash_Array_3[1]=Motor1_info->KD;
  Flash_info->Flash_Array_3[2]=Steer2_info->KP;
  Flash_info->Flash_Array_3[3]=Steer2_info->KD;
  Flash_info->Flash_Array_3[4]=Motor2_info->KP;
  Flash_info->Flash_Array_3[5]=Motor2_info->KI;
  Flash_info->Flash_Array_3[6]=Motor2_info->KD;
  
  Flash_info->Flash_Array_3[7]=CCD1_info->TurnRoundDirec_Flag;
//Flash_info->Flash_Array[2]=CCD_info->LeftLinePixel;
//Flash_info->Flash_Array[3]=CCD_info->MidLinePixel;
//Flash_info->Flash_Array[4]=CCD_info->RightLinePixel;
  for(i=0;i<10;i++)                                 //赋值
    {
      Flash_Array_1[i]=Flash_info->Flash_Array_1[i];
    }
  
  for(i=0;i<10;i++)
    {
      Flash_Array_2[i]=Flash_info->Flash_Array_2[i];
    }
  
  for(i=0;i<10;i++)
    {
      Flash_Array_3[i]=Flash_info->Flash_Array_3[i];
    }
    
  LDPY_Flash_ByteProgram(                                        //写入
                           (uint32)(FLASH_ARRAY_ADDR_1 ), 
                           (uint32*)(Flash_Array_1),
                           FLASH_ARRAY_SIZE
                         );
  
  LDPY_Flash_ByteProgram(
                           (uint32)(FLASH_ARRAY_ADDR_2 ), 
                           (uint32*)(Flash_Array_2),
                            FLASH_ARRAY_SIZE
                         );
  LDPY_Flash_ByteProgram(
                           (uint32)(FLASH_ARRAY_ADDR_3 ), 
                           (uint32*)(Flash_Array_3),
                           FLASH_ARRAY_SIZE
                         );
}

/*************************************************************************
*				零	度	偏	移   
*  函数名称: myFlash_ReadFlash
*  功能说明: 读取Flash存储值
*  参数说明:	
*            
*  函数返回:
*  修改时间:2014-05-14
*  备	注:	
***************************************************************************/
void myFlash_ReadFlash(   Car_State * Car_state,
						  Parameter_Info * Parameter_info,
						  CCD_Info * CCD1_info,
						  CCD_Info * CCD2_info,
						  Steer_Info * Steer1_info,
						  Steer_Info * Steer2_info,
						  Speed_Info * Speed1_info,
						  Speed_Info * Speed2_info,
						  Flash_Info * Flash_info,
						  Motor_Info * Motor1_info,
						  Motor_Info * Motor2_info  
					  )
						  
{
  uint8 i;
  float * ptr_1 = (float*)FLASH_ARRAY_ADDR_1;   //将起始地址指针变量赋给ptr_1
  float * ptr_2 = (float*)FLASH_ARRAY_ADDR_2;   //将起始地址指针变量赋给ptr_2 
  float * ptr_3 = (float*)FLASH_ARRAY_ADDR_3;   //将起始地址指针变量赋给ptr_3
 
  
  for(i = 0;i < 10 ;i++)                    //float型的数据每次加i，地址加四位
    {                                       //只有10个数据，所以i为10就可以 
      *((float*)Flash_info->Flash_Array_1 + i) = *(ptr_1 + i);     //Flash_Array数组依次得到对应的地址，地址找到了就读出来了
    }
  
  for(i = 0;i < 10 ;i++)                  //float型的数据每次加i，地址加四位
    {
      *((float*)Flash_info->Flash_Array_2 + i) = *(ptr_2 + i);     //Flash_Array数组依次得到对应的地址，地址找到了就读出来了
    }

  for(i = 0;i < 10 ;i++)                    //float型的数据每次加i，地址加四位
    {
      *((float*)Flash_info->Flash_Array_3 + i) = *(ptr_3 + i);     //Flash_Array数组依次得到对应的地址，地址找到了就读出来了
    }
  
  
  Parameter_info->CCD_IntegrationTime=(uint32)Flash_info->Flash_Array_1[0];    //赋值
//  CCD1_info->SearchIntervalNum=(uint8)Flash_info->Flash_Array_1[1];
//  CCD2_info->SearchIntervalNum=(uint8)Flash_info->Flash_Array_1[2];
  Steer1_info->Steer_Center=(uint32)Flash_info->Flash_Array_1[3];
  Steer2_info->Steer_Center=(uint32)Flash_info->Flash_Array_1[4];
  
  
  Speed1_info->Straight_Speed=(uint16)Flash_info->Flash_Array_1[5];
  Speed2_info->Straight_Speed=(uint16)Flash_info->Flash_Array_1[6];
  Speed1_info->Cur_Speed=(uint16)Flash_info->Flash_Array_1[7];
  Speed2_info->Cur_Speed=(uint16)Flash_info->Flash_Array_1[8];
  
  Speed1_info->Ramp_Down_Speed=(uint16)Flash_info->Flash_Array_1[9];
  Speed1_info->Snake_Speed=(uint16)Flash_info->Flash_Array_2[0];
  Speed1_info->TurnRound_Speed=(uint16)Flash_info->Flash_Array_2[1];
  Speed2_info->Ramp_Down_Speed=(uint16)Flash_info->Flash_Array_2[2];
  Speed2_info->Snake_Speed=(uint16)Flash_info->Flash_Array_2[3];
  Speed2_info->TurnRound_Speed=(uint16)Flash_info->Flash_Array_2[4];
  
  
                        
  CCD1_info->RoadWidth=(uint8)Flash_info->Flash_Array_2[5];  //存入道路宽度    
  CCD2_info->RoadWidth=(uint8)Flash_info->Flash_Array_2[6];

  Steer1_info->KP=Flash_info->Flash_Array_2[7];
  Steer1_info->KD=Flash_info->Flash_Array_2[8];
  Motor1_info->KP=Flash_info->Flash_Array_2[9];
  Motor1_info->KI=Flash_info->Flash_Array_3[0];
  Motor1_info->KD=Flash_info->Flash_Array_3[1];
  Steer2_info->KP=Flash_info->Flash_Array_3[2];
  Steer2_info->KD=Flash_info->Flash_Array_3[3];
  Motor2_info->KP=Flash_info->Flash_Array_3[4];
  Motor2_info->KI=Flash_info->Flash_Array_3[5];
  Motor2_info->KD=Flash_info->Flash_Array_3[6];
  
  CCD1_info->TurnRoundDirec_Flag=(uint8)Flash_info->Flash_Array_3[7];
}


/*-------------------------------end of myFlash.c--------------------------------*/
