/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       VCAN_NRF24L0.c
 * @brief      NRF24L0驱动函数实现
 * @author     山外科技
 * @version    v5.0
 * @date       2013-07-9
 */


#include "common.h"
#include "MK60_port.h"
#include "MK60_gpio.h"
#include "MK60_spi.h"

#include "VCAN_NRF24L0.h"

//NRF24L01+状态
typedef enum
{
    NOT_INIT = 0,
    TX_MODE,
    RX_MODE,
} nrf_mode_e;

typedef enum
{
    QUEUE_EMPTY = 0,        //队列空模式，只可入队列
    QUEUE_NORMAL,           //正常模式，可正常出入队列，即队列不空不满
    QUEUE_FULL,             //队列满模式，满了则不再添加，丢弃掉数据
} nrf_rx_queueflag_e; //中断接收时，队列状态标记位


//gpio控制CE和IRQ
#define NRF_CE_HIGH()       GPIO_SET(NRF_CE_PTXn,1)
#define NRF_CE_LOW()        GPIO_SET(NRF_CE_PTXn,0)           //CE置低
#define NRF_Read_IRQ()      GPIO_SET(NRF_IRQ_PTXn)


// 用户配置 发送和 接收地址，频道

uint8 TX_ADDRESS[5] = {0x34, 0x43, 0x10, 0x10, 0x01};   // 定义一个静态发送地址
uint8 RX_ADDRESS[5] = {0x34, 0x43, 0x10, 0x10, 0x01};

#define CHANAL          40                              //频道选择


// 内部配置参量
#define TX_ADR_WIDTH    ADR_WIDTH                       //发射地址宽度
#define TX_PLOAD_WIDTH  DATA_PACKET                     //发射数据通道有效数据宽度0~32Byte

#define RX_ADR_WIDTH    ADR_WIDTH                       //接收地址宽度
#define RX_PLOAD_WIDTH  DATA_PACKET                     //接收数据通道有效数据宽度0~32Byte

/******************************** NRF24L01+ 寄存器命令 宏定义***************************************/

// SPI(nRF24L01) commands , NRF的SPI命令宏定义，详见NRF功能使用文档
#define NRF_READ_REG    0x00    // Define read command to register
#define NRF_WRITE_REG   0x20    // Define write command to register
#define RD_RX_PLOAD     0x61    // Define RX payload register address
#define WR_TX_PLOAD     0xA0    // Define TX payload register address
#define FLUSH_TX        0xE1    // Define flush TX register command
#define FLUSH_RX        0xE2    // Define flush RX register command
#define REUSE_TX_PL     0xE3    // Define reuse TX payload register command
#define NOP             0xFF    // Define No Operation, might be used to read status register

// SPI(nRF24L01) registers(addresses) ，NRF24L01 相关寄存器地址的宏定义
#define CONFIG      0x00        // 'Config' register address
#define EN_AA       0x01        // 'Enable Auto Acknowledgment' register address
#define EN_RXADDR   0x02        // 'Enabled RX addresses' register address
#define SETUP_AW    0x03        // 'Setup address width' register address
#define SETUP_RETR  0x04        // 'Setup Auto. Retrans' register address
#define RF_CH       0x05        // 'RF channel' register address
#define RF_SETUP    0x06        // 'RF setup' register address
#define STATUS      0x07        // 'Status' register address
#define OBSERVE_TX  0x08        // 'Observe TX' register address
#define CD          0x09        // 'Carrier Detect' register address
#define RX_ADDR_P0  0x0A        // 'RX address pipe0' register address
#define RX_ADDR_P1  0x0B        // 'RX address pipe1' register address
#define RX_ADDR_P2  0x0C        // 'RX address pipe2' register address
#define RX_ADDR_P3  0x0D        // 'RX address pipe3' register address
#define RX_ADDR_P4  0x0E        // 'RX address pipe4' register address
#define RX_ADDR_P5  0x0F        // 'RX address pipe5' register address
#define TX_ADDR     0x10        // 'TX address' register address
#define RX_PW_P0    0x11        // 'RX payload width, pipe0' register address
#define RX_PW_P1    0x12        // 'RX payload width, pipe1' register address
#define RX_PW_P2    0x13        // 'RX payload width, pipe2' register address
#define RX_PW_P3    0x14        // 'RX payload width, pipe3' register address
#define RX_PW_P4    0x15        // 'RX payload width, pipe4' register address
#define RX_PW_P5    0x16        // 'RX payload width, pipe5' register address
#define FIFO_STATUS 0x17        // 'FIFO Status Register' register address


