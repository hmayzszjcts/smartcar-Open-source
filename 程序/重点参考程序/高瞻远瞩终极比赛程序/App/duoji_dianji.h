#include "include.h"
#define steer_mid 770//8025
#define steer_max 8750//9000
#define steer_min 6750  //7050

extern s16 speed_a,speed_b;

void duoji_dianji_Init(void);
void get_maichong(void);
void FTM_QUAD_init();
void dianji_zhengzhuan_zuo(int zhankongbi);
void dianji_zhengzhuan_you(int zhankongbi);
void dianji_fanzhuan_zuo(int zhankongbi);
void dianji_fanzhuan_you(int zhankongbi);
void dianji_PID_zuo(int QWMC_zuo);
void dianji_PID_you(int QWMC_you);
void dianji_canshu_init(void);