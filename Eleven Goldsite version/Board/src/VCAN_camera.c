#include "common.h"


//压缩二值化图像解压（空间 换 时间 解压）
//srclen 是二值化图像的占用空间大小
//【鹰眼解压】鹰眼图像解压，转为 二维数组 - 智能车资料区 - 山外论坛 http://vcan123.com/forum.php?mod=viewthread&tid=17&ctid=6
//解压的时候，里面有个数组，配置黑、白对应的值是多少。
void img_extract(void *dst, void *src, uint32_t srclen)
{
    uint8_t colour[2] = {255, 0}; //0 和 1 分别对应的颜色
    uint8_t * mdst = dst;
    uint8_t * msrc = src;
    //注：山外的摄像头 0 表示 白色，1表示 黑色
    uint8_t tmpsrc;
    while(srclen --)
    {
        tmpsrc = *msrc++;
        *mdst++ = colour[ (tmpsrc >> 7 ) & 0x01 ];
        *mdst++ = colour[ (tmpsrc >> 6 ) & 0x01 ];
        *mdst++ = colour[ (tmpsrc >> 5 ) & 0x01 ];
        *mdst++ = colour[ (tmpsrc >> 4 ) & 0x01 ];
        *mdst++ = colour[ (tmpsrc >> 3 ) & 0x01 ];
        *mdst++ = colour[ (tmpsrc >> 2 ) & 0x01 ];
        *mdst++ = colour[ (tmpsrc >> 1 ) & 0x01 ];
        *mdst++ = colour[ (tmpsrc >> 0 ) & 0x01 ];
    }
}