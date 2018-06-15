
#include "common.h"
#include "dac.h"

volatile struct DAC_MemMap *DACx[2] = {DAC0_BASE_PTR, DAC1_BASE_PTR}; //定义两个指针数组保存 DACx 的地址

/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：vref_out_init
*  功能说明：内部参考电压初始化
*  参数说明：无
*  函数返回：无
*  修改时间：2012-3-25   已测试
*  备    注：
*************************************************************************/
void VREF_init(void)
{

    SIM_SCGC4 |= SIM_SCGC4_VREF_MASK ;          //开启VREF模块时钟

    VREF_SC = (  0
                 | VREF_SC_MODE_LV(1)             //VREF_OUT内部外部使用有效
                 | VREF_SC_VREFEN_MASK            //使能内部参考电压
                 | VREF_SC_REGEN_MASK             //1.75V 电压跟随器
              ) ;

    while (!(VREF_SC & VREF_SC_VREFST_MASK));    //等待内部电压参考稳定
}

/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：dac_once_init
*  功能说明：DAC一次转换初始化
*  参数说明：DACn        模块号（ DAC0、 DAC1）
*            VREF        参考电压选择（VREF_OUT、VDDA）
*  函数返回：无
*  修改时间：2012-3-25      已测试
*  备    注：
*************************************************************************/
void dac_once_init(DACn dacn, VREF vref)
{


    /* 使能时钟 */
    SIM_SCGC2 |= (SIM_SCGC2_DAC0_MASK << dacn) ;    //使能DAC模块

    if(vref == VREF_OUT)
    {
        VREF_init();
    }

    /*  配置DAC寄存器  */

    //配置DAC_C0寄存器
    DAC_C0_REG(DACx[dacn])  = ( 0
                                |  DAC_C0_DACTRGSEL_MASK                //选择软件触发
                                |  (vref << DAC_C0_DACRFS_SHIFT )       //选择参考电压
                                |  DAC_C0_DACEN_MASK                    //使能DAC模块
                              );

    //配置DAC_C1寄存器
    DAC_C1_REG(DACx[dacn]) = ( 0
                             ) ;
    //配置DAC_C2寄存器
    DAC_C2_REG(DACx[dacn]) = ( 0
                               | DAC_C2_DACBFRP(0)             //设置缓冲区读指针指向0
                             );

    DAC_DATA_REG(DACx[dacn], 0) = 0;   //默认输出最低电压
}

/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：dac_once_convert
*  功能说明：DAC一次转换操作
*  参数说明：DACn        模块号（ DAC0、 DAC1）
*            val         DAC转换数据（12bit）
*  函数返回：无
*  修改时间：2012-3-25      已测试
*  备    注：
*************************************************************************/
void dac_once_convert(DACn dacn, u16 val)
{
    ASSERT(val < 0x1000);               //val 为 12bit

    DAC_DATA_REG(DACx[dacn], 0) = val;
}

/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：dac_once_get
*  功能说明：DAC一次转换里，获取当前转换的数字量
*  参数说明：DACn        模块号（ DAC0、 DAC1）
*  函数返回：无
*  修改时间：2012-3-25      已测试
*  备    注：
*************************************************************************/
u16 dac_once_get(DACn dacn)
{
    return DAC_DATA_REG(DACx[dacn], 0);
}
