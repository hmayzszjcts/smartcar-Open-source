#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_







#define Data_num_A  1   //发送的变量个数（1个变量16位,2个字节）
#define Data_num_B  2   //发送的变量个数（1个变量16位，2个字节）

/*智能车的模式枚举类型声明*/
enum mode_car
{
  car_A,
  car_B,
};


void LogicalFunction(void);
extern enum mode_car car;//声明车子类型
extern void car_communication();// 双车通信函数
extern void PORTE_IRQHandler();//PORTE中断服务函数

#endif