/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名       ：uart.h
 * 描述         ：串口相关函数的头文件
 * 备注         ：
 *
 * 实验平台     ：野火kinetis开发板
 * 库版本       ：
 * 嵌入系统     ：
 *
 * 作者         ：
 * 淘宝店       ：http://firestm32.taobao.com
 * 技术支持论坛 ：http://www.ourdev.cn/bbs/bbs_list.jsp?bbs_id=1008
**********************************************************************************/

#ifndef __UART_H__
#define __UART_H__

typedef enum  UARTn
{
    //初始化默认配置       --TXD--      --RXD--     可以复用其他通道，请自行修改 uart_init
    UART0,    //           PTD7         PTD6
    UART1,    //           PTC4         PTC3
    UART2,    //           PTD3         PTD2
    UART3,    //           PTC17        PTC16
    UART4,    //           PTE24        PTE25
    UART5     //           PTE8         PTE9
} UARTn;

extern volatile struct UART_MemMap *UARTx[6];
/********************************************************************/



void uart_init (UARTn, u32 baud);                     //初始化uartx模块

char uart_getchar   (UARTn);                          //无限等待接受1个字节
char uart_pendchar  (UARTn, char *ch);                //有限时间等待接收一个字符
char uart_pendstr   (UARTn, char *str);               //有限时间等待接收字符串

int uart_query      (UARTn);                          //查询是否接收到一个字节

void uart_putchar   (UARTn, char ch);                 //发送1个字节
void uart_sendN     (UARTn , uint8 *buff, uint16 len); //发送n个字节
void uart_sendStr   (UARTn , const u8 *str);          //发送字符串


void uart_irq_EN    (UARTn);                          //开串口接收中断
void uart_irq_DIS   (UARTn);                          //关串口接收中断

void SCISend_UnsignedInt(UARTn uratn,unsigned int pnum);
void SCISend_Int(UARTn uratn,int pnum);
void SCISend_to_PIDDebug(UARTn uratn);
void SCISendFloat(UARTn uratn,float f);
void SCISend_to_PIDDebug(UARTn uratn);
void SCISend_to_Own(UARTn uratn);
void Send_Ccddata_to_PC(UARTn uratn);
void Send_to_DataScope();
unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT);
void OutPut_Data();



#define UART_IRQ_EN(UARTn)   UART_C2_REG(UARTx[UARTn])|=UART_C2_RIE_MASK; enable_irq((UARTn<<1)+45)     //宏定义开串口接收中断
#define UART_IRQ_DIS(UARTn)  UART_C2_REG(UARTx[UARTn])&=~UART_C2_RIE_MASK; disable_irq((UARTn<<1)+45)   //宏定义关接收引脚的IRQ中断




/********************************************************************/

#endif /* __UART_H__ */
