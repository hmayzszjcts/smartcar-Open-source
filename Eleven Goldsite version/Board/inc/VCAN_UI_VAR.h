/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       VCAN_UI_VAR.h
 * @brief      UI 变量调试
 * @author     山外科技
 * @version    v5.0
 * @date       2014-01-04
 */

#ifndef _VCAN_UI_VAR_H_
#define _VCAN_UI_VAR_H_


#define VAR_SELECT_HOLD_OFFSET      ((VAR_MAX+4-1)/4)       //快上快下时，每次切换偏移量。目前设置为变量总数的1/4.可修改成其他数
#define VAR_VALUE_HOLE_OFFSET       10

#define UI_VAR_USE_LCD              1                       //同步信号时需要定义是否使用LCD显示数据（1为显示，0为不显示）


typedef struct
{
    uint32  val;            //目前的值
    uint32  oldval;         //通常情况下，两者是相同，修改后，没按确认键，则不相同。按确认键发送后，则相同
                            //即最后发送的值
    uint32  minval;         //最小值
    uint32  maxval;         //最大值
    Site_t  site;           //LCD 显示的坐标位置
} ui_var_info_t;            //变量信息

//变量的发送与接收
typedef enum
{
    //变量的编号

    /* 8位变量 */
    CAR_CTRL,
    VAR1,
    VAR2,
    VAR_8BIT = VAR2, //8位变量的结束编号

    /* 16位变量 */
    VAR3,
    VAR4,
    VAR_16BIT  = VAR4,          //16位变量的结束编号

    /* 32位变量 */
    VAR5,
    VAR6,
    VAR_32BIT  = VAR6,          //32位变量的结束编号

    VAR_MAX,        //变量数目
} var_tab_e;

typedef enum
{
    VAR_NEXT,           //下一个
    VAR_PREV,           //上一个
    VAR_NEXT_HOLD,      //快下，偏移为：VAR_SELECT_HOLD_OFFSET
    VAR_PREV_HOLD,      //快上，偏移为：VAR_SELECT_HOLD_OFFSET

    VAR_ADD,            //加1
    VAR_SUB,            //减1
    VAR_ADD_HOLD,       //快加，偏移为：VAR_VALUE_HOLE_OFFSET
    VAR_SUB_HOLD,       //快减，偏移为：VAR_VALUE_HOLE_OFFSET

    VAR_OK,             //确定
    VAR_CANCEL,         //取消

    VAR_EVENT_MAX,
} ui_var_event_e;

typedef enum
{
    CAR_NULL,           //不处理

    //图像的发送
    CAR_IMG_CLOSE,      //关闭小车图像发送(包括摄像头图像和线性CCD等大容量数据)
    CAR_IMG_OPEN,       //开启小车图像发送

    //一键停车开车
    CAR_START,          //开车
    CAR_STOP,           //停车

    //遥控小车
    CAR_FORWARD,        //前进
    CAR_BACK,           //后退

    CAR_LEFT,           //左转
    CAR_RIGHT,          //右转

    CAR_FAST,           //加速
    CAR_SLOW,           //降速





    //用户可在 此处加入 一些自定义功能
    //若增加功能，务必保证 接收和发送两端都是 使用相同的 控制功能编号


    CAR_CTRL_MAX,

}CAR_CTRL_MODE_e;


extern uint32   last_tab;           //最后接收到的变量编号

void var_init();

//变量 的 赋值 与 读值
extern void     save_var(var_tab_e var_tal, uint32 var_data);       //保存编号变量的值
extern void     get_var(var_tab_e var_tal, uint32 *var_data);       //获取编号变量的值 (根据变量编号找到变量地址，从而获取变量的值)
extern void     updata_var(var_tab_e var_tal);                      //更新编号变量的值（修改变量的值后，需要调用此函数来更新编号变量的值）

extern void     var_display(uint8 tab);                             //显示编号变量的值
extern uint8    var_syn(uint8 tab);                                 //同步指定的值。tab 为 VAR_NUM 时表示全部同步，小于则同步对应的编号变量


//变量 的 选择 与 加减 控制
extern void     var_select(ui_var_event_e  ctrl);   //选择切换变量
extern void     var_value(ui_var_event_e ctrl);     //改变变量的值
extern void     var_ok();                           //确认当前选择的
extern void     val_cancel();                       //取消当前选择的值  OK


extern uint8    car_ctrl_get();                     //获取小车控制命令
extern uint8    car_ctrl_syn(CAR_CTRL_MODE_e mode); //发送小车控制命令



#endif  //_VCAN_UI_VAR_H_

