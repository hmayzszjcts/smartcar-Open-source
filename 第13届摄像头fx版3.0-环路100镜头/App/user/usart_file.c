#include "usart_file.h"
#include  "info_deal_save.h"

char usart_read_str[10];
void uart4_handler(void);
void usart_init()
{
    uart_init(UART_PORT,UART_BAUD);
    set_vector_handler(UART5_RX_TX_VECTORn , uart5_handler);
    set_irq_priority((IRQn_Type)UART5_RX_TX_VECTORn,0);
    uart_rx_irq_en (UART_PORT);
    
}
void uart5_handler(void)
{
    UARTn_e uratn = UART_PORT;
    if(UART_S1_REG(UARTN[uratn]) & UART_S1_RDRF_MASK)   //接收数据寄存器满
    {
        uart_querystr(UART_PORT,(char *)cmd_buf,10);
        
        if(*cmd_buf!=0)
        {
//            usart_Par_cmd(cmd_buf);
            rec_cmd_flag=1;
        }
    }
    if(UART_S1_REG(UARTN[uratn]) & UART_S1_TDRE_MASK )  //发送数据寄存器空
    {
        //用户需要处理发送数据

    }
}