//几个重要的状态标记
#define TX_FULL     0x01        //TX FIFO 寄存器满标志。 1 为 满，0为 不满
#define MAX_RT      0x10        //达到最大重发次数中断标志位
#define TX_DS       0x20        //发送完成中断标志位
#define RX_DR       0x40        //接收到数据中断标志位



//内部寄存器操作函数声明
static  uint8   nrf_writereg(uint8 reg, uint8 dat);
static  uint8   nrf_readreg (uint8 reg, uint8 *dat);

static  uint8   nrf_writebuf(uint8 reg , uint8 *pBuf, uint32 len);
static  uint8   nrf_readbuf (uint8 reg, uint8 *pBuf, uint32  len);


static  void    nrf_rx_mode(void);           //进入接收模式
static  void    nrf_tx_mode(void);           //进入发送模式

/*!
 *  @brief      NRF24L01+ 模式标记
 */
volatile uint8  nrf_mode = NOT_INIT;


volatile uint8  nrf_rx_front = 0, nrf_rx_rear = 0;              //接收FIFO的指针
volatile uint8  nrf_rx_flag = QUEUE_EMPTY;

uint8 NRF_ISR_RX_FIFO[RX_FIFO_PACKET_NUM][DATA_PACKET];         //中断接收的FIFO


volatile uint8    *nrf_irq_tx_addr      = NULL;
volatile uint32    nrf_irq_tx_pnum      = 0;                    //pnum = (len+MAX_ONCE_TX_NUM -1)  / MAX_ONCE_TX_NUM

volatile uint8      nrf_irq_tx_flag  = 0;                     //0 表示成功 ，1 表示 发送失败

/*!
 *  @brief      NRF24L01+初始化，默认进入接收模式
 *  @return     初始化成功标记，0为初始化失败，1为初始化成功
 *  @since      v5.0
 *  Sample usage:
                        while(!nrf_init())                                     //初始化NRF24L01+ ,等待初始化成功为止
                        {
                            printf("\n  NRF与MCU连接失败，请重新检查接线。\n");
                        }

                        printf("\n      NRF与MCU连接成功！\n");
 */
uint8 nrf_init(void)
{
    //配置NRF管脚复用
    spi_init(NRF_SPI, NRF_CS, MASTER,12500*1000);                     //初始化SPI,主机模式

    gpio_init(NRF_CE_PTXn, GPO, LOW);                               //初始化CE，默认进入待机模式

    gpio_init(NRF_IRQ_PTXn, GPI, LOW);                              //初始化IRQ管脚为输入
    port_init_NoALT(NRF_IRQ_PTXn, IRQ_FALLING | PULLUP);            //初始化IRQ管脚为下降沿 触发中断

    //配置NRF寄存器
    NRF_CE_LOW();

    nrf_writereg(NRF_WRITE_REG + SETUP_AW, ADR_WIDTH - 2);          //设置地址长度为 TX_ADR_WIDTH

    nrf_writereg(NRF_WRITE_REG + RF_CH, CHANAL);                    //设置RF通道为CHANAL
    nrf_writereg(NRF_WRITE_REG + RF_SETUP, 0x0f);                   //设置TX发射参数,0db增益,2Mbps,低噪声增益开启

    nrf_writereg(NRF_WRITE_REG + EN_AA, 0x01);                      //使能通道0的自动应答

    nrf_writereg(NRF_WRITE_REG + EN_RXADDR, 0x01);                  //使能通道0的接收地址

    //RX模式配置
    nrf_writebuf(NRF_WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); //写RX节点地址

    nrf_writereg(NRF_WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH);         //选择通道0的有效数据宽度

    nrf_writereg(FLUSH_RX, NOP);                                    //清除RX FIFO寄存器

    //TX模式配置
    nrf_writebuf(NRF_WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH); //写TX节点地址

    nrf_writereg(NRF_WRITE_REG + SETUP_RETR, 0x0F);                 //设置自动重发间隔时间:250us + 86us;最大自动重发次数:15次

    nrf_writereg(FLUSH_TX, NOP);                                    //清除TX FIFO寄存器

    nrf_rx_mode();                                                  //默认进入接收模式

    NRF_CE_HIGH();

    return nrf_link_check();

}

