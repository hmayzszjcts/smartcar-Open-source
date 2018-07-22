/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       VCAN_KEY.c
 * @brief      KEY驱动函数实现
 * @author     山外科技
 * @version    v5.0
 * @date       2013-07-10
 */


/*
 * 包含头文件
 */
#include "common.h"
#include "MK60_port.h"
#include "MK60_gpio.h"
#include "VCAN_key.h"


/*
 * 定义 KEY 编号对应的管脚
 */
PTXn_e KEY_PTxn[KEY_MAX] = {PTD10, PTD14, PTD11, PTD12, PTD7, PTD13, PTC14, PTC15};


/*!
 *  @brief      初始化key端口(key 小于 KEY_MAX 时初始化 对应端口，否则初始化全部端口)
 *  @param      KEY_e    KEY编号
 *  @since      v5.0
 *  Sample usage:       key_init (KEY_U);    //初始化 KEY_U
 */
void    key_init(KEY_e key)
{
    if(key < KEY_MAX)
    {
        gpio_init(KEY_PTxn[key], GPI, 0);
        port_init_NoALT(KEY_PTxn[key], PULLUP);         //保持复用不变，仅仅改变配置选项
    }
    else
    {
        key = KEY_MAX;

        //初始化全部 按键
        while(key--)
        {
            gpio_init(KEY_PTxn[key], GPI, 0);
            port_init_NoALT(KEY_PTxn[key], PULLUP);         //保持复用不变，仅仅改变配置选项
        }

    }
}

/*!
 *  @brief      获取key状态（不带延时消抖）
 *  @param      KEY_e           KEY编号
 *  @return     KEY_STATUS_e    KEY状态（KEY_DOWN、KEY_DOWN）
 *  @since      v5.0
 *  Sample usage:
                    if(key_get(KEY_U) ==  KEY_DOWN)
                    {
                        printf("\n按键按下")
                    }
 */
KEY_STATUS_e key_get(KEY_e key)
{
    if(gpio_get(KEY_PTxn[key]) == KEY_DOWN)
    {
        return KEY_DOWN;
    }
    return KEY_UP;
}


/*!
 *  @brief      检测key状态（带延时消抖）
 *  @param      KEY_e           KEY编号
 *  @return     KEY_STATUS_e    KEY状态（KEY_DOWN、KEY_DOWN）
 *  @since      v5.0
 *  Sample usage:
                    if(key_check(KEY_U) ==  KEY_DOWN)
                    {
                        printf("\n按键按下")
                    }
 */
KEY_STATUS_e key_check(KEY_e key)
{
    if(key_get(key) == KEY_DOWN)
    {
        DELAY_MS(10);
        if( key_get(key) == KEY_DOWN)
        {
            return KEY_DOWN;
        }
    }
    return KEY_UP;
}


/*********************  如下代码是实现按键定时扫描，发送消息到FIFO  ********************/
/*
 * 定义按键消息FIFO状态
 */
typedef enum
{
    KEY_MSG_EMPTY,      //没有按键消息
    KEY_MSG_NORMAL,     //正常，有按键消息，但不满
    KEY_MSG_FULL,       //按键消息满
} key_msg_e;

/*
 * 定义按键消息FIFO相关的变量
 */
KEY_MSG_t           key_msg[KEY_MSG_FIFO_SIZE];             //按键消息FIFO
volatile uint8      key_msg_front = 0, key_msg_rear = 0;    //接收FIFO的指针
volatile uint8      key_msg_flag = KEY_MSG_EMPTY;           //按键消息FIFO状态


/*!
 *  @brief      发送按键消息到FIFO
 *  @param      KEY_MSG_t       按键消息
 *  @since      v5.0
 *  Sample usage:
                    KEY_MSG_t *keymsg;
                    keymsg.key      = KEY_U;
                    keymsg.status   = KEY_HOLD;
                    send_key_msg(keymsg);                   //发送
 */
