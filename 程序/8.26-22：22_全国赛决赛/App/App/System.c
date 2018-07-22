#include "System.h"

extern const unsigned char gImage_123456[];

Key_Parameter TYPE[MaxLengh]={{"OV7725_CNST:",0,2},
                              {"Ramp_OFF_ON:",0,1},
                              {"Ramp_Speed :",0,5},
                              {"Ramp_Delay :",0,10},
                              {"S3010_P    :",0,1},
                              {"Annu_OFF_ON:",0,1},
                    };
Key_Parameter *Typemeber;

uint8  nrf_rx_buff[CAMERA_SIZE + 2*COM_LEN + DATA_PACKET];         //预多
uint8  nrf_tx_buff[CAMERA_SIZE + 2*COM_LEN + DATA_PACKET];         //预多
uint8 Image_Send_Flag = 1;	// 图像发送控制，1为发送，0为不发送
uint8 Send_OK = 0;

uint8 CNST_Value=60;
uint8 key_state,Key_Flag=0;
uint8 Img_Display;
void System_Init()
{
        uint32 i;
        uint8 j;
        /************************ 配置 K60 的优先级  ***********************/

        NVIC_SetPriorityGrouping(4);            //设置优先级分组,4bit 抢占优先级,没有亚优先级

        NVIC_SetPriority(PORTA_IRQn,0);         //配置优先级
        NVIC_SetPriority(PORTE_IRQn,1);         //配置优先级
        NVIC_SetPriority(DMA0_IRQn,2);          //配置优先级
        NVIC_SetPriority(PIT0_IRQn,3);          //配置优先级
//        NVIC_SetPriority(PIT1_IRQn,4);          //配置优先级
        /********************** PID参数初始化 ***********************/
        PlacePID_Init(&S3010_PID);//舵机PID参数初始化   参数还未调节
        PlacePID_Init(&CS_PID);//舵机PID参数初始化   参数还未调节
        IncPID_Init(&Left_MOTOR_PID,Left_MOTOR);	//电机左   PID参数初始化
        IncPID_Init(&Right_MOTOR_PID,Right_MOTOR);	//电机右   PID参数初始化
        
        /*********************** 舵机 初始化 ***********************/  
        ftm_pwm_init(S3010_FTM, S3010_CH,S3010_HZ,S3010_MID);   //初始化 舵机 PWM
        
        /************************** LED 初始化  ***************************/
        led_init(LED_MAX);
        /************************** OLED 初始化  ***************************/
        OLED_IIC_Init();
        OLED_Init();
        OLED_Fill(0xff);//全屏灭
        /************************** 拨码开关 初始化  ***************************/
         Boma_Key_Init();
        /************************ 各种参数 初始化  ************************************/
        Parameter_Init();
        
        while(!Key_Flag)
        {
              KEY_Adjuet();
              if(Key_Flag)break;
        }
        CNST_Value = Typemeber->Value;        //同步摄像头阈值
        Ramp_OFF_ON=(Typemeber+1)->Value;
        Ramp_Speed = (Typemeber+2)->Value;   //同步坡道速度
        Ramp_Delay = (Typemeber+3)->Value;   //同步坡道延时时间
        S3010[Set][0] = (Typemeber+4)->Value; //同步舵机P
        Annu_OFF_ON  = (Typemeber+5)->Value;
        SCCB_WriteByte (OV7725_CNST, CNST_Value);
        
        if (Img_Display)OLED_Fill(0xff);//全屏灭
        else 
        {
            OLED_Fill(0xff);//全屏灭
            OLED_DrawBMP(32,0,96,8,(unsigned char *)gImage_123456);//测试BMP位图显示
        }
        led(LED_MAX,LED_ON);// 打开LED指示灯		全部初始化成功
        /************************ 各种参数 初始化 结束 ************************************/
        
        
        
        
     
        
        /************************ 陀螺仪 初始化 **********************************/        
//        	while (MPU6050_Init());		// 陀螺仪初始化成功返回0
//	
//	for (i = 0; i < 400; i++)
//	{
//		for (j = 0; j < 5; j++)
//		{
//			MPU6050_GetData(&GYRO, &ACC);	// 读取陀螺仪数据
//			Data_Filter();					// 对原始数据滑动滤波
//			DELAY_US(100);
//		}
//		//Get_Attitude();	// 姿态解算
//	}

  	
	
	
	/************************ 电机 初始化  ***********************/
	ftm_pwm_init(MOTOR_FTM, Left_MOTOR1_PWM,MOTOR_HZ,0);      //初始化 电机左1 PWM
	ftm_pwm_init(MOTOR_FTM, Left_MOTOR2_PWM,MOTOR_HZ,0);      //初始化 电机左2 PWM 
        ftm_pwm_init(MOTOR_FTM, Right_MOTOR1_PWM,MOTOR_HZ,0);      //初始化 电机右1 PWM
	ftm_pwm_init(MOTOR_FTM, Right_MOTOR2_PWM,MOTOR_HZ,0);      //初始化 电机右2 PWM 
	/************************ 正交解码  ***********************/   
	ftm_quad_init(FTM1);                                    //FTM1 正交解码初始化（所用的管脚可查 port_cfg.h ）
	ftm_quad_init(FTM2);                                    //FTM2 正交解码初始化（所用的管脚可查 port_cfg.h ）
                                                               //使用定时器0触发，10ms执行一次
        
        /************************ 摄像头 初始化  ***********************/
        camera_init(p_imgbuff1);                                   //摄像头初始化，把图像采集到 imgbuff 地址
        //配置中断服务函数
        set_vector_handler(PORTA_VECTORn ,PORTA_IRQHandler);    //设置 PORTA 的中断服务函数为 PORTA_IRQHandler
        set_vector_handler(DMA0_VECTORn ,DMA0_IRQHandler);      //设置 DMA0 的中断服务函数为 DMA0_IRQHandler
        
          	/************************ 无线模块 NRF 初始化  ***********************/

//        while(!nrf_init());
//        //配置中断服务函数
//        set_vector_handler(PORTE_VECTORn ,PORTE_IRQHandler);                //设置 PORTE 的中断服务函数为 PORTE_VECTORn
//        enable_irq(PORTE_IRQn);
//        nrf_msg_init();                                                     //无线模块消息初始化
        
        //摄像头模块，需要 发送 空命令给 调试板模块，这样可以清掉 接收图像失败而产生多余数据
//        i = 20;
//        while(i--)
//        {
//          nrf_msg_tx(COM_RETRAN,nrf_tx_buff);                              //发送多个 空 命令过去，用于清空 接收端的缓存数据
//        }



	/******************** 定时器 初始化  *********************/ 
        pit_init_ms(PIT0,10);                                               //pit 定时中断(用于按键定时扫描)
        set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);                  //设置 PIT0 的中断服务函数为 PIT0_IRQHandler
        enable_irq(PIT0_IRQn);							  //使能中断
        
