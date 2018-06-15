//============================================================================
// 文件名称：lptmr.c                                                          
// 功能概要：lptmr构件源文件(Low Power Timer (LPTMR))
//============================================================================
//#include "MK60F15.h"
//#include "MK60DZ10.H"
#include "common.h"   
//============================================================================
//函数名称：lptmr_internal_ref_init
//函数返回：无
//参数说明：内部参考时钟初始化
//功能概要：采用内部参考时钟（PSC=0X0）
//         内部参考时钟有两个时钟源:(1)MCG_C2[IRCS]=0,使用慢速内部时钟(32kHz)
//                                 (2)MCG_C2[IRCS]=1,使用快速内部时钟(2Mhz)   
//         本例采用的是快速时钟源，周期= compare_value/ClkBus/prescale=4秒
//         prescale = 2^(8+1)=512,ClkBus=2Mhz,compare_value=15625
//============================================================================
#define LPTMR_ALT1 0x1  // PORTA19 pin 18
#define LPTMR_ALT2 0x2  // PORTA19 pin 18
#define OSC_BASE_PTR                             ((OSC_MemMapPtr)0x40065000u)
#define OSC_CR                                   OSC_CR_REG(OSC_BASE_PTR)
void lptmr_clear_registers();
void lptmr_internal_ref_init()
{
	unsigned int compare_value=15625; //4秒
	
	lptmr_clear_registers();          //复位LPTMR模块
	SIM_SCGC5|=SIM_SCGC5_LPTIMER_MASK;//使能LPT模块时钟
	MCG_C1|=MCG_C1_IRCLKEN_MASK;      //使能内部参考时钟
	 
	MCG_C2|=MCG_C2_IRCS_MASK; //MCG_C2[IRCS]=1,使能快速内部参考时钟（2MHz）
							  //MCG_C2[IRCS]=0,为慢速模式（32Khz）
	
	// 配置 LPTMR 
	LPTMR0_CMR=LPTMR_CMR_COMPARE(compare_value);          //设置比较寄存器值
	LPTMR0_PSR=LPTMR_PSR_PCS(0x0)|LPTMR_PSR_PRESCALE(0x8);//使用内部时钟，512预分频(2^(n+1))

	LPTMR0_CSR|=LPTMR_CSR_TEN_MASK; //开启LPT模块设置
}

//============================================================================
//函数名称：lptmr_LPO_init
//函数返回：无
//参数说明：LPO时钟初始化
//功能概要：采用LPO时钟（PSC=0X1,1kHz）   
//         本例采用的是快速时钟源，周期= compare_value/ClkBus/prescale=5秒
//         prescale = 无,ClkBus=1kHz,compare_value=5000
//============================================================================
void lptmr_LPO_init()
{
	int compare_value=1000;  //值必须小于 (0xFFFF,65535)
	
	lptmr_clear_registers();          //复位LPTMR模块
	SIM_SCGC5|=SIM_SCGC5_LPTIMER_MASK;//使能LPT模块时钟
	
	//配置 LPTMR 
	LPTMR0_CMR=LPTMR_CMR_COMPARE(compare_value);      //设置比较寄存器值
	LPTMR0_PSR=LPTMR_PSR_PCS(0x1)|LPTMR_PSR_PBYP_MASK;
	
	
	LPTMR0_CSR|=LPTMR_CSR_TEN_MASK; //开启LPT模块设置
}

//============================================================================
//函数名称：lptmr_32khz_init
//函数返回：无
//参数说明：LDO时钟初始化
//功能概要：采用二级外部参考时钟（PSC=0x2,32kHz）   
//         SOPT1[OSC32KSEL]=1,使用连接到XTAL32上的32kHz RTC crystal
//         SOPT1[OSC32KSEL]=0,使用连32kHz 系统 crystal，需要主板上有32kHz的crystal
//
//         周期= compare_value/ClkBus/prescale=5秒
//         prescale = 无,ClkBus=1kHz,compare_value=5000
//============================================================================
void lptmr_32khz_init()
{
	unsigned int compare_value=32768; //4 second delay with prescale=1
	
	lptmr_clear_registers();//复位LPTMR模块
	SIM_SCGC5|=SIM_SCGC5_LPTIMER_MASK;//使能LPT模块时钟
	
	// 配置使用来自RTC时钟的二级外部参考时钟（32Khz）
	SIM_SCGC6 |= SIM_SCGC6_RTC_MASK;      //使能 RTC 寄存器
	RTC_CR    |= RTC_CR_OSCE_MASK;        //开启  RTC oscillator
	SIM_SOPT1 |= SIM_SOPT1_OSC32KSEL_MASK;//选择 RTC OSC 作为 ERCLK32K的时钟源
	
	//配置Configure LPTMR
	LPTMR0_CMR=LPTMR_CMR_COMPARE(compare_value);          //设置比较寄存器值
	LPTMR0_PSR=LPTMR_PSR_PCS(0x2)|LPTMR_PSR_PRESCALE(0x1);//使用 32khz clock (ERCLK32K)且进行 4分频
	
	LPTMR0_CSR|=LPTMR_CSR_TEN_MASK; //开启LPT模块设置
}

