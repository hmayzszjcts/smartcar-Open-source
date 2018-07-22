#ifndef _DEBUG_H_
#define _DEBUG_H_
#define true  1
#define false 0
//将变量名转化为字符串
#define  VNAME(name) (#name)
//取一个数据的各个位
#define BYTE0(Temp)       (*(char *)(&Temp))     
#define BYTE1(Temp)       (*((char *)(&Temp) + 1))
#define BYTE2(Temp)       (*((char *)(&Temp) + 2))
#define BYTE3(Temp)       (*((char *)(&Temp) + 3))



#define TEST_HIGH        PTE26_OUT=1
#define TEST_LOW         PTE26_OUT=0


#define Voltage_Ratio 0.00252297  //定义电压转换比率

extern uint8 Starting;
extern int  Start_Cnt;
extern  uint8 Page_Index,Para_Index,Para_Checked,OLED_Refresh,Light_Tower_Index,light_tower_detected;
extern  float *Control_Para[16];
extern  float *Variable[20];
extern  uint8 CCD_Draw_Buff[150];
extern  float Voltage;
extern  uint8 Stop,CarStopedJustNow;
extern  uint8 Debug_Index;
extern  uint8 Debug_Motor_ON;
extern  uint8 SendPara,SendCCD,SendSD,Uart_Send;
extern  uint8 OverWrite;
void    Check_BottonPress();
void    Check_StartPress();
void    Modify_Para(uint8 *buff);
void    Send_Para();
void    Send_Variable();
void    Send_SD();
void    Send_Inductor_Adc();
void    Send_CCD();
void    OLED_Draw_UI();  //画出界面
void    Read_Switch(); 
void    UART0_RX_IRQHandler(void);
#endif