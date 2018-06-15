/*!*****************************************************************
 *  黑工院 飞思卡尔 串口调参程序历程
 *  功能 ：通过蓝牙串口设置参数，支持自定义个数的调参过程历史记录
*          可自定义保存调试好的参数表个数（参数结构体），并支持删除
 *  使用方式 ：通过指令控制参数，指令表看程序中的指令表输出函数
 *作者 ：电子协会飞思卡尔团队--谢洪言
********************************************************************
  串口使用经验：刚开始使用串口总是进入中断就出不来了，经过查资料和实验找到
这个解决办法。 K60的中断标志位不能用手动，硬件自己会清除标志位。*但*是*！
只有操作顺序对了的情况下才会自动清除。否则将出不去中断。
  正确操作顺序：一！定！要！先读取标记位 if(UART_S1_REG(UARTN[uratn]) & UART_S1_RDRF_MASK) 
               然后再读取串口数据 uart_querychar(UART_PORT,(uint8 *)cmd_buf);
               这样就能清除中断标志位了
  *注*意*,还没完呢！！！ 上边这只是接收一个字符的情况，基本都是这个写的，所以
    一般不会遇到出不去中断的问题。因为我直接在中断调用查询接收字符串的函数，所以
      会出不去中断函数，因为只有第一个字节来的时候中断会成功清除，后边的字节中断
        并没有清除。解决办法是将最底层的接收一个字节的函数添加查询标记位的一句话
        char uart_querychar (UARTn_e uratn, char *ch)//山外库
        {
            if( UART_RCFIFO_REG(UARTN[uratn]) )         //查询是否接受到数据
            {
                UART_S1_REG(UARTN[uratn]) &UART_S1_RDRF_MASK;//我后加的，这样能保证
                                                            //正确的读取顺序，在中断中使用能成功清除标记位
                *ch  =   UART_D_REG(UARTN[uratn]);      //接受到8位的数据
                return  1;                              //返回 1 表示接收成功
            }

            *ch = 0;                                    //接收不到，应该清空了接收区
            return 0;                                   //返回0表示接收失败
        }  
********************************************************************/
#include  "info_deal_save.h"
#include <stdio.h>
#include "speed_new.h"
#include "control.h"
#include "my_cfg.h"

