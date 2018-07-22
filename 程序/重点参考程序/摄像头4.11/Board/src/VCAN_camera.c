#include "common.h"
#include  "VCAN_camera.h"       //摄像头总头文件

//压缩二值化图像解压（空间 换 时间 解压）
//srclen 是二值化图像的占用空间大小
//【鹰眼解压】鹰眼图像解压，转为 二维数组 - 智能车资料区 - 山外论坛 http://vcan123.com/forum.php?mod=viewthread&tid=17&ctid=6
//解压的时候，里面有个数组，配置黑、白对应的值是多少。
#if 1
void img_extract(void *dst, void *src)
{
    uint8_t colour[2] = {255, 0}; //0 和 1 分别对应的颜色
    uint8_t * mdst = dst;  //二维数组
    uint8_t * msrc = src;  //一维数组
    uint32_t i,j;
    //注：山外的摄像头 0 表示 白色，1表示 黑色
/**      解压前，0 表示 白色，1表示 黑色；
*        解压后，由这个数组来定。上述的代码是：
*        colour[0]=255 ，即白色
*        colour[1]=0    ，即黑色
*/
    uint8_t tmpsrc;
   
    for(i=0;i<OV7725_EAGLE_H;i++)//总行数解压
    {
        for(j=0;j<OV7725_EAGLE_W/8;j++)//每一行
        {
            tmpsrc = *(msrc+i*OV7725_EAGLE_W/8+j);//取一个一维数组中的元素
            *mdst++ = colour[ (tmpsrc >> 7 ) & 0x01 ];  //令每一个为1的bit，都用colour[1]代替，按顺序装到二维数组中
            *mdst++ = colour[ (tmpsrc >> 6 ) & 0x01 ]; ////令每一个为0的bit，都用colour[0]代替
            *mdst++ = colour[ (tmpsrc >> 5 ) & 0x01 ];
            *mdst++ = colour[ (tmpsrc >> 4 ) & 0x01 ];
            *mdst++ = colour[ (tmpsrc >> 3 ) & 0x01 ];
            *mdst++ = colour[ (tmpsrc >> 2 ) & 0x01 ];
            *mdst++ = colour[ (tmpsrc >> 1 ) & 0x01 ];
            *mdst++ = colour[ (tmpsrc >> 0 ) & 0x01 ];
        }
    }
}
#endif


