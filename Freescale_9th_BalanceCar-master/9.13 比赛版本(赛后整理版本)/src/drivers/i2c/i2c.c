/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名       ：i2c.c
 * 描述         ：i2c驱动函数
 * 备注         ：修改自官方例程
 *
 * 实验平台     ：野火kinetis开发板
 * 库版本       ：
 * 嵌入系统     ：
 *
 * 作者         ：
 * 淘宝店       ：http://firestm32.taobao.com
 * 技术支持论坛 ：http://www.ourdev.cn/bbs/bbs_list.jsp?bbs_id=1008
**********************************************************************************/


#include "common.h"
#include "i2c.h"


unsigned char MasterTransmission;
unsigned char SlaveID;

volatile struct I2C_MemMap *I2Cx[2] = {I2C0_BASE_PTR, I2C1_BASE_PTR}; //定义两个指针数组保存 I2Cx 的地址


/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：I2C_init
*  功能说明：I2C初始化，设置波特率和启动I2C
*  参数说明：I2Cn        模块号（I2C0、I2C1）
*  函数返回：无
*  修改时间：2012-1-20
*  备    注：
*************************************************************************/
void I2C_init(I2Cn i2cn)
{
    u8 mult;
    if(bus_clk_khz <= 50000)mult = 0;         //bus 一分频
    else  if(bus_clk_khz <= 100000)mult = 1;  //bus 二分频
    else      mult = 2;                       //bus 四分频

    if(i2cn == I2C0)
    {
        /* 开启时钟 */
        SIM_SCGC4 |= SIM_SCGC4_I2C0_MASK;         //开启 I2C0时钟

        /* 配置 I2C0功能的 GPIO 接口 */
        if(I2C0_SCL == PTB0)
            PORTB_PCR0 = PORT_PCR_MUX(2);
        else if(I2C0_SCL == PTB2)
            PORTB_PCR2 = PORT_PCR_MUX(2);
        else if(I2C0_SCL == PTD8)
            PORTD_PCR8 = PORT_PCR_MUX(2);
        else
            assert_failed(__FILE__, __LINE__);                   //设置管脚有误？

        if(I2C0_SDA == PTB1)
            PORTB_PCR1 = PORT_PCR_MUX(2);
        else if(I2C0_SDA == PTB3)
            PORTB_PCR3 = PORT_PCR_MUX(2);
        else if(I2C0_SDA == PTD9)
            PORTD_PCR9 = PORT_PCR_MUX(2);
        else
            assert_failed(__FILE__, __LINE__);                   //设置管脚有误？
    }
    else
    {
        /* 开启时钟 */
        SIM_SCGC4 |= SIM_SCGC4_I2C1_MASK;         //开启 I2C1时钟

        /* 配置 I2C1功能的 GPIO 接口 */
        if(I2C1_SCL == PTE1)
            PORTE_PCR1 = PORT_PCR_MUX(6);
        else if(I2C1_SCL == PTC10)
            PORTC_PCR10 = PORT_PCR_MUX(2);
        else
            assert_failed(__FILE__, __LINE__);                   //设置管脚有误？

        if(I2C1_SDA == PTE0)
            PORTE_PCR0 = PORT_PCR_MUX(6);
        else if (I2C1_SDA == PTC11)
            PORTC_PCR11 = PORT_PCR_MUX(2);
        else
            assert_failed(__FILE__, __LINE__);                   //设置管脚有误？

    }

    /* 设置频率 */
    I2C_F_REG(I2Cx[i2cn])  = I2C_F_MULT(mult) | I2C_F_ICR(17) ;  // I2C Frequency Divider register (I2Cx_F)  I2C分频寄存器   I2C最大波特率为 400k
    // MULT=00  即  mul = 1
    // ICR =14  ICR为Clock rate时钟频率
    // 从《k16 reference manual.pdf》P1460 可得：
    // ICR    SCL Divider   SDA Hold Value    SCL Hold (Start) Value    SCL Hold (Stop) Value
    //  14        80              17                   34                         41
    //  17       128              21                   58                         65
    // I2C baud rate = bus speed (Hz)/(mul × SCL divider)  即这里 50MHz/(1 ×128)=390.625kHz
    // SDA hold time = bus period (s) × mul × SDA hold value
    // SCL start hold time = bus period (s) × mul × SCL start hold value
    // SCL stop hold time = bus period (s) × mul × SCL stop hold value

    /* 使能 IIC1 */
    I2C_C1_REG(I2Cx[i2cn]) = I2C_C1_IICEN_MASK;
}