uint8_t cmd_buf[30];
uint8_t rec_cmd_flag=0;
ParValue myPar_num;
/*!
 *  @brief      写入数据缓冲区到 flash指定地址
 *  @param      addr         要写入的地址
*  @param      cnt      写入的字节数
 *  @param      buf             需要写入的数据缓冲区首地址
 *  @return     执行结果(1成功，0失败)
 *  @since      v5.0
 *  Sample usage:           uint32 buff[10];
                            flash_write_buf(127,0,sizeof(buff),buff);        //扇区127,偏移地址为0,写入数据源地址:buff,数目sizeof(buff)
 *****************改的山外库****************
*/
__RAMFUNC uint8 flash_write_data(uint32  addr, uint16 cnt, uint8 *buf)
{
    uint32  size;
    uint32  data;
    // 设置写入命令
    FCMD = FLASH_WRITE_CMD;

    for(size = 0; size < cnt; size += FLASH_ALIGN )
    {

        // 设置目标地址
        FADDR2 = ((Dtype *)&addr)->B[2];
        FADDR1 = ((Dtype *)&addr)->B[1];
        FADDR0 = ((Dtype *)&addr)->B[0];

        // 设置 低32位数据
        data =  *(uint32 *)buf;

        FDATA0 = ((Dtype *)&data)->B[3];                    // 设置写入数据
        FDATA1 = ((Dtype *)&data)->B[2];
        FDATA2 = ((Dtype *)&data)->B[1];
        FDATA3 = ((Dtype *)&data)->B[0];

#if defined(MK60F15)
        // 设置 高32位数据
        data = *(uint32 *)(buf+4);

        FDATA4 = ((Dtype *)&data)->B[3];                    // 设置写入数据
        FDATA5 = ((Dtype *)&data)->B[2];
        FDATA6 = ((Dtype *)&data)->B[1];
        FDATA7 = ((Dtype *)&data)->B[0];
#endif

        if(flash_cmd() == 0)
        {
            return 0;
        }

        addr += FLASH_ALIGN;
        buf += FLASH_ALIGN;
    }
    return 1;  //成功执行
}
//*****************根据需求更改的函数****************************
//初始化全局结构体变量的值，然后保存到flash中，
//只有第一次flash中没有数据的时候调用，以后在下载程序的时候就不要调用了，
//不然会覆盖数据
void init_myPar_num()
{
    ParValue pv;
    myPar_num.SET_FLAG=0;
    myPar_num.ZHIDAO_SPEED=zhidao_speed;
    myPar_num.CD_SPEED[0]=CD_speed;
    myPar_num.CD_SPEED[1]=0;
    myPar_num.CD_SPEED[2]=0;
    myPar_num.CD_SPEED[3]=0;
    myPar_num.CD_SPEED[4]=0;
    myPar_num.CD_SPEED[5]=0;
    myPar_num.CD_SPEED[6]=0;
    myPar_num.CD_SPEED[7]=0;
    myPar_num.DJ_KP[0] = 1;
    myPar_num.DJ_KP[1] = 1;
    myPar_num.DJ_KP[2] = 1;
    myPar_num.DJ_KP[3] = 1;
    myPar_num.DJ_KP[4] = 1;
    myPar_num.DJ_KP[5] = 1;
    myPar_num.KP = speed_P;
    myPar_num.KD = speed_D;
    myPar_num.KI = speed_I;
    flash_erase_sector(Par_DEBUG_ADDR);                   //擦除扇区
    flash_write_data(Par_Debug_head_addr,PAR_STRUCT_SIZE,(uint8 *)&myPar_num);
    get_debug_Par_from_FLASH(&pv,0);//测试一下写的对不对，在调试时看参数
}
/*******************************************************************************
函数名称：InitPar
函数功能: 初始化参数函数,将结构体的参数值赋值给全局变量
参数：无
***这个根据自己的结构体类型改***
*******************************************************************************/
void InitPar()
{
     //evaluate_all(myPar_num);
     myPar_num.SET_FLAG=0;
     zhidao_speed=(uint8)myPar_num.ZHIDAO_SPEED;
     CD_speed=(uint8)myPar_num.CD_SPEED[0];
     speed_P = myPar_num.KP;
     speed_D = myPar_num.KD;
     speed_I = myPar_num.KI;
}

