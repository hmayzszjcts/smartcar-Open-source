#include"include.h"




/************************************************
*  函数名称：System_Init
*  功能说明：系统所有功能初始化函数
*  参数说明：无
*  函数返回：无
*  修改时间：2014-1-14    no测试
*************************************************/
void System_Init()
{
    static unsigned char ID1,ID2;
    
    //buzzer初始化
    gpio_init(PORTD,15, GPO, LOW);
    Buzzer_Ring_ms(100);
    
    
    //LED:全灭
    gpio_init(PORTD, 3, GPO, HIGH);
    gpio_init(PORTD, 4, GPO, HIGH);
    gpio_init(PORTD, 5, GPO, HIGH);
    gpio_init(PORTD, 6, GPO, HIGH);
    
    
    //做时间测试
    //gpio_init(PORTE,7, GPO, LOW);
    //紧急停车指令接口
    //gpio_init(PORTE,8, GPI, LOW);
    
    
    //LCD模块初始化
    LCD_init(FALSE);    //TRUE  和 FALSE是正着还是反着刷屏 
    Disp_single_colour(Yellow);//Black
    //Print_child();    //小破孩刷屏
    
    
    //拨码开关端口初始化
    Switch_init();
    //重要参数初始化
    Parameters_init_CAR();
    LCD_Put_Int(0, 0,"Set_speed   :",(int)Speed.Goal_init,Red,Green);
    LCD_Put_Int(0,20,"Start enable:",StartLine.enable,Red,Green);
    LCD_Put_Int(0,40,"Ren   enable:",Ren.enable,Red,Green);
    LCD_Put_Int(0,60,"Slope enable:",Slope_Check_enable,Red,Green);
    if(Multiple_line_enable==1)
        LCD_PutString(0,80,"Multiple line mode",Red,Green);
    else
        LCD_PutString(0,80,"Single line mode",Red,Green);
    //LCD_Put_Int(0,100,"SD    enable:",SD_enable,Red,Green);

    
    //串口初始化
    uart_init(UART3,115200);
    uart_irq_DIS(UART3);//(比赛要DIS，不能是EN)
    
    
    //AD初始化
    //adc_init(ADC1, SE14);//PB10
    //adc_init(ADC1, SE15);//PB11
    
    
    //1ms的定时器
    pit_init_ms(PIT0, 1);
    set_irq_priority(68,2);
    enable_irq(68);
    
    
    //初始化陀螺仪L3G4200D,初始化加速度计mma8451
    Init_L3G4200D();//11010011		
    ID1 = Single_Read(L3G4200_Addr,WHO_AM_I);
    MMA845x_init();//00011010
    ID2 = Single_Read(MMA845x_IIC_ADDRESS,WHO_AM_I_REG);
    if(ID1 == 0xD3 && ID2==0x1A)
    {
        LCD_PutString(0,120,"L3G + MMA OK!",Red,Green);
        //printf("IIC OK!!!");
    }
    else
    {
        //printf("IIC ERROR!!!");
    }
    
    
    //pwm模块,编码器测速初始化
    Pwm_Port_init();
    Decoder_init();
    
    
    //鹰眼摄像头初始化,包括场中断和DMA
    Ov7725_Init();
    
    
    //sd卡模块初始化和dflash初始化(比赛干掉)
    if(SD_enable==1)
    {
        flash_init();
        for(int i=186;i<255;i++)
            flash_erase_sector(i);
    }
    //SDcard_Init();
    
    
    //结束初始化
    delayms(100);
    Buzzer_Ring_ms(100);
}