//        pit_init_ms(PIT1,20);                                               //pit 定时中断(用于按键定时扫描)
//        set_vector_handler(PIT1_VECTORn ,PIT1_IRQHandler);                  //设置 PIT0 的中断服务函数为 PIT0_IRQHandler
//        enable_irq(PIT1_IRQn);							  //使能中断
  
	/************************ 图像采集 **************************/
//	camera_get_img();//先采集一次图像
        
      /************************ LED 初始化  ************************************/
      led(LED_MAX,LED_OFF);// 关闭LED指示灯	

}


void data_send()
{		  
	com_e     com;
	nrf_result_e nrf_result;
	//uint8 imgtxflag = 0;    //0表示发送图像
  
  	/************************ 无线发送和接收数据  ***********************/
  	        do
        {
            nrf_result = nrf_msg_rx(&com, nrf_rx_buff);
            if(nrf_result == NRF_RESULT_RX_VALID)
            {
                switch(com)
                {
                    case COM_IMG:
                        //显示 到 屏幕上
//#if     ( CAMERA_COLOR == 1 )                                       //灰度摄像头
//                        LCD_Img_gray_Z(site,size, imgbuff,imgsize);
//#elif   ( CAMERA_COLOR == 0 )                                       //黑白摄像头
//                        LCD_Img_Binary_Z(site,size, imgbuff,imgsize);
//#endif
                        break;

                    case COM_VAR:

                        break;
                    default:
                        break;
                }
            }
        }while(nrf_result != NRF_RESULT_RX_NO);         //接收不到数据 才退出

        //把图像 发送出去
        nrf_msg_tx(COM_IMG,nrf_tx_buff);

        while(nrf_tx_state() == NRF_TXING);             //等待发送完成

	 var_syn(VAR2);
	 var_syn(VAR3);
//	 var_syn(VAR4);
//	 var_syn(VAR5);
//	 var_syn(VAR6);
}