/*!
 *  @brief      NRF24L01+写寄存器
 *  @param      reg         寄存器
 *  @param      dat         需要写入的数据
 *  @return     NRF24L01+ 状态
 *  @since      v5.0
 *  Sample usage:    nrf_writereg(NRF_WRITE_REG + RF_CH, CHANAL);   //设置RF通道为CHANAL
 */
uint8 nrf_writereg(uint8 reg, uint8 dat)
{
    uint8 buff[2];

    buff[0] = reg;          //先发送寄存器
    buff[1] = dat;          //再发送数据

    spi_mosi(NRF_SPI, NRF_CS, buff, buff, 2); //发送buff里数据，并采集到 buff里

    /*返回状态寄存器的值*/
    return buff[0];
}

/*!
 *  @brief      NRF24L01+读寄存器
 *  @param      reg         寄存器
 *  @param      dat         需要读取的数据的存放地址
 *  @return     NRF24L01+ 状态
 *  @since      v5.0
 *  Sample usage:
                    uint8 data;
                    nrf_readreg(STATUS,&data);
 */
uint8 nrf_readreg(uint8 reg, uint8 *dat)
{

    uint8 buff[2];

    buff[0] = reg;          //先发送寄存器

    spi_mosi(NRF_SPI, NRF_CS, buff, buff, 2); //发送buff数据，并保存到buff里

    *dat = buff[1];                         //提取第二个数据

    /*返回状态寄存器的值*/
    return buff[0];
}

/*!
 *  @brief      NRF24L01+写寄存器一串数据
 *  @param      reg         寄存器
 *  @param      pBuf        需要写入的数据缓冲区
 *  @param      len         需要写入数据长度
 *  @return     NRF24L01+ 状态
 *  @since      v5.0
 *  Sample usage:    nrf_writebuf(NRF_WRITE_REG+TX_ADDR,TX_ADDRESS,TX_ADR_WIDTH);    //写TX节点地址
 */
uint8 nrf_writebuf(uint8 reg , uint8 *pBuf, uint32 len)
{
    spi_mosi_cmd(NRF_SPI, NRF_CS, &reg , &reg, pBuf, NULL, 1 , len); //发送 reg ，pBuf 内容，不接收
    return reg;    //返回NRF24L01的状态
}


/*!
 *  @brief      NRF24L01+读寄存器一串数据
 *  @param      reg         寄存器
 *  @param      dat         需要读取的数据的存放地址
 *  @param      len         需要读取的数据长度
 *  @return     NRF24L01+ 状态
 *  @since      v5.0
 *  Sample usage:
                    uint8 data;
                    nrf_readreg(STATUS,&data);
 */
uint8 nrf_readbuf(uint8 reg, uint8 *pBuf, uint32 len)
{
    spi_mosi_cmd(NRF_SPI, NRF_CS, &reg , &reg, NULL, pBuf, 1 , len); //发送reg，接收到buff

    return reg;    //返回NRF24L01的状态
}

/*!
 *  @brief      检测NRF24L01+与MCU是否正常连接
 *  @return     NRF24L01+ 的通信状态，0表示通信不正常，1表示正常
 *  @since      v5.0
 *  Sample usage:
                    while(nrf_link_check() == 0)
                    {
                        printf("\n通信失败");
                    }
 */
uint8 nrf_link_check(void)
{
#define NRF_CHECH_DATA  0xC2        //此值为校验数据时使用，可修改为其他值

    uint8 reg;

    uint8 buff[5] = {NRF_CHECH_DATA, NRF_CHECH_DATA, NRF_CHECH_DATA, NRF_CHECH_DATA, NRF_CHECH_DATA};
    uint8 i;


    reg = NRF_WRITE_REG + TX_ADDR;
    spi_mosi_cmd(NRF_SPI, NRF_CS, &reg, NULL , buff, NULL, 1 , 5); //写入校验数据

    reg = TX_ADDR;
    spi_mosi_cmd(NRF_SPI, NRF_CS, &reg, NULL , NULL, buff, 1 , 5); //读取校验数据


    /*比较*/
    for(i = 0; i < 5; i++)
    {
        if(buff[i] != NRF_CHECH_DATA)
        {
            return 0 ;        //MCU与NRF不正常连接
        }
    }
    return 1 ;             //MCU与NRF成功连接
}

