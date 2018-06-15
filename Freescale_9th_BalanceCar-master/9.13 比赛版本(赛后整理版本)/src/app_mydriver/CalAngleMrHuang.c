#include"include.h"


//»Æ°à³¤¸ß¼¶ÂË²¨
quaternion att;
double acc_qing[3];
double gyr_qing[3];
int xx,yy,zz;
float yaw = 0,pitch = 0,roll = 0;

void get_accgyr(void)
{
    acc_qing[0] = Get_mma8451_once('X');
    acc_qing[1] = Get_mma8451_once('Y');
    acc_qing[2] = Get_mma8451_once('Z');
    
    xx = Get_Gyro(1,'X') - 13;
    yy = Get_Gyro(1,'Y') - 11;
    zz = Get_Gyro(1,'Z') + 16;
    
    gyr_qing[0] = 0 - (double)(yy*0.00122173);
    gyr_qing[1] = (double)(xx*0.00122173);
    gyr_qing[2] = (double)(zz*0.00122173);  
}

float math_rsqrt(float number)
{
    long i;
    float x2, y;
    const float threehalfs = 1.5F;
    
    x2 = number * 0.5F;
    y  = number;
    i  = * ( long * ) &y;                       // evil floating point bit level hacking(?????????hack)
    i  = 0x5f3759df - ( i >> 1 );               // what the fuck?(??????????)
    y  = * ( float * ) &i;
    y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration (???????)
    y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed(?????,????)
    
    return y;
}

void quaternion_normalize(quaternion * q)
{
    float norm_r = math_rsqrt(q->w*q->w + q->x*q->x + q->y*q->y + q->z*q->z);
    q->w *= norm_r;
    q->x *= norm_r;
    q->y *= norm_r;
    q->z *= norm_r;
}

float acc_factor1 = 0.01;   //0.001
void mix_gyrAcc_crossMethod(quaternion * attitude,double gyr[3],double acc[3],float interval)
{
    float w_q = attitude->w;
    float x_q = attitude->x;
    float y_q = attitude->y;
    float z_q = attitude->z;
    float x_q_2 = x_q * 2;
    float y_q_2 = y_q * 2;
    float z_q_2 = z_q * 2;
    //
    float a_rsqrt = math_rsqrt(acc[0]*acc[0]+acc[1]*acc[1]+acc[2]*acc[2]);
    float x_aa = acc[0] * a_rsqrt;
    float y_aa = acc[1] * a_rsqrt;
    float z_aa = acc[2] * a_rsqrt;
    //
    float x_ac = x_q*z_q_2 - w_q*y_q_2;
    float y_ac = y_q*z_q_2 + w_q*x_q_2;
    float z_ac = 1 - x_q*x_q_2 - y_q*y_q_2;
    //
    float x_ca = y_aa * z_ac - z_aa * y_ac;
    float y_ca = z_aa * x_ac - x_aa * z_ac;
    float z_ca = x_aa * y_ac - y_aa * x_ac;
    //
    float delta_x = gyr[0] * interval / 2 + x_ca * acc_factor1;
    float delta_y = gyr[1] * interval / 2 + y_ca * acc_factor1;
    float delta_z = gyr[2] * interval / 2 + z_ca * acc_factor1;
    //
    attitude->w = w_q         - x_q*delta_x - y_q*delta_y - z_q*delta_z;
    attitude->x = w_q*delta_x + x_q         + y_q*delta_z - z_q*delta_y;
    attitude->y = w_q*delta_y - x_q*delta_z + y_q         + z_q*delta_x;
    attitude->z = w_q*delta_z + x_q*delta_y - y_q*delta_x + z_q;
    quaternion_normalize(attitude);
    
    if(Time_1ms < 100)
        acc_factor1 = 0.5;
    else
        acc_factor1 = 0.01;
}

void attitude_init(void)
{
    att.w = 0.878817;
    att.x = 0;
    att.y = 0;
    att.z = 0.477159;
}

void eular_angle(quaternion *a)
{
    float tmp1,tmp2;
    tmp1 = 2*(a->w*a->z + a->x*a->y);
    tmp2 = 1-2*(a->z*a->z + a->x*a->x);
    yaw = atan2(tmp1,tmp2);  //57.2958
    yaw *= 57.2958;
    
    tmp1 = 2*(a->w*a->x - a->y*a->z);
    pitch = asin(tmp1);
    pitch *= 57.2958;
    
    tmp1 = 2*(a->w*a->y + a->z*a->x);
    tmp2 = 1-2*(a->x*a->x + a->y*a->y);
    roll = atan2(tmp1,tmp2);
    roll *= 57.2958;
}