/************************************************
*  函数名称：Parameters_init_CAR2
*  功能说明：重要变量初始化(CAR2)
*  参数说明：无
*  函数返回：无
*  修改时间：2014-1-14    已经测试
*************************************************/
void Parameters_init_CAR()
{
    //平衡重要参数
    Angle.G_angle = 12;
    Angle.OFFSET = 10;
    Angle.P = 400;
    Angle.D = 16;
    
    
    //播码分配
    //bit7-->>起跑线使能 
    //bit6-->>人字使能
    //bit5-->>障碍使能
    //bit4-->>多行和单行的选择
    //bit3-->>SD卡使能（比赛时候关掉。并且可以换成不靠谱十字使能）
    //bit2-->>speed.bit2
    //bit1-->>speed.bit1
    //bit0-->>speed.bit0
    
    
    //**************起跑线**************
    StartLine.enable = (sw_PORT&0x80)>>7;
    StartLine.Delaytime = 200;
    
    //**************人字**************
    Ren.enable = (sw_PORT&0x40)>>6;
    //人字延时请看详细设定
    
    //**************坡道**************
    Slope_Check_enable = (sw_PORT&0x20)>>5;
    Block.enable = 1;
    
    
    //**************单行还是加权平均**************
    Multiple_line_enable = (sw_PORT&0x10)>>4;
    
    //**************SD和多个人字**************
    //    SD_enable  = (sw_PORT&0x08)>>3;
    //    Ren.Multiple_enable = 0;
    if(Ren.enable==1)
        Ren.Multiple_enable = (sw_PORT&0x08)>>3;
    
    
    //设定速度和方向的初始化
    if((sw_PORT&0x07) == 0)
    {
        Speed.Mode = 0;
        //speed
        Speed.Goal = 0;
        Speed.Goal_init = 0;
        Speed.P = 0;
        Speed.I = 0;
        //direction
        near_hang = 180;
        far_hang = 70;
        Direction.P = 0;
        Direction.D = 0;
    }
    else if((sw_PORT&0x07) == 1)//差不多不用调 1.74m/s
    {
        Speed.Mode = 1;
        //speed
        Speed.Goal_init = 4700;
        Speed.P = 2;
        Speed.I = 0.3;
        //speed积分限幅
        Speed.I_Error_Start = 1000;//超调速度
        Speed.I_Limit_PWM_max = 1500;
        Speed.I_Limit_PWM_min = -1500;
        //direction
        if(Multiple_line_enable==1)
        {
            near_hang = 200;
            far_hang = 85;
            Direction.P = 23 * 0.8 * 2;
            Direction.D = 0.6;
            //人字
            Ren.Delaytime1 = 0;
            Ren.Strength = 100;
            Ren.Delaytime2 = 300;
            Ren.Angle_OFFSET[0] = 9;Ren.Angle_OFFSET[1] = 9;
        }
        else
        {
            Direction.P = 35 * 0.8 * 2;
            Direction.D = 0.7;
            //人字
            Ren.Delaytime1 = 0;
            Ren.Strength = 65;
            Ren.Delaytime2 = 300;
            Ren.Angle_OFFSET[0] = 9;Ren.Angle_OFFSET[1] = 9;
        }
        //障碍
        Block.delaytime = 150;
        Block.Offset_Init = 15;
        //坡道
        Slope_delaytime = 1500;
    }
    else if((sw_PORT&0x07) == 2)//差不多不用调2.0m/s
    {
        Speed.Mode = 2;
        //speed
        Speed.Goal = 0;
        Speed.Goal_init = 5300;
        Speed.P = 2;
        Speed.I = 0.2;
        //speed积分限幅
        Speed.I_Error_Start = 1000;//超调速度
        Speed.I_Limit_PWM_max = 1500;
        Speed.I_Limit_PWM_min = -2000;
        Speed.PWM_Integral = 1400;
        //direction
        if(Multiple_line_enable==1)
        {
            near_hang = 180;
            far_hang = 65;
            Direction.P = 21 * 0.8 * 2;
            Direction.D = 0.45;
            //人字
            Ren.Delaytime1 = 0;
            Ren.Strength = 95;
            Ren.Delaytime2 = 250;
            Ren.Angle_OFFSET[0] = 9;Ren.Angle_OFFSET[1] = 9;
        }
        else
        {
            Direction.P = 30 * 0.8 * 2;
            Direction.D = 0.5;
            //人字
            Ren.Delaytime1 = 0;
            Ren.Strength = 65;
            Ren.Delaytime2 = 250;
            Ren.Angle_OFFSET[0] = 9;Ren.Angle_OFFSET[1] = 9;
        }
        
        //障碍
        Block.delaytime = 150;
        Block.Offset_Init = 15; 
        //坡道
        Slope_delaytime = 1500;
    }
    else if((sw_PORT&0x07) == 3)//差不多不用调2.2m/s
    {
        Speed.Mode = 3;
        //speed
        Speed.Goal = 0;
        Speed.Goal_init = 5650;
        Speed.P = 2;
        Speed.I = 0.2;
        
        Speed.I_Error_Start = 1200;//超调速度
        Speed.I_Limit_PWM_max = 1500;
        Speed.I_Limit_PWM_min = -1500;
        Speed.PWM_Integral = 1400;
        //direction
        if(Multiple_line_enable==1)
        {
            near_hang = 190;
            far_hang = 40;
            Direction.P = 22 * 0.8 * 2;
            Direction.D = 0.45;
            //人字
            Ren.Delaytime1 = 0;
            Ren.Strength = 90;
            Ren.Delaytime2 = 170;
            Ren.Angle_OFFSET[0] = 9;Ren.Angle_OFFSET[1] = 9;
        }
        else
        {
            Direction.P = 30 * 0.8 * 2;
            Direction.D = 0.5;
            //人字
            Ren.Delaytime1 = 0;
            Ren.Strength = 65;
            Ren.Delaytime2 = 170;
            Ren.Angle_OFFSET[0] = 9;Ren.Angle_OFFSET[1] = 9;
        }
        //障碍
        Block.delaytime = 120;
        Block.Offset_Init = 15;
    }
    else if((sw_PORT&0x07) == 4)//差不多不用调2.4m/s待定！！
    {
        Speed.Mode = 4;
        //speed
        Speed.Goal = 0;
        Speed.Goal_init = 5850;
        Speed.P = 2;
        Speed.I = 0.2;
        
        Speed.I_Error_Start = 1200;//超调速度
        Speed.I_Limit_PWM_max = 1500;
        Speed.I_Limit_PWM_min = -1500;
        Speed.PWM_Integral = 1400;
        //direction
        if(Multiple_line_enable==1)
        {
            near_hang = 190;
            far_hang = 40;
            Direction.P = 22 * 0.8 * 2;
            Direction.D = 0.45;
            //人字
            Ren.Delaytime1 = 0;
            Ren.Strength = 100;
            Ren.Delaytime2 = 170;
            Ren.Angle_OFFSET[0] = 9;Ren.Angle_OFFSET[1] = 9;
        }
        else
        {
            Direction.P = 30 * 0.8 * 2;
            Direction.D = 0.5;
            //人字
            Ren.Delaytime1 = 0;
            Ren.Strength = 75;
            Ren.Delaytime2 = 170;
            Ren.Angle_OFFSET[0] = 9;Ren.Angle_OFFSET[1] = 9;
        }
        //障碍
        Block.delaytime = 120;
        Block.Offset_Init = 15;
    }
    else if((sw_PORT&0x07) == 5)//爆档，我和小伙伴都惊呆了档~~~
    {
        Speed.Mode = 5;
        //speed
        Speed.Goal = 0;
        Speed.Goal_init = 6100;
        Speed.P = 2;
        Speed.I = 0.2;
        
        Speed.I_Error_Start = 1200;//超调速度
        Speed.I_Limit_PWM_max = 1500;
        Speed.I_Limit_PWM_min = -1500;
        Speed.PWM_Integral = 1400;
        //direction
        if(Multiple_line_enable==1)
        {
            near_hang = 175;
            far_hang = 10;
            Direction.P = 22 * 0.8 * 2;
            Direction.D = 0.45;
            //人字
            Ren.Delaytime1 = 0;
            Ren.Strength = 105;
            Ren.Delaytime2 = 170;
            Ren.Angle_OFFSET[0] = 9;Ren.Angle_OFFSET[1] = 9;
        }
        else
        {
            Direction.P = 30 * 0.8 * 2;
            Direction.D = 0.5;
            //人字
            Ren.Delaytime1 = 0;
            Ren.Strength = 65;
            Ren.Delaytime2 = 170;
            Ren.Angle_OFFSET[0] = 9;Ren.Angle_OFFSET[1] = 9;
        }
        //障碍
        Block.delaytime = 120;
        Block.Offset_Init = 15;
    }
    else if((sw_PORT&0x07) == 6)//临时国赛加的龟速档位。防止小坡道
    {
        Speed.Mode = 6;
        //speed
        Speed.Goal_init = 4000;
        Speed.P = 2;
        Speed.I = 0.3;
        //speed积分限幅
        Speed.I_Error_Start = 1000;//超调速度
        Speed.I_Limit_PWM_max = 1500;
        Speed.I_Limit_PWM_min = -1500;
        //direction
        if(Multiple_line_enable==1)
        {
            near_hang = 200;
            far_hang = 85;
            Direction.P = 23 * 0.8 * 2;
            Direction.D = 0.6;
            //人字
            Ren.Delaytime1 = 80;
            Ren.Strength = 100;
            Ren.Delaytime2 = 300;
            Ren.Angle_OFFSET[0] = 9;Ren.Angle_OFFSET[1] = 9;
        }
        else
        {
            Direction.P = 35 * 0.8 * 2;
            Direction.D = 0.7;
            //人字
            Ren.Delaytime1 = 100;
            Ren.Strength = 65;
            Ren.Delaytime2 = 300;
            Ren.Angle_OFFSET[0] = 9;Ren.Angle_OFFSET[1] = 9;
        }
        //障碍
        Block.delaytime = 150;
        Block.Offset_Init = 15;
        //坡道
        Slope_delaytime = 1500;
    }
    else if((sw_PORT&0x07) == 7)//开环。。。编码器跪了档位
    {
        Speed.Mode = 7;//跑1档的图像
        //speed
        Speed.Goal = 0;
        Speed.Goal_init = 0;
        Speed.P = 0;
        Speed.I = 0;
        
        Speed.I_Error_Start = 1000;//超调速度
        Speed.I_Limit_PWM_max = 1500;
        Speed.I_Limit_PWM_min = 1500;
        Speed.PWM_Integral = 1200;
        //direction
        Direction.P = 38 * 0.8 * 2;
        Direction.D = 0.6;
        //人字
        Ren.Delaytime1 = 0;
        Ren.Strength = 80;
        Ren.Delaytime2 = 200;
        Ren.Angle_OFFSET[0] = 9;Ren.Angle_OFFSET[1] = 9;
    }
    else
    {
        //speed
        Speed.Goal = 0;
        Speed.Goal_init = 0;
        Speed.P = 0;
        Speed.I = 0;
        //direction
        Direction.P = 0;
        Direction.D = 0;
    }
}