//============================================================================
//函数名称：lptmr_external_clk_init
//函数返回：无
//参数说明：外部时钟初始化
//功能概要：采用二级外部参考时钟（PSC=0x3,50MHz）   
//
//         周期= compare_value/ClkBus/prescale=1秒
//         prescale = 1024,ClkBus=48MHz,compare_value=46875
//============================================================================
void lptmr_external_clk_init()
{
	unsigned int compare_value=46875;
	
	lptmr_clear_registers();          //复位LPTMR模块
	SIM_SCGC5|=SIM_SCGC5_LPTIMER_MASK;//使能LPT模块时钟
	
	//打开外部参考时钟
	MCG_C2&=~MCG_C2_EREFS_MASK;     //允许 extal to drive
	OSC_CR |= OSC_CR_ERCLKEN_MASK;  //选择 EXTAL to drive  XOSCxERCLK
	
	// 配置 LPTMR
	LPTMR0_CMR=LPTMR_CMR_COMPARE(compare_value);          //设置比较值
	LPTMR0_PSR=LPTMR_PSR_PCS(0x3)|LPTMR_PSR_PRESCALE(0x09);//使用外部时钟，预分频为 65536
	
	LPTMR0_CSR|=LPTMR_CSR_TEN_MASK; //开启LPT模块设置
}

//============================================================================
//函数名称：lptmr_pulse_counter
//函数返回：无
//参数说明：pin_select:选择的引脚（仅1,2）
//功能概要：脉冲累加功能 仅LPTMR0_ALT1和LPTMR0_ALT2引脚
//         LPTMR0_ALT1是引脚PORTA19 (ALT6)
//         LPTMR0_ALT2是引脚PORTC5 (ALT4).
//         
//============================================================================
void lptmr_pulse_counter(char pin_select)
{
	unsigned int compare_value=0xffff;  //在编码器计数模式，只需定时去读取计数值就可以，不需要比较
//	char input;

	lptmr_clear_registers();          //复位LPTMR模块
	SIM_SCGC5|=SIM_SCGC5_LPTIMER_MASK;//使能LPT模块时钟

	//Set up GPIO
	if(pin_select==LPTMR_ALT1)
	{
		SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK; //打开 PORTA 时钟
		PORTA_PCR19=PORT_PCR_MUX(0x6);     //在PTA19上使用 ALT6
	}
	else if(pin_select==LPTMR_ALT2)
	{
		SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK; //打开 PORTC 时钟 
		PORTC_PCR5=PORT_PCR_MUX(0x4);      //在PTC5上使用 ALT4
	}
	else
	{
		return;
	}

	LPTMR0_PSR=LPTMR_PSR_PCS(0x1)|LPTMR_PSR_PBYP_MASK; //使用 LPO clock 且 bypass glitch filter
	LPTMR0_CMR=LPTMR_CMR_COMPARE(compare_value);        //设置比较值
	LPTMR0_CSR=LPTMR_CSR_TPS(pin_select)|LPTMR_CSR_TMS_MASK; //设置LPT使用选择的引脚,且进入脉冲累加模式,上升沿捕捉
	
	LPTMR0_CSR|=LPTMR_CSR_TEN_MASK; //开启 LPT模块
}

//============================================================================
//函数名称：lptmr_clear_registers
//函数返回：无
//参数说明：无
//功能概要：清LPT模块的所有寄存器
//============================================================================
void lptmr_clear_registers()
{
//	LPTMR0_CSR=0x00;
//	LPTMR0_PSR=0x00;
//	LPTMR0_CMR=0x00;
}


//============================================================================
//函数名称：enable_lptmr_int
//函数返回：无
//参数说明：无
//功能概要：开启LPT定时器中断
//============================================================================
void enable_lptmr_int()
{
	LPTMR0_CSR|=LPTMR_CSR_TIE_MASK; //开启LPT定时器中断            
	enable_irq(LPTMR_IRQn);	    //开引脚的IRQ中断	
}

//============================================================================
//函数名称：disable_lptmr_int
//函数返回：无
//参数说明：无
//功能概要：关闭LPT定时器中断
//============================================================================
void disable_lptmr_int()
{
	LPTMR0_CSR &=~LPTMR_CSR_TIE_MASK;   //禁止LPT定时器中断          
	disable_irq(LPTMR_IRQn);	    //关引脚的IRQ中断
}


