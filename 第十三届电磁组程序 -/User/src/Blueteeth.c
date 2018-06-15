

#include "Blueteeth.h"
#if 0
extern float  SpeedP;
extern float  SpeedI;
extern float  SpeedD;
extern float  Differential;
extern int32  SpeedSet;
extern int    Startflag;
extern int   D;
extern float P;
extern float P1;
extern int pwm;
extern float Kp_U,Kd_U;
extern int Track_flag;

void uart0_handler(void)  
{  
   char ch;
   uart_getchar(UART0,&ch); 
   printf("Tx Succeed!\n");
   switch(ch)                       
     {  
         case 0x00:Startflag=0,printf("Startflag=0\n");break;
         case 0x11:Startflag=1,printf("Startflag=1\n");break;
 #if MotoPID_Open          
      case 0x01:Kp_U+=0.0001,printf("Kp_U=%d\n",(int)(Kp_U*10000));break;      
      case 0x02:Kd_U+=0.001,printf("Kd_U=%d\n",(int)(1000*Kd_U));break;
      case 0x03:Kp_U-=0.0001,printf("Kp_U=%d\n",(int)(Kp_U*10000));break;      
      case 0x04:Kd_U-=0.001,printf("Kd_U=%d\n",(int)(1000*Kd_U));break;            
        //case 0x01:SpeedP+=1,printf("SpeedP=%d\n",(int)(SpeedP));break;      
        // case 0x02:SpeedI+=0.001,printf("SpeedI=%d\n",(int)(1000*SpeedI));break;      
         //case 0x03:SpeedP-=1,printf("SpeedP=%d\n",(int)(SpeedP));break;
        // case 0x04:SpeedI-=0.001,printf("SpeedI=%d\n",(int)(1000*SpeedI));break;
         case 0x05:SpeedSet+=1,printf("SpeedSet=%d\n",SpeedSet);break;
         case 0x06:SpeedSet-=1,printf("SpeedSet=%d\n",SpeedSet);break;
         case 0x07:SpeedD+=0.1,printf("SpeedD=%d\n",(int)(SpeedD*10));break;         
         case 0x08:SpeedD-=0.1,printf("SpeedD=%d\n",(int)(SpeedD*10));break;
         case 0x09:Differential+=0.001,printf("Differential=%d\n",(int)(Differential*1000));break;
         case 0x10:Differential-=0.001,printf("Differential=%d\n",(int)(Differential*1000));break;
        // case 0x09:pwm+=1,printf("pwm=%d\n",pwm);break;
         //case 0x10:pwm-=1,printf("pwm=%d\n",pwm);break;
#endif
#if SteerPD_Open         
         case 0x01:P+=0.01,printf("P=%d\n",(int)(P*100));break;      
         case 0x02:D+=1,printf("D=%d\n",D);break;      
         case 0x03:P-=0.01,printf("P=%d\n",(int)(P*100));break;
         case 0x04:D-=1,printf("D=%d\n",D);break;
         case 0x05:SpeedSet+=1,printf("SpeedSet=%d\n",SpeedSet);break;
         case 0x06:SpeedSet-=1,printf("SpeedSet=%d\n",SpeedSet);break;
         case 0x07:P1+=0.1,printf("P1=%d\n",(int)(P1*10));break;         
         case 0x08:P1-=0.1,printf("P1=%d\n",(int)(P1*10));break;
         case 0x09:Differential+=0.001,printf("Differential=%d\n",(int)(Differential*1000));break;
         case 0x10:Differential-=0.001,printf("Differential=%d\n",(int)(Differential*1000));break;
#endif         
         default :
         printf("enter error!\n");
         break;
   } 
}

extern float YY_Distance;

void uart4_handler(void)
{
  uint8 buff[2];
  
  if(YY_Distance<600)
  {
   buff[0]=(uint8)(YY_Distance);
   uart_putbuff(UART4,buff,2);
  }
}
#endif