/************************************************
*  函数名称：Buzzer_Ring_ms
*  功能说明：蜂鸣器叫
*  参数说明：unsigned int n，n为n毫秒
*  函数返回：无
*  修改时间：2014-1-14    已经测试
*************************************************/
void Buzzer_Ring_ms(unsigned int n)
{
    buzzer = 1;
    delayms(n);
    buzzer = 0;
}



/************************************************
*  函数名称：Switch_init
*  功能说明：拨码开关8个引脚初始化
*  参数说明：无
*  函数返回：无
*  修改时间：2014-1-14    已经测试
*************************************************/
void Switch_init()
{
    gpio_init(PORTB, 0, GPI, LOW);
    gpio_init(PORTB, 1, GPI, LOW);
    gpio_init(PORTB, 2, GPI, LOW);
    gpio_init(PORTB, 3, GPI, LOW);
    gpio_init(PORTB, 4, GPI, LOW);
    gpio_init(PORTB, 5, GPI, LOW);
    gpio_init(PORTB, 6, GPI, LOW);
    gpio_init(PORTB, 7, GPI, LOW);
}


/************************************************
*  函数名称：PwmPort_init
*  功能说明：两路pwm输出初始化,FTM0的CH6对应硬件D6，FTM0的CH5对应硬件D5
             DIR_L对应硬件A19，DIR_R对应硬件A24
*  参数说明：无
*  函数返回：无
*  修改时间：2014-1-14    已经测试
*************************************************/
void Pwm_Port_init()
{
    FTM_PWM_init(FTM0, CH2, 10000, 0);       //左路pwm
    FTM_PWM_init(FTM0, CH1, 10000, 0);       //右路pwm
    gpio_init(PORTE,25, GPO, LOW);              //DIR_L
    gpio_init(PORTE,24, GPO, LOW);              //DIR_R
    FTM_PWM_Duty(FTM0, CH2 ,  0);            //输出
    FTM_PWM_Duty(FTM0, CH1 ,  0);            //输出
}