//功能 ：输出命令表
//注意 ：因为一次输出太多会接收不全，所以多获取几次
void dir_cmd()
{
    static uint8 flag=0;
    if(flag==0)
    {
        flag++;
        printf("-----\n");
        printf("lc->get_dir_cmd\n");//lc 得到命令表
        printf("ldx->get debug[x]\n");//ldx 获取第x个调试参数
        printf("lsx->get save_Par[x]\n");//lsx 获取第x个保存参数
        printf("gx->debug[x]=save[x]\n");//将第x个调整好的参数加载到当前的调试表中
        printf("Dx->delete save[x]\n");  //删除保存好的参数中第x个参数表
    }
    else if(flag==1)
    {
        flag=0;
        printf("z-zhidao_speed\n");
        printf("cx-CD_speed[x] \n");
        printf("b-biLi \n");
        printf("p-Kp \n");
        printf("d-Kd \n");
        printf("Px-duoji_Kp[x] \n");
    }
}
//功能 ：输出对应的参数，需要改成自己结构体的参数
void printf_Par(ParValue pv)
{
//    static uint8 flag=0;
//    uint8 i;
//    if(flag==0)
//    {
//          flag++;
//          printf("------\n");
//          printf("zhidao_speed =%d\n",pv.ZHIDAO_SPEED);
//          printf("CD_speed =%d\n",pv.CD_SPEED[0]);
//          printf("biLi =%d.%d\n",(int)pv.BILI,((int)(pv.BILI*100))%100);   
//          printf("Kp =%d\n",pv.KP);
//          printf("Kd =%d\n",pv.KD);
//    }
//    else if(flag==1)
//    {
//        flag++;
//        printf("duoji_Kp =%d.%d\n",(int)pv.DJ_KP[0],((int)(pv.DJ_KP[0]*100))%100);
//        printf("duoji_Kp1 =%d.%d\n",(int)pv.DJ_KP[1],((int)(pv.DJ_KP[1]*100))%100);
//        printf("duoji_Kp2 =%d.%d\n",(int)pv.DJ_KP[2],((int)(pv.DJ_KP[2]*100))%100);
//        printf("duoji_Kp3 =%d.%d\n",(int)pv.DJ_KP[3],((int)(pv.DJ_KP[3]*100))%100);
//        printf("duoji_Kp4 =%d.%d\n",(int)pv.DJ_KP[4],((int)(pv.DJ_KP[4]*100))%100);
//        printf("duoji_Kp5 =%d.%d\n",(int)pv.DJ_KP[5],((int)(pv.DJ_KP[5]*100))%100);
//    }
//    else if(flag==2)
//    {
//        flag=0;
//        printf("CD_speed[1] =%d\n",pv.CD_SPEED[1]);
//        printf("CD_speed[2] =%d\n",pv.CD_SPEED[2]);
//        printf("CD_speed[3] =%d\n",pv.CD_SPEED[3]);
//        printf("CD_speed[4] =%d\n",pv.CD_SPEED[4]);
//        printf("CD_speed[5] =%d\n",pv.CD_SPEED[5]);
//        printf("CD_speed[6] =%d\n",pv.CD_SPEED[6]);
//        printf("CD_speed[7] =%d\n",pv.CD_SPEED[7]);
//    }
//   
}
int max_speed=0;
int min_speed=0;
//用于调参后赋值（P,I,D,max_speed,mix_speed）
void evaluate_all(ParValue pv)
{
	//将调好的参数进行赋值给全局变量
//	speed_D  =pv.KD;
//	speed_I  =pv.KI;
//	speed_P  =pv.KP;
	zhidao_speed=pv.ZHIDAO_SPEED;//最大速度
	CD_speed=pv.CD_SPEED[0];//最小速度
	kp_val=myPar_num.DJ_KP[0];//基准P
	diff_speed=(float)myPar_num.BILI;//差速比例（整型）
	PID_init(); //
}
//用于蓝牙调参返回
void printf_val(ParValue pv)
{
// printf("max_speed =%d\n",pv.ZHIDAO_SPEED);
// printf("min_speed =%d\n",pv.CD_SPEED[0]);
// printf("Kp =%d\n",pv.KP);
// printf("Ki =%lf\n",pv.KI);
// printf("Kd =%d\n",pv.KD);

}
//float pintf_f(float val)
//{
//    int a,b;
//    float ret=0.0;
//    int shi_fen,bai_fen,qian_fen;
//    a=(int)(val*1000);//0.321*1000=321
//    shi_fen=a/100;//3
//    bai_fen=(a%100)/10;//2
//    qian_fen=(a%100)%10;//1
//    ret=(float)shi_fen*0.1+(float)bai_fen*0.01+(float)qian_fen*0.001;
//    
//    return ret;
//}
//功能 ：根据串口收到的数据解析并执行命令
//参数 ：cmd 串口就收到的字符串
//注意：参数赋值的地方需要自己改
void usart_Par_cmd(uint8 *cmd)
{
    float par=str2float(cmd);//跳过前边没用的字符，把字符串转换成数字
    switch(cmd[0])
    {
        //*******基础的指令可以不改**********
        case 'l': if(cmd[1]=='c') printf_val(myPar_num);//dir_cmd();//lc 遍历命令表
                   else if(cmd[1]=='d')get_debug_and_printf_Par((char)par);//ldx 获取调试表中第x个参数 例：ld5 
                   else if(cmd[1]=='s')get_save_and_printf_Par((char)par); //lsx 获取调整好的参数表的第x个
                      return;
        case 's':save_Save_par();return;//如果现在的参数可以了，用s指令将现在调试的参数储存起来，需要保存多少个可改写宏MAX_Par_SAVE_COUNT
        case 'g':get_savePar_to_debugPar((char)par);return;//gx 将设置好的参数表中第x个覆盖正在调试的参数
        case 'D':del_save_par((char)par);return;    //Dx删除保存的好的第x个数据表
        case 'r':run_start_flag=~run_start_flag;return;    //Dx删除保存的好的第x个数据表
        case 'I':if(my_debug_flag) my_debug_flag=0;
                  else my_debug_flag=1;return;   
        //********下边是对自己的参数赋值，字母含义知己编*******************************                  
        case 'y':if(cmd[1]=='1') //高速档
                  {
                    myPar_num.ZHIDAO_SPEED=136;myPar_num.CD_SPEED[0]=100;
                    printf("gaosu");
                  }
                  else if(cmd[1]=='2')//刹车
                  {
                    myPar_num.ZHIDAO_SPEED=0;myPar_num.CD_SPEED[0]=0;
                    printf("stop");                  
                  } 
                  else if(cmd[1]=='3')//中速
                  {
                    myPar_num.ZHIDAO_SPEED=80;myPar_num.CD_SPEED[0]=60;                          
                  }  
                  else if(cmd[1]=='4')//低速
                  {
                    myPar_num.ZHIDAO_SPEED=40;myPar_num.CD_SPEED[0]=30;                          
                  }  
                  else if(cmd[1]=='5')//极低速
                  {
                    myPar_num.ZHIDAO_SPEED=8;myPar_num.CD_SPEED[0]=3;                          
                  }  
                  break;
                  break;                 
                  
        case 'z':if(cmd[1]=='a') //最大速度
                    myPar_num.ZHIDAO_SPEED++;//直到速度加一
                  else if(cmd[1]=='d')  myPar_num.ZHIDAO_SPEED--;
                  else  myPar_num.ZHIDAO_SPEED=(char)par  ;
                  if(myPar_num.ZHIDAO_SPEED>240) myPar_num.ZHIDAO_SPEED=240;
                  printf("max_speed=%d",myPar_num.ZHIDAO_SPEED);
                  break;
        case 'c':par=str2float(cmd+2); //最小速度
                  if(cmd[2]=='a') myPar_num.CD_SPEED[cmd[1]-'0']++;//直到速度加一
                  else if(cmd[2]=='d')  myPar_num.CD_SPEED[cmd[1]-'0']--;
                  else  myPar_num.CD_SPEED[cmd[1]-'0']= (char)par  ;
                  printf("min_speed=%d",myPar_num.CD_SPEED[cmd[1]-'0']);
                  break;//全局速度
        case 't':if(cmd[1]=='a') myPar_num.BILI+=1;//差速因子
                  else if(cmd[1]=='d')  myPar_num.BILI-=1;
                  else  myPar_num.BILI=(char)par;
                  printf("biLi =%d\n",myPar_num.BILI); break;//全局比例
//                  printf("biLi =%d.%d\n",(int)myPar_num.BILI,((int)(myPar_num.BILI*100))%100); break;//全局比例
        case 'P':par=str2float(cmd+2);          //Px 注意是大写的，舵机的第x个比例P
                 if(cmd[2]=='a')   myPar_num.DJ_KP[cmd[1]-'0']+=1;
                 else if(cmd[2]=='d')   myPar_num.DJ_KP[cmd[1]-'0']-=1;
                 else myPar_num.DJ_KP[cmd[1]-'0']=par;
                printf("DJ_KP[%d] =%d.%d\n",cmd[1]-'0',(int)myPar_num.DJ_KP[cmd[1]-'0'],((int)(myPar_num.DJ_KP[cmd[1]-'0']*100))%100); break;
                
        case 'i': if(cmd[1]=='a') myPar_num.KI+=0.1;//电机I
                  else if(cmd[1]=='d')  myPar_num.KI-=0.1;
                  
                  else  myPar_num.KI=par;
                 // printf("p=%.3f",myPar_num.KI);  
                  printf("i =%d.%d\n",(int)myPar_num.KI,((int)(myPar_num.KI*100))%100); 
                  break;//全局比例
        case 'p': if(cmd[1]=='a') myPar_num.KP+=0.1;//电机P
                  else if(cmd[1]=='d')  myPar_num.KP-=0.1;
                  else  myPar_num.KP=(char)par;
//                  printf("p=%d",myPar_num.KP);
                  printf("p =%d.%d\n",(int)myPar_num.KP,((int)(myPar_num.KP*100))%100); 
                  break;
        case 'd': if(cmd[1]=='a') myPar_num.KD+=0.1;//电机D
                  else if(cmd[1]=='d')  myPar_num.KD-=0.1;
                  else  myPar_num.KD=(char)par;
//                  printf("d=%d",myPar_num.KD);
                  printf("d =%d.%d\n",(int)myPar_num.KD,((int)(myPar_num.KD*100))%100); 
                  break;
        default :printf("cmd err\n"); return;
    }
    save_Bebug_par();//保存参数修改
}
//**********下边的不用改***********************************************************

