#ifndef _CALANGLEMRHUANG_H
#define _CALANGLEMRHUANG_H

//»Æ°à³¤¸ß¼¶ÂË²¨
typedef struct
{
    float w;
    float x;
    float y;
    float z;
}quaternion;

extern  double acc_qing[3];
extern double gyr_qing[3];
extern quaternion att;
extern int xx,yy,zz;
extern float yaw,pitch,roll;

void mix_gyrAcc_crossMethod(quaternion * attitude,double gyr[3],double acc[3],float interval);
void get_accgyr(void);
void eular_angle(quaternion * a);
void attitude_init(void);

#endif