/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：I2C_StartTransmission
*  功能说明：启动 I2C 传输
*  参数说明：I2Cn        模块号（I2C0、I2C1）
*            SlaveID     7位从机地址
*            MSmode      读写模式（ MWSR 或 MRSW ）
*  函数返回：无
*  修改时间：2012-1-20
*  备    注：
*************************************************************************/
void I2C_StartTransmission (I2Cn i2cn, u8 SlaveID, MSmode Mode)
{

    //ASSERT(Mode == MWSR || Mode == MRSW);         //使用断言，检测 Mode 是否为 读 或 写

    SlaveID = ( SlaveID << 1 ) | Mode ;            //确定写地址和读地址

    /* send start signal */
    i2c_Start(i2cn);

    /* send ID with W/R bit */
    i2c_write_byte(i2cn, SlaveID);
}

/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：Pause
*  功能说明：延时，I2C读写后，需要延时一下
*  参数说明：无
*  函数返回：无
*  修改时间：2012-1-20
*  备    注：在官方基础上增大延时数
*************************************************************************/
void Pause(void)
{
    u16 n;
    for(n = 1; n < 50000; n++)      //注意，这个数据太小，会导致读取错误。
    {
        asm("nop");
    }
}

/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：I2C_ReadAddr
*  功能说明：读取I2C设备指定地址寄存器的数据
*  参数说明：I2Cn        模块号（I2C0、I2C1）
*            SlaveID     7位从机地址
*            Addr        从机的寄存器地址
*  函数返回：从机寄存器的数据
*  修改时间：2012-1-20
*  备    注：
*************************************************************************/
u8 I2C_ReadAddr(I2Cn i2cn, u8 SlaveID, u8 Addr)
{
    u8 result;

    /* Send Slave Address */
    I2C_StartTransmission (i2cn, SlaveID, MWSR);
    i2c_Wait(i2cn);

    /* Write Register Address */
    i2c_write_byte(i2cn, Addr);
    i2c_Wait(i2cn);

    /* Do a repeated start */
    i2c_RepeatedStart(i2cn);

    /* Send Slave Address */
    i2c_write_byte(i2cn, ( SlaveID << 1) | MRSW );
    i2c_Wait(i2cn);

    /* Put in Rx Mode */
    i2c_PutinRxMode(i2cn);

    /* Turn off ACK since this is second to last byte being read*/
    i2c_DisableAck(i2cn); //不应答

    /* Dummy read 虚假读取*/
    result = I2C_D_REG(I2Cx[i2cn]);
    i2c_Wait(i2cn);

    /* Send stop since about to read last byte */
    i2c_Stop(i2cn);

    /* Read byte */
    result = I2C_D_REG(I2Cx[i2cn]);

    return result;
}


/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：I2C_WriteAddr
*  功能说明：写入一个字节数据到I2C设备指定寄存器地址
*  参数说明：I2Cn        模块号（I2C0、I2C1）
*            SlaveID     7位从机地址
*            Addr        从机的寄存器地址
*            Data        数据
*  函数返回：无
*  修改时间：2012-1-20
*  备    注：
*************************************************************************/
void I2C_WriteAddr(I2Cn i2cn, u8 SlaveID, u8 Addr, u8 Data)
{
    /* send data to slave */
    I2C_StartTransmission(i2cn, SlaveID, MWSR);    //启动传输
    i2c_Wait(i2cn);

    i2c_write_byte(i2cn, Addr);                    //写地址
    i2c_Wait(i2cn);

    i2c_write_byte(i2cn, Data);                    //写数据
    i2c_Wait(i2cn);

    i2c_Stop(i2cn);

    Pause();                                        //延时太短的话，可能写出错
}