/*************************按键***************************************/

void Boma_Key_Init(void)
{
     gpio_init(PTE0, GPI, 0);
     port_init_NoALT(PTE0, PULLDOWN);         //保持复用不变，仅仅改变配置选项
          gpio_init(PTE1, GPI, 0);
     port_init_NoALT(PTE1, PULLDOWN);         //保持复用不变，仅仅改变配置选项
          gpio_init(PTE2, GPI, 0);
     port_init_NoALT(PTE2, PULLDOWN);         //保持复用不变，仅仅改变配置选项
          gpio_init(PTE3, GPI, 0);
     port_init_NoALT(PTE3, PULLDOWN);         //保持复用不变，仅仅改变配置选项
          gpio_init(PTE4, GPI, 0);
     port_init_NoALT(PTE4, PULLDOWN);         //保持复用不变，仅仅改变配置选项
          gpio_init(PTE5, GPI, 0);
     port_init_NoALT(PTE5, PULLDOWN);         //保持复用不变，仅仅改变配置选项
          gpio_init(PTE6, GPI, 0);
     port_init_NoALT(PTE6, PULLDOWN);         //保持复用不变，仅仅改变配置选项
          gpio_init(PTE7, GPI, 0);
     port_init_NoALT(PTE7, PULLDOWN);         //保持复用不变，仅仅改变配置选项
     
     //按键
    key_init(KEY_U);
    key_init(KEY_D);
    key_init(KEY_L);
    key_init(KEY_R);
        
}


void Parameter_Init(void)
{
    if (gpio_get(PTE7) == KEY_UP)  //设置是否显示图像
    {
            Img_Display = 1;
    }
    else
    {
            Img_Display = 0;
    }
     if(gpio_get(PTE0) == KEY_UP)          //第一个环路
    {
        Annulus_Count[0] = 1;
    }
    else 
    {
        Annulus_Count[0] = 0;
    }
    if(gpio_get(PTE1) == KEY_UP)           //第二个环路
    {
       Annulus_Count[1] = 1;
    }
    else
    {
        Annulus_Count[1] = 0;
    }
    if(gpio_get(PTE2) == KEY_UP)           //第三个环路
    {
      Annulus_Count[2] = 1;
    }
    else
    {
      Annulus_Count[2] = 0;
    }
    if(gpio_get(PTE3) == KEY_UP)           //第四个环路
    {
      Hazard_Left_Right=1;
    }
    else
    {
      Hazard_Left_Right=0;
    }
    if(Hazard_Left_Right)Point_Mid=Point_Right;
    else Point_Mid=Point_Left;
//    if(gpio_get(PTE3) == KEY_DOWN&&gpio_get(PTE4) == KEY_DOWN)
//    {
//          SCCB_WriteByte (OV7725_CNST, 50);	//改变图像阈值	白天50	晚上60	
//    }
//    else if(gpio_get(PTE3) == KEY_DOWN&&gpio_get(PTE4) == KEY_UP)
//    {
//          SCCB_WriteByte (OV7725_CNST, 53);	//改变图像阈值	白天50	晚上60	
//    }
//    else if(gpio_get(PTE3) == KEY_UP&&gpio_get(PTE4) == KEY_DOWN)
//    {
//          SCCB_WriteByte (OV7725_CNST, 56);	//改变图像阈值	白天50	晚上60	
//    }
//     else if(gpio_get(PTE3) == KEY_UP&&gpio_get(PTE4) == KEY_UP)
//    {
//          SCCB_WriteByte (OV7725_CNST, 60);	//改变图像阈值	白天50	晚上60	
//    }
    if (gpio_get(PTE4) == KEY_DOWN)
    {
            if(gpio_get(PTE5) == KEY_DOWN&&gpio_get(PTE6) == KEY_DOWN)     //速度设定
            {
                  Speed_Set = 1;
                  Set = 0; 
            }
            else if(gpio_get(PTE5) == KEY_DOWN&&gpio_get(PTE6) == KEY_UP)
            {
                  Speed_Set = 2;
                  Set = 1; 
            }
            else if(gpio_get(PTE5) == KEY_UP&&gpio_get(PTE6) == KEY_DOWN)
            {
                  Speed_Set = 3;
                  Set = 1;
            }
             else if(gpio_get(PTE5) == KEY_UP&&gpio_get(PTE6) == KEY_UP)
            {
                  Speed_Set = 4;
                  Set = 0;
            }
    }
    if (gpio_get(PTE4) == KEY_UP)
    {
            if(gpio_get(PTE5) == KEY_DOWN&&gpio_get(PTE6) == KEY_DOWN)     //速度设定
            {
                  Speed_Set = 5;
                  Set = 1;
            }
            else if(gpio_get(PTE5) == KEY_DOWN&&gpio_get(PTE6) == KEY_UP)
            {
                  Speed_Set = 6;
                  Set = 1;
            }
            else if(gpio_get(PTE5) == KEY_UP&&gpio_get(PTE6) == KEY_DOWN)
            {
                  Speed_Set = 7;
                  Set = 1;
            }
             else if(gpio_get(PTE5) == KEY_UP&&gpio_get(PTE6) == KEY_UP)
            {
                  Speed_Set = 8;
                  Set = 1;
            }
    }
     
//     if(gpio_get(PTE7) == KEY_UP)           //控制按键位
//    {
//        key
//    }
//    else
//    {
//    
//    }
        Typemeber=TYPE;
        OLED_ShowStr(0,0,Typemeber->name, 1);
        OLED_ShowStr(0,1,(Typemeber+1)->name, 1);
        OLED_ShowStr(0,2,(Typemeber+2)->name, 1);
        OLED_ShowStr(0,3,(Typemeber+3)->name, 1);
        OLED_ShowStr(0,4,(Typemeber+4)->name, 1);
        OLED_ShowStr(0,5,(Typemeber+5)->name, 1);
        
        Typemeber->Value=CNST_Value;
        (Typemeber+1)->Value=Ramp_OFF_ON;
        (Typemeber+2)->Value=Ramp_Speed;
        (Typemeber+3)->Value=Ramp_Delay;
        (Typemeber+4)->Value=S3010[Set][0];
        (Typemeber+5)->Value=Annu_OFF_ON;
}