/*!
*  @brief      NRF24L01+进入接收模式
*  @since      v5.0
*/
void nrf_rx_mode(void)
{
    NRF_CE_LOW();

    nrf_writereg(NRF_WRITE_REG + EN_AA, 0x01);          //使能通道0的自动应答

    nrf_writereg(NRF_WRITE_REG + EN_RXADDR, 0x01);      //使能通道0的接收地址

    nrf_writebuf(NRF_WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); //写RX节点地址


    nrf_writereg(NRF_WRITE_REG + CONFIG, 0x0B | (IS_CRC16 << 2));       //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式

    /* 清除中断标志*/
    nrf_writereg(NRF_WRITE_REG + STATUS, 0xff);

    nrf_writereg(FLUSH_RX, NOP);                    //清除RX FIFO寄存器

    /*CE拉高，进入接收模式*/
    NRF_CE_HIGH();

    nrf_mode = RX_MODE;
}

/*!
*  @brief      NRF24L01+进入发送模式
*  @since      v5.0
*/
void nrf_tx_mode(void)
{
    volatile uint32 i;

    NRF_CE_LOW();
    //DELAY_MS(1);

    nrf_writebuf(NRF_WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH); //写TX节点地址

    nrf_writebuf(NRF_WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); //设置RX节点地址 ,主要为了使能ACK

    nrf_writereg(NRF_WRITE_REG + CONFIG, 0x0A | (IS_CRC16 << 2)); //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,发射模式,开启所有中断


    /*CE拉高，进入发送模式*/
    NRF_CE_HIGH();

    nrf_mode = TX_MODE;

    i = 0x0fff;
    while(i--);         //CE要拉高一段时间才进入发送模式

    //DELAY_MS(1);


}

/*!
 *  @brief      NRF24L01+数据接收
 *  @param      rxbuf       接收缓冲区地址
 *  @param      len         最大接收长度
 *  @return     实际接收的数据长度
 *  Sample usage:
            relen = nrf_rx(buff,DATA_PACKET);               //等待接收一个数据包，数据存储在buff里
            if(relen != 0)
            {
                printf("\n接收到数据:%s",buff);             //打印接收到的数据 （这里接收到的是字符串才可以用printf哦！）
            }

 *  @since      v5.0
 */
uint32  nrf_rx(uint8 *rxbuf, uint32 len)
{
    uint32 tmplen = 0;
    uint8 tmp;

    while( (nrf_rx_flag != QUEUE_EMPTY) && (len != 0) )
    {
        if(len < DATA_PACKET)
        {
            vcan_cpy(rxbuf, (uint8 *)&(NRF_ISR_RX_FIFO[nrf_rx_front]), len);

            NRF_CE_LOW();           //进入待机状态

            nrf_rx_front++;                //由于非空，所以可以直接出队列

            if(nrf_rx_front >= RX_FIFO_PACKET_NUM)
            {
                nrf_rx_front = 0;          //重头开始
            }
            tmp =  nrf_rx_rear;
            if(nrf_rx_front == tmp)       //追到屁股了，接收队列空
            {
                nrf_rx_flag = QUEUE_EMPTY;
            }
            NRF_CE_HIGH();          //进入接收模式

            tmplen += len;
            return tmplen;
        }

        vcan_cpy(rxbuf, (uint8 *)&(NRF_ISR_RX_FIFO[nrf_rx_front]), DATA_PACKET);
        rxbuf   += DATA_PACKET;
        len     -= DATA_PACKET;
        tmplen  += DATA_PACKET;

        NRF_CE_LOW();           //进入待机状态

        nrf_rx_front++;                //由于非空，所以可以直接出队列

        if(nrf_rx_front >= RX_FIFO_PACKET_NUM)
        {
            nrf_rx_front = 0;          //重头开始
        }
        tmp  = nrf_rx_rear;
        if(nrf_rx_front == tmp)       //追到屁股了，接收队列空
        {
            nrf_rx_flag = QUEUE_EMPTY;
        }
        else
        {
            nrf_rx_flag = QUEUE_NORMAL;
        }
        NRF_CE_HIGH();          //进入接收模式
    }

    return tmplen;
}

