#include "include.h"


/*************************************************************************
*  函数名称：VSYNC_IRQ
*  功能说明：PORTD端口中断服务函数
*  参数说明：无
*  函数返回：无
*  修改时间：2012-1-25    已测试
*  备    注：引脚号需要自己初始化来清除
*************************************************************************/
void VSYNC_IRQ(void)
{    
    static u16 czd=0;
    static u32 flag;
    //Clear Interrupt flag
    flag = PORTD_ISFR;
    PORTD_ISFR = flag;
    czd++;
    if(img_flag == IMG_START)	//需要开始采集图像
    {
        DMA_PORTx2BUFF_Init(CAMERA_DMA_CH, (void *)&PTB_BYTE2_IN, (void *)Image_fire, PTE27, DMA_BYTE1, CAMERA_SIZE , DMA_falling);
        DMA_EN(CAMERA_DMA_CH);            		//使能通道CHn 硬件请求
        DMA_DADDR(CAMERA_DMA_CH) = (u32)Image_fire; //恢复地址
        img_flag = IMG_GATHER;		        //标记图像采集中
        disable_irq(90);  
    }
    else					//图像采集错误
    {
        disable_irq(90); 			//关闭PTA的中断
        img_flag = IMG_FAIL;		//标记图像采集失败
    }
}




/*************************************************************************
*  函数名称：DMA0_IRQHandler
*  功能说明：DMA0
*  参数说明：无
*  函数返回：无
*  修改时间：2012-1-25    已测试
*  备    注：引脚号需要根据自己初始化来修改
*************************************************************************/
void DMA0_IRQHandler()
{
    DMA_DIS(CAMERA_DMA_CH);            	//关闭通道CHn 硬件请求
    DMA_IRQ_CLEAN(CAMERA_DMA_CH);           //清除通道传输中断标志位
    img_flag = IMG_FINISH ;  
}






/*************************************************************************
*  函数名称：PIT0_IRQHandler
*  功能说明：PIT0 定时中断服务函数
*  参数说明：无
*  函数返回：无
*  修改时间：2014-1-14    已测试
*  备    注：
*************************************************************************/
void PIT0_IRQHandler(void)
{
    static unsigned char TimeCnt_20ms = 0;	  //20ms时间计数器
    static unsigned char flag = 0;                //5ms的分时标志
    PIT_Flag_Clear(PIT0);
    Time_1ms++; 
    _LED1 = (Time_1ms/500)%2;
    TimeCnt_20ms++;
    if(TimeCnt_20ms >= 20)
    {TimeCnt_20ms = 0;}
    flag = TimeCnt_20ms%5;
    //到此为止T=0.5us
    /**************************************************************/
    /*********************Own code*********************************/
    /**************************************************************/   

    
    //速度控制 Tmax=8.5us
    if(flag==0 && Time_1ms>Stand_Time)
    {
        Measure_speed();
        Speed_control();
    }
    //计算angle_Out，并且直立控制 T=449us IIC时间长
    else if(flag==1)
    {
        Measure_Acc();//T=340us
        Angle_control();//T=100us
        Motor_Control();//T=9us
    }
    else if(Ren.flag==1 && flag==2)//有人字的方向输出
    {
        Direction_control_with_Ren();//T=58us
        Motor_Control();
    }
    //方向输出
    if(Ren.flag==0 && Direction.output_enable==1)
    {
        Direction_control();//T=54us
        Motor_Control();
        Direction.output_enable = 0;
    }
    
    
    //加速阶段 T=3us，直接就达到设定值
    if(Time_1ms > Stand_Time && Speed.Goal < Speed.Goal_init)
    { 
        Speed.Goal = Speed.Goal_init + 1;
        buzzer = 1;
    }
    else
    {
        if(Ren.flag==1 || Cross_flag==1 || Slope_flag==1 || Time_1ms < Block.time + 1000 || smallS_flag==1)
            buzzer = 1;
        else
            buzzer = 0;
    }
    
    //LED流水灯，证明在进定时器中断
    //Water_LEDs();
    /*********************************************************************/
    /**************************Own code end*******************************/
    /*********************************************************************/
    PIT_Flag_Clear(PIT0);       //清中断标志位
}