//功能 ：从flash得到指定地址的参数表
//参数 pv结构体变量指针 num-要找的第几个结构体
void get_debug_Par_from_FLASH(PAR_STRUCT_TYPE_NAME *pv,uint8 num)
{
    if(num>=MAX_Par_DEBUG_COUNT)
    {
        printf("cmd err\n");
        return;
    }
    memcpy((char *)pv,(char *)(Par_Debug_head_addr+PAR_STRUCT_SIZE*num),sizeof(PAR_STRUCT_TYPE_NAME));
}
//功能 ：从flash得到指定地址的参数表
//参数 pv结构体变量指针 num-要找的第几个结构体
void get_save_Par_from_FLASH(PAR_STRUCT_TYPE_NAME *pv,uint8 num)
{
    if(num>=MAX_Par_SAVE_COUNT)
    {
        printf("cmd err\n");
        return;
    }
    memcpy((char *)pv,(char *)(Par_Save_head_addr+PAR_STRUCT_SIZE*num),sizeof(PAR_STRUCT_TYPE_NAME));
}
//功能 ：从flash得到指定地址的参数表并且输出
//参数 ：num-要找的第几个结构体
void get_debug_and_printf_Par(uint8 num)
{
     PAR_STRUCT_TYPE_NAME pv;
     if(num<MAX_Par_DEBUG_COUNT)
     {
       get_debug_Par_from_FLASH(&pv,num);
       printf_Par(pv);
     }
     else
     {
        printf("cmd err\n");
     }
}
//功能 ：从flash得到指定地址的参数表并且输出
//参数 ：num-要找的第几个结构体
void get_save_and_printf_Par(uint8 num)
{
     PAR_STRUCT_TYPE_NAME pv;
     if(num<MAX_Par_SAVE_COUNT)
     {
       get_save_Par_from_FLASH(&pv,num);
       printf_Par(pv);
     }
     else
     {
        printf("cmd err\n");
     }
}
void save_Bebug_par()
{
    uint8 flag=1;
    char pv_data_buf[PAR_STRUCT_SIZE*(MAX_Par_DEBUG_COUNT)];
    memcpy(pv_data_buf+PAR_STRUCT_SIZE,(uint8*)Par_Debug_head_addr,sizeof(pv_data_buf)-PAR_STRUCT_SIZE);
    memcpy(pv_data_buf,&PAR_STRUCT_NAME,PAR_STRUCT_SIZE);
    flash_erase_sector(Par_DEBUG_ADDR);                     //擦除扇区
    flash_write_data(Par_Debug_head_addr,sizeof(pv_data_buf),(unsigned char *)pv_data_buf);
    if(flag)
    {
        get_debug_Par_from_FLASH(&PAR_STRUCT_NAME,0);
        InitPar();
//        printf_Par(PAR_STRUCT_NAME);
        printf("Save OK \n");
    }
    else
    {
        printf("Save ERR \n");
    }
}
void save_Save_par()
{
    uint8 flag=1;
    char pv_data_buf[PAR_STRUCT_SIZE*(MAX_Par_SAVE_COUNT)];
    memcpy(pv_data_buf+PAR_STRUCT_SIZE,(uint8*)Par_Save_head_addr,sizeof(pv_data_buf)-PAR_STRUCT_SIZE);
    memcpy(pv_data_buf,&PAR_STRUCT_NAME,PAR_STRUCT_SIZE);
    flash_erase_sector(Par_SAVE_ADDR);                     //擦除扇区
    flag=flash_write_data(Par_Save_head_addr,sizeof(pv_data_buf),(unsigned char *)pv_data_buf);
    if(flag)
    {
        get_save_Par_from_FLASH(&PAR_STRUCT_NAME,0);
        printf_Par(PAR_STRUCT_NAME);
        printf("Save OK \n");
    }
    else
    {
        printf("Save ERR \n");
    }
}
//功能 ：删除调试好的参数表中第num个参数表
void del_save_par(uint8 num)
{
    uint8 flag=1;
    char pv_data_buf[PAR_STRUCT_SIZE*(MAX_Par_SAVE_COUNT)];
    memcpy(pv_data_buf,(uint8*)Par_Save_head_addr,PAR_STRUCT_SIZE*num);
    memcpy(pv_data_buf+PAR_STRUCT_SIZE*num,(uint8*)Par_Save_head_addr+PAR_STRUCT_SIZE*(num+1),PAR_STRUCT_SIZE*(MAX_Par_SAVE_COUNT-num-1));
    flash_erase_sector(Par_SAVE_ADDR);                     //擦除扇区
    flag=flash_write_data(Par_Save_head_addr,sizeof(pv_data_buf),(unsigned char *)pv_data_buf);
    if(flag)
    {
        get_save_Par_from_FLASH(&PAR_STRUCT_NAME,0);
        printf_Par(PAR_STRUCT_NAME);
        printf("delete OK \n");
    }
    else
    {
        printf("delete ERR \n");
    }
}
//功能 ：将调好的参数第num个覆盖掉当前的调试参数
void get_savePar_to_debugPar(uint8 num)
{
    get_save_Par_from_FLASH(&PAR_STRUCT_NAME,num);
    save_Bebug_par();
}
//功能 ：提取接收到的数值
float str2float(uint8 *cmd)
{
    float f=0,i=10;
    while(*cmd<'0'||*cmd>'9')//去掉非数字部分
    {
        if(*cmd==0)
          break;
        cmd++;
    }
    while(*cmd!=0)//转换整数部分
    {
        if(*cmd<'0'||*cmd>'9')
            break;
        f=f*10+(*cmd-'0');
        cmd++;
    }
    if(*cmd=='.')
    {   
        cmd++;
        while(*cmd>='0'&&*cmd<='9')
        {
            f+=((*cmd-'0')/(i));
            i*=10;
            cmd++;
        }
    }
    return f;    
}

//void init_val_myPar_num()
//{
//  myPar_num.ZHIDAO_SPEED=145;//全局最大速度
//  myPar_num.CD_SPEED[0]=110;//全局最小速度
//  
//  myPar_num.KP=72;//电机P
//  myPar_num.KI=239;//电机I
//  myPar_num.KD=15;//电机D
//  
//  myPar_num.BILI=14;//差速因子  
//  
//}
void init_Par_from_FLASH()
{
    flash_init(); //用的野火的库函数
    //init_myPar_num(); //初始化全局结构体变量的值，然后保存到flash中，
                        //只有第一次flash中没有数据的时候调用，以后在下载程序的时候就不要调用了，
                        //不然会覆盖数据  
   // init_val_myPar_num();
    get_debug_Par_from_FLASH(&PAR_STRUCT_NAME,0);
    //InitPar();
    printf_Par(PAR_STRUCT_NAME);
}
void cmd_deal()
{
       if(rec_cmd_flag==1)
      {
          rec_cmd_flag=0;
          usart_Par_cmd(cmd_buf);
          memset(&cmd_buf,0,sizeof(cmd_buf)); 
      }
      evaluate_all(myPar_num);
}