/*!
 *  @brief      NRF24L01+数据发送
 *  @param      rxbuf       发送缓冲区地址
 *  @param      len         发送长度
 *  @return     发送结果，0表示发送失败，1为发送中。最终发送结果需要调用nrf_tx_state()判断。
 *  Sample usage:
                    if(nrf_tx(buff,DATA_PACKET) == 1 );         //发送一个数据包：buff（包为32字节）
                    {
                        //等待发送过程中，此处可以加入处理任务

                        while(nrf_tx_state() == NRF_TXING);         //等待发送完成

                        if( NRF_TX_OK == nrf_tx_state () )
                        {
                            printf("\n发送成功:%d",i);
                            i++;                                    //发送成功则加1，可验证是否漏包
                        }
                        else
                        {
                            printf("\n发送失败:%d",i);
                        }
                    }
                    else
                    {
                        printf("\n发送失败:%d",i);
                    }

 *  @since      v5.0
 */
uint8    nrf_tx(uint8 *txbuf, uint32 len)
{
    nrf_irq_tx_flag = 0;        //复位标志位

    if((txbuf == 0 ) || (len == 0))
    {
        return 0;
    }

    if(nrf_irq_tx_addr == 0 )
    {
        //
        nrf_irq_tx_pnum = (len - 1) / DATA_PACKET;        // 进 1 求得 包 的数目
        nrf_irq_tx_addr = txbuf;

        if( nrf_mode != TX_MODE)
        {
            nrf_tx_mode();
        }

        //需要 先发送一次数据包后才能 中断发送

        /*ce为低，进入待机模式1*/
        NRF_CE_LOW();

        /*写数据到TX BUF 最大 32个字节*/
        nrf_writebuf(WR_TX_PLOAD, txbuf, DATA_PACKET);

        /*CE为高，txbuf非空，发送数据包 */
        NRF_CE_HIGH();

        return 1;
    }
    else
    {
        return 0;
    }
}


/*!
 *  @brief      检查NRF24L01+发送状态
 *  @return     发送结果，参考 nrf_tx_state_e 枚举的定义。
 *  Sample usage:
                    if(nrf_tx(buff,DATA_PACKET) == 1 );         //发送一个数据包：buff（包为32字节）
                    {
                        //等待发送过程中，此处可以加入处理任务

                        while(nrf_tx_state() == NRF_TXING);         //等待发送完成

                        if( NRF_TX_OK == nrf_tx_state () )
                        {
                            printf("\n发送成功:%d",i);
                            i++;                                    //发送成功则加1，可验证是否漏包
                        }
                        else
                        {
                            printf("\n发送失败:%d",i);
                        }
                    }
                    else
                    {
                        printf("\n发送失败:%d",i);
                    }

 *  @since      v5.0
 */
nrf_tx_state_e nrf_tx_state ()
{
    /*
    if(nrf_mode != TX_MODE)
    {
        return NRF_NOT_TX;
    }
    */

    if((nrf_irq_tx_addr == 0) && (nrf_irq_tx_pnum == 0))
    {
        //发送完成
        if(nrf_irq_tx_flag)
        {
            return NRF_TX_ERROR;
        }
        else
        {
            return NRF_TX_OK;
        }
    }
    else
    {
        return NRF_TXING;
    }
}