/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：USART3_IRQHandler
*  功能说明：串口3 中断 接收 服务函数
*  参数说明：无
*  函数返回：无
*  修改时间：2014-1-14    已测试
*  备    注：
*************************************************************************/
void USART3_IRQHandler(void)
{
    unsigned char ch;
    //接收一个字节数据并回发
    ch = uart_getchar (UART3);      //接收到一个数据
    switch(ch)
    {
//      case 'a':break;
//      case 'b': Angle.P+=10;  printf("angle_P:");SCISendFloat(UART3, Angle.P);break;
//      case 'c': Angle.P-=10;  printf("angle_P:");SCISendFloat(UART3, Angle.P);break;
//      case 'd': Angle.D+=10;  printf("angle_D:");SCISendFloat(UART3, Angle.D);break;
//      case 'e': Angle.D-=10;  printf("angle_D:");SCISendFloat(UART3, Angle.D);break;
//      case 'f': Angle.D+=1;   printf("angle_D:");SCISendFloat(UART3, Angle.D);break;
//      case 'g': Angle.D-=1;   printf("angle_D:");SCISendFloat(UART3, Angle.D);break;
//      case '0': Angle.OFFSET+=1;  printf("OFFSET_angle:");SCISendFloat(UART3, Angle.OFFSET);break;
//      case '1': Angle.OFFSET-=1;  printf("OFFSET_angle:");SCISendFloat(UART3, Angle.OFFSET);break;
//      case '2': Angle.OFFSET+=0.1;printf("OFFSET_angle:");SCISendFloat(UART3, Angle.OFFSET);break;
//      case '3': Angle.OFFSET-=0.1;printf("OFFSET_angle:");SCISendFloat(UART3, Angle.OFFSET);break;
//      //case '4':tg+=1;printf("tg:");SCISendFloat(UART3,tg);break;
      //case '5':tg-=1;printf("tg:");SCISendFloat(UART3,tg);break;
      //胡乱添加的。。
      case 'a':Speed.P+=0.2; printf("speed_P:");SCISendFloat(UART3,Speed.P);break;
      case 'b':Speed.P-=0.2; printf("speed_P:");SCISendFloat(UART3,Speed.P);break;
      case 'e':Speed.I+=0.1;  printf("speed_I:");SCISendFloat(UART3,Speed.I);break;
      case 'f':Speed.I-=0.1;  printf("speed_I:");SCISendFloat(UART3,Speed.I);break;
      case 'g':Speed.I+=0.01;  printf("speed_I:");SCISendFloat(UART3,Speed.I);break;
      case 'h':Speed.I-=0.01;  printf("speed_I:");SCISendFloat(UART3,Speed.I);break;
      //case 'j':SD_enable=1;StartLine.flag=1;break;
//      case '6':pwm_Dead_L+=10;printf("pwm_Dead_L:");SCISendFloat(UART3,pwm_Dead_L);break;
//      case '7':pwm_Dead_L-=10;printf("pwm_Dead_L:");SCISendFloat(UART3,pwm_Dead_L);break;
//      case '8':pwm_Dead_R+=10;printf("pwm_Dead_R:");SCISendFloat(UART3,pwm_Dead_R);break;
//      case '9':pwm_Dead_R-=10;printf("pwm_Dead_R:");SCISendFloat(UART3,pwm_Dead_R);break;
      //default: printf("error");
    }
    uart_putchar(UART3,'\n');
}




/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：SysTick_Handler
*  功能说明：系统滴答定时器中断服务函数
*  参数说明：无
*  函数返回：无
*  修改时间：2012-2-18    已测试
*  备    注：ucos里用得到
*************************************************************************/
void SysTick_Handler(void)
{
    //    OSIntEnter();
    //    OSTimeTick();
    //    OSIntExit();
}





/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：HardFault_Handler
*  功能说明：硬件上访中断服务函数
*  参数说明：无
*  函数返回：无
*  修改时间：2012-2-4    已测试
*  备    注：可以用LED闪烁来指示发生了硬件上访
*************************************************************************/
void HardFault_Handler(void)
{
    while (1)
    {
        printf("\n****硬件上访错误!!!*****\r\n\n");
    }
}




/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：PendSV_Handler
*  功能说明：PendSV（可悬起系统调用）中断服务函数
*  参数说明：无
*  函数返回：无
*  修改时间：2012-2-15    已测试
*  备    注：uC/OS用来切换任务
*************************************************************************/
void PendSV_Handler(void)
{
}





/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：FTM0_IRQHandler
*  功能说明：FTM0输入捕捉中断服务函数
*  参数说明：无
*  函数返回：无
*  修改时间：2012-2-25
*  备    注：引脚号需要根据自己初始化来修改，参考现有的代码添加自己的功能
*************************************************************************/
void FTM0_IRQHandler()
{
}




/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：FTM1_IRQHandler
*  功能说明：FTM1输入捕捉中断服务函数
*  参数说明：无
*  函数返回：无
*  修改时间：2012-2-25
*  备    注：引脚号需要根据自己初始化来修改，参考现有的代码添加自己的功能
*************************************************************************/
void FTM1_IRQHandler()
{
}