uint8 wait_key_down()
{
    uint8 ret_key=0xff;
    uint16 key_time=0;
    while(1)
    {
      if(key_check(KEY_U)==KEY_DOWN)
          ret_key=KEY_U;
      else if(key_check(KEY_D)==KEY_DOWN)
          ret_key=KEY_D;
      else if(key_check(KEY_L)==KEY_DOWN)
          ret_key=KEY_L;
      else if(key_check(KEY_R)==KEY_DOWN)
          ret_key=KEY_R;
      if(ret_key!=0xff)
      {
          key_state=KEY_DOWN;
          while(key_get(ret_key)==KEY_DOWN)
          {
              DELAY_MS(1);
              key_time++;
          }
          if(key_time>300)
          {
              key_state=KEY_HOLD;
          }
          break;
      }
    }
    return ret_key;
}


void KEY_Adjuet(void)
{
  uint8 key=0;
  static uint8 i=0;
 
  key = wait_key_down();
  
  switch (key)
  {
    case KEY_U:
    {
        if(i>=MaxLengh)i=0;
        Typemeber=TYPE+i;
        OLED_ShowStr(0,7,"              ", 1);
        OLED_ShowStr(0,7,Typemeber->name, 1);
        OLED_ShowLongNum(72,7,Typemeber->Value);
        i++;
        OLED_ShowLongNum(72,i-1,0);
        OLED_ShowLongNum(72,i-1,Typemeber->Value);
    }
    break;
    case KEY_D:
    {
      Typemeber->Value+=Typemeber->Step;
      OLED_ShowLongNum(72,7,0);
      OLED_ShowLongNum(72,7,Typemeber->Value);
      OLED_ShowLongNum(72,i-1,0);
      OLED_ShowLongNum(72,i-1,Typemeber->Value);
    }
    break;
  case KEY_L:
    {
        Typemeber->Value-=Typemeber->Step;
        OLED_ShowLongNum(72,7,0);
        OLED_ShowLongNum(72,7,Typemeber->Value);
        OLED_ShowLongNum(72,i-1,0);
        OLED_ShowLongNum(72,i-1,Typemeber->Value);
    }
    break;
    case KEY_R:
    {
      Key_Flag=1;
      Typemeber=TYPE;
      i=0;
    }
    break;
    
  }

}