/************************************************
*  函数名称：Decoder_init
*  功能说明：两路测速初始化,FTM1对应硬件A8，A9，FTM1对应硬件A10，A11，
*  参数说明：无
*  函数返回：无
*  修改时间：2014-1-14    已经测试
*************************************************/
void Decoder_init(void)
{
    /*************FTM1******************/
    SIM_BASE_PTR->SCGC5 |= SIM_SCGC5_PORTA_MASK;   //使能PortB的时钟
    SIM_BASE_PTR->SCGC6 |= SIM_SCGC6_FTM1_MASK;    //使能FlexTimer0(FTM1)的时钟
    PORTA_PCR8 |= PORT_PCR_MUX(0x6);              //复用PortB0的引脚功能为FTM1_QD_PHA
    PORTA_PCR9 |= PORT_PCR_MUX(0x6);              //复用PortB1的引脚功能为FTM1_QD_PHB

    FTM1_CNT = 0;
    FTM1_MOD = 10000;
    FTM1_CNTIN = 0;
    FTM1_CNT = 0;
    FTM1_MODE |= FTM_MODE_WPDIS_MASK;
    FTM1_MODE |= FTM_MODE_FTMEN_MASK;
    FTM1_QDCTRL |= FTM_QDCTRL_QUADEN_MASK;
    FTM1_CONF |= FTM_CONF_BDMMODE(3);
    FTM1_SC |= FTM_SC_PS(0);                      //计时器时钟预分频因子为 1
    FTM1_SC |= FTM_SC_TOIE_MASK;
    (void)FTM1_SC;
    FTM1_SC &= ~FTM_SC_TOF_MASK;
    FTM1_SC |= FTM_SC_CLKS(1);     //设置参考时钟为总线时钟并使能输出

    /***********FTM2*************/
    //SIM_BASE_PTR->SCGC5 |= SIM_SCGC5_PORTB_MASK; //使能PortB的时钟
    SIM_BASE_PTR->SCGC3 |= SIM_SCGC3_FTM2_MASK;//使能FlexTimer2(FTM2)的时钟
    PORTA_PCR10 |= PORT_PCR_MUX(0x6);    //复用PortB18的引脚功能为FTM2_QD_PHA
    PORTA_PCR11 |= PORT_PCR_MUX(0x6);   //复用PortB19的引脚功能为FTM2_QD_PHB

    FTM2_CNT = 0;
    FTM2_MOD = 10000;
    FTM2_CNTIN = 0;
    FTM2_CNT = 0;
    FTM2_MODE |= FTM_MODE_WPDIS_MASK;
    FTM2_MODE |= FTM_MODE_FTMEN_MASK;
    FTM2_QDCTRL |= FTM_QDCTRL_QUADEN_MASK;
    FTM2_CONF |= FTM_CONF_BDMMODE(3);
    FTM2_SC |= FTM_SC_PS(0);
    FTM2_SC |= FTM_SC_TOIE_MASK;//计时器时钟预分频因子为 1
    (void)FTM2_SC;
    FTM2_SC &= ~FTM_SC_TOF_MASK;
    FTM2_SC |= FTM_SC_CLKS(1);//设置参考时钟为总线时钟并使能输出
}