void send_key_msg(KEY_MSG_t keymsg)
{
    uint8 tmp;
    //保存在FIFO里
    if(key_msg_flag == KEY_MSG_FULL)
    {
        //满了直接不处理
        return ;
    }
    key_msg[key_msg_rear].key = keymsg.key;
    key_msg[key_msg_rear].status = keymsg.status;

    key_msg_rear++;

    if(key_msg_rear >= KEY_MSG_FIFO_SIZE)
    {
        key_msg_rear = 0;                       //重头开始
    }

    tmp = key_msg_rear;
    if(tmp == key_msg_front)                   //追到屁股了，满了
    {
        key_msg_flag = KEY_MSG_FULL;
    }
    else
    {
        key_msg_flag = KEY_MSG_NORMAL;
    }
}


/*!
 *  @brief      从FIFO里获取按键消息
 *  @param      KEY_MSG_t       按键消息
 *  @return     是否获取按键消息（1为获取成功，0为没获取到按键消息）
 *  @since      v5.0
 *  Sample usage:
                    KEY_MSG_t keymsg;
                    if(get_key_msg(&keymsg) == 1)
                    {
                        printf("\n按下按键KEY%d,类型为%d（0为按下，1为弹起，2为长按）",keymsg.key,keymsg.status);
                    }
 */
uint8 get_key_msg(KEY_MSG_t *keymsg)
{
    uint8 tmp;

    if(key_msg_flag == KEY_MSG_EMPTY)               //按键消息FIFO为空，直接返回0
    {
        return 0;
    }

    keymsg->key = key_msg[key_msg_front].key;       //从FIFO队首中获取按键值
    keymsg->status = key_msg[key_msg_front].status; //从FIFO队首中获取按键类型

    key_msg_front++;                                //FIFO队首指针加1，指向下一个消息

    if(key_msg_front >= KEY_MSG_FIFO_SIZE)          //FIFO指针队首溢出则从0开始计数
    {
        key_msg_front = 0;                          //重头开始计数（循环利用数组）
    }

    tmp = key_msg_rear;
    if(key_msg_front == tmp)                        //比较队首和队尾是否一样，一样则表示FIFO已空了
    {
        key_msg_flag = KEY_MSG_EMPTY;
    }
    else
    {
        key_msg_flag = KEY_MSG_NORMAL;
    }

    return 1;
}

/*!
 *  @brief      定时检测key状态
 *  @since      v5.0
 *  @note       此函数需要放入 定时中断服务函数里，定时10ms执行一次
 */
void key_IRQHandler(void)
{

    KEY_e   keynum;
    static uint8 keytime[KEY_MAX];                          //静态数组，保存各数组按下时间

    KEY_MSG_t keymsg;                                       //按键消息

    for(keynum = (KEY_e)0 ; keynum < KEY_MAX; keynum ++)    //每个按键轮询
    {
        if(key_get(keynum) == KEY_DOWN)                     //判断按键是否按下
        {
            keytime[keynum]++;                              //按下时间累加

            if(keytime[keynum] <= KEY_DOWN_TIME)            //判断时间是否没超过消抖确认按下时间
            {
                continue;                                   //没达到，则继续等待
            }
            else if(keytime[keynum] == KEY_DOWN_TIME + 1 )  //判断时间是否为消抖确认按下时间
            {
                //确认按键按下
                keymsg.key = keynum;
                keymsg.status = KEY_DOWN;
                send_key_msg(keymsg);                       //把按键值和按键类型发送消息到FIFO
            }
            else if(keytime[keynum] <= KEY_HOLD_TIME)       //是否没超过长按HOLD按键确认时间
            {
                continue;                                   //没超过，则继续等待
            }
            else if(keytime[keynum]  == KEY_HOLD_TIME + 1)  //是否为长按hold确认时间
            {
                //确认长按HOLD
                keymsg.key = keynum;
                keymsg.status = KEY_HOLD;
                send_key_msg(keymsg);                       //发送
                keytime[keynum] = KEY_DOWN_TIME + 1;
            }
            else
            {
                keytime[keynum] = KEY_DOWN_TIME + 1;        //继续重复检测 hold 状态
            }
        }
        else
        {
            if(keytime[keynum] > KEY_DOWN_TIME)             //如果确认过按下按键
            {
                keymsg.key = keynum;
                keymsg.status = KEY_UP;
                send_key_msg(keymsg);                       //发送按键弹起消息
            }

            keytime[keynum] = 0;                            //时间累计清0
        }
    }
}



