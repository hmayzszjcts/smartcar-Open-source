#include "common.h"
#include "vcan_img2sd.h"
#include "ff.h"
#include "VCAN_camera.h"       //摄像头总头文件

static FATFS vcanfs;    //文件系统
static FIL   vcansrc;   //文件


void img_sd_init(void)
{
    int     vcanres;
    char    myfilename[20];
    uint16  imgsize[] = {
                            CAMERA_H,
                            CAMERA_W,
                            //CAMERA_COLOR,
                         };      //高、宽、摄像头颜色类型
    uint32  mybw;
    uint32  Imag_num = 0;

    f_mount(0, &vcanfs);

    do
    {
        Imag_num ++;
        sprintf(myfilename, "0:/vcan%d.sd", Imag_num);
        vcanres = f_open( &vcansrc , myfilename, FA_CREATE_NEW | FA_WRITE);

        if(vcanfs.fs_type == 0)
        {
            vcansrc.fs = 0;
            return;
        }

    }while(vcanres == FR_EXIST);        //如果文件存在，则命名继续加1

    if ( vcanres == FR_OK )
    {
        vcanres = f_write(&vcansrc, imgsize, sizeof(imgsize),&mybw);  //先写入高和宽，方便上位机处理
    }
    else
    {
        f_close(&vcansrc);
        vcansrc.fs = 0;
    }
}




void img_sd_save(uint8 * imgaddr,uint32 size)
{
#define F_SYNC_TIME   30      //保存 n 次后 才 同步

    int   vcanres;
    uint32 mybw;
    static uint8 time = 0;
    //uint32 size = CAMERA_SIZE;

    if(vcansrc.fs != 0)
    {
        time ++;

        vcanres = f_write(&vcansrc, imgaddr, size ,&mybw);

        if(vcanres != FR_OK)
        {
            f_close(&vcansrc);
            vcansrc.fs = 0;
        }

        if(time > F_SYNC_TIME)
        {
            time = 0 ;
            f_sync(&vcansrc);
        }
    }
}

void img_sd_exit(void)
{
    f_close(&vcansrc);
    vcansrc.fs = 0;
}