void nrf_handler(void)
{
    uint8 state;
    uint8 tmp;
    /*读取status寄存器的值  */
    nrf_readreg(STATUS, &state);

    /* 清除中断标志*/
    nrf_writereg(NRF_WRITE_REG + STATUS, state);

    if(state & RX_DR) //接收到数据
    {
        NRF_CE_LOW();

        if(nrf_rx_flag != QUEUE_FULL)
        {
            //还没满，则继续接收
            //printf("+");
            nrf_readbuf(RD_RX_PLOAD, (uint8 *)&(NRF_ISR_RX_FIFO[nrf_rx_rear]), RX_PLOAD_WIDTH); //读取数据

            nrf_rx_rear++;

            if(nrf_rx_rear >= RX_FIFO_PACKET_NUM)
            {
                nrf_rx_rear = 0;                            //重头开始
            }
            tmp = nrf_rx_front;
            if(nrf_rx_rear == tmp)                 //追到屁股了，满了
            {
                nrf_rx_flag = QUEUE_FULL;
            }
            else
            {
                nrf_rx_flag = QUEUE_NORMAL;
            }
        }
        else
        {
            nrf_writereg(FLUSH_RX, NOP);                    //清除RX FIFO寄存器
        }
        NRF_CE_HIGH();                                      //进入接收模式
    }

    if(state & TX_DS) //发送完数据
    {
        if(nrf_irq_tx_pnum == 0)
        {
            nrf_irq_tx_addr = 0;

            // 注意: nrf_irq_tx_pnum == 0 表示 数据 已经全部发送到FIFO 。 nrf_irq_tx_addr == 0 才是 全部发送完了

            //发送完成后 默认 进入 接收模式
#if 1
            if( nrf_mode != RX_MODE)
            {
                nrf_rx_mode();
            }
#endif

            //发送长度 为 0个包，即发送完成
            //nrf_writereg(FLUSH_TX, NOP);                        //清除TX FIFO寄存器
        }
        else
        {
            if( nrf_mode != TX_MODE)
            {
                nrf_tx_mode();
            }

            //还没发送完成，就继续发送
            nrf_irq_tx_addr += DATA_PACKET;    //指向下一个地址
            nrf_irq_tx_pnum --;                 //包数目减少

            /*ce为低，进入待机模式1*/
            NRF_CE_LOW();

            /*写数据到TX BUF 最大 32个字节*/
            nrf_writebuf(WR_TX_PLOAD, (uint8 *)nrf_irq_tx_addr, DATA_PACKET);

            /*CE为高，txbuf非空，发送数据包 */
            NRF_CE_HIGH();
        }
    }

    if(state & MAX_RT)      //发送超时
    {
        nrf_irq_tx_flag = 1;                            //标记发送失败
        nrf_writereg(FLUSH_TX, NOP);                    //清除TX FIFO寄存器


        //有可能是 对方也处于 发送状态

        //放弃本次发送
        nrf_irq_tx_addr = 0;
        nrf_irq_tx_pnum = 0;

        nrf_rx_mode();                                  //进入 接收状态


        //printf("\nMAX_RT");
    }

    if(state & TX_FULL) //TX FIFO 满
    {
        //printf("\nTX_FULL");

    }
}


//检测 接收FIFO 的数据  (0 没接收够 、1 为接收正确)
uint8  nrf_rx_fifo_check(uint32 offset,uint16 * val)
{
    uint8 rx_num = (offset + 1 + DATA_PACKET - 1 ) / DATA_PACKET;   //加1 是因为返回2个字节，最后一个自己所在的包数。
                                                                    //+ DATA_PACKET - 1 是四舍五入
    uint8 tmp;
    if(nrf_rx_flag == QUEUE_EMPTY)
    {
        return 0;
    }

    if(rx_num > RX_FIFO_PACKET_NUM)                                 //偏移太大，超过 FIFO 限制
    {
        return 0;
    }

    rx_num = nrf_rx_front + rx_num - 1;                             //目标查询的 包的位置
    tmp =  nrf_rx_rear;
    if(nrf_rx_front <  tmp)                                 //接收数据在 一圈之内
    {
        if(rx_num >= nrf_rx_rear )                                  //没接收足够的数据
        {
            return 0;
        }

        //获取数据

    }
    else                                                            //越过一圈
    {
        if(rx_num >= RX_FIFO_PACKET_NUM)                            //超过一圈
        {
            rx_num -= RX_FIFO_PACKET_NUM;

            if( rx_num >= nrf_rx_rear )                             //还没接收够
            {
                return 0;
            }
        }
        //获取数据
    }

    //获取数据
    *val = *(uint16 *)((char *)&NRF_ISR_RX_FIFO + ( rx_num*DATA_PACKET + (offset % DATA_PACKET - 2) )) ;
    return 1;

}


