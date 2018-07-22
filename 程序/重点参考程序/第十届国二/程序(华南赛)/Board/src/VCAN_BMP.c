//支持24bit BMP图片显示
#include "common.h"
#include "VCAN_BMP.h"
#include "ff.h"
#include "vcan_lcd.h"


#define BMP_SIZE(W,H)           ((((W)*4+3)/4)*(H))         //((w * 4)+3)/4  是 为了 进 1 ，因为 bmp 宽度 是 4 个 倍数
#define BMP_BYTECNT(W,H,BIT)    (BMP_SIZE(W,H)*BIT/8)

#define MAX_BMP_SIZE            BMP_SIZE(MAX_BMP_W,MAX_BMP_H)
#define MAX_BMP_BYTECNT         BMP_BYTECNT(MAX_BMP_W,MAX_BMP_H,MAX_BMP_BIT)

#define BMP_LINE_BYTE(W,BIT)    ((((W)*(BIT)+31)/32)*4)
#define MAX_BMP_LINE_BYTE       BMP_LINE_BYTE(MAX_BMP_W,MAX_BMP_BIT)



//BMP一行图像
uint8 BMP_Buffer[MAX_BMP_LINE_BYTE];


int8 SD2LCD_BMP(char *file,Site_t site0)
{
    //文件系统相关的 变量
    FATFS fs={0};
    FIL fsrc={0};   
    FRESULT res;
    uint32 br;    

    
    BMP_FILEHEADER BmpHead;

    int Width,Height ;
    uint32 OffBits,Bitcnt ;



    uint8 r,g,b ;
    uint16 rgb;

    Size_t size;
    Site_t site;

    int x=0,y=0,BytesPerLine,Step ;

    uint8 *pColor;


    f_mount(0,&fs);

    //打开文件
    res=f_open(&fsrc,file,FA_OPEN_EXISTING | FA_READ);
    if(res)
    {
        goto bmp_exit_false;
    }

    //读取BMP 头部文件
    res=f_read(&fsrc,&BmpHead,sizeof(BmpHead),&br);
    if(res)
    {
        goto bmp_exit_false;
    }

    Width       = BmpHead.biWidth;
    Height      = BmpHead.biHeight;
    OffBits     = BmpHead.bfOffBits;        //bmp颜色数据起始位置
    Bitcnt      = BmpHead.biBitCount;

    if((BmpHead.bfType != 0x4d42)||(BmpHead.biCompression))           //'BM' 和  没压缩
    {
        DEBUG_PRINTF("\r\n不支持该图片显示！");
        goto bmp_exit_false;
    }

    if(Width > MAX_BMP_W)                   //图片宽度不能超过MAX_BMP_W
    {
        DEBUG_PRINTF("\r\n图片太大，无法显示！");
        goto bmp_exit_false;
    }

    if(Height>0)                            //如果高度是 正数，图像从左下角往上读数据
    {
        y = Height -1;
        Step = -1;
    }
    else if(Height < 0)                     //如果高度是 负数，图像从左上角往下读数据
    {
        Height = -Height;
        y = 0;
        Step = 1;
    }
    else
    {
        goto bmp_exit_false;
    }

    //一行一行读取
    size.H = 1;
    size.W = Width;
    site.x = site0.x;
    BytesPerLine = BMP_LINE_BYTE(Width,Bitcnt);       //一行的数目

    switch(Bitcnt)
    {
        case 24:        //24位真彩色 :BGR24
        {
            //开始画图
            res=f_lseek(&fsrc,OffBits); //找到bmp颜色数据起始位置

            for(;(y<Height)&&(y>=0);y+=Step)
            {
                res=f_read(&fsrc,BMP_Buffer,BytesPerLine,&br);
                if(res) goto bmp_exit_false;
                pColor = BMP_Buffer;

                //由于调用液晶顶层接口效率不高，这里直接调用底层
                site.y = site0.y +y;
                LCD_PTLON(site, size);              //开窗
                LCD_RAMWR();                        //写内存

                for(x=0;x<Width;x++)
                {
                    //BGR24
                    b=*pColor++;
                    g=*pColor++;
                    r=*pColor++;

                    rgb = RGB24_RGB565(r,g,b);
                    LCD_WR_DATA( rgb);
                }
            }
            break;

        }

        //16位中，最低的5位表示蓝色分量，中间的5位表示绿色分量，高的5位表示红色分量，一共占用了15位，最高的一位保留，设为0。这种格式也被称作555 16位位图。
        //如果是 RBG565 ,那么 biCompression = BI_BITFIELDS ，
        case 16:        //16位高彩色
        {
            //开始画图
            res=f_lseek(&fsrc,OffBits); //找到bmp颜色数据起始位置

            for(;(y<Height)&&(y>=0);y+=Step)
            {
                res=f_read(&fsrc,BMP_Buffer,BytesPerLine,&br);
                if(res)goto bmp_exit_false;
                pColor = BMP_Buffer;

                //由于调用液晶顶层接口效率不高，这里直接调用底层
                site.y = site0.y +y;
                LCD_PTLON(site, size);              //开窗
                LCD_RAMWR();                        //写内存

                for(x=0;x<Width;x++)
                {
                    rgb = *(uint16 *)pColor;    //bgr555
                    rgb = RGB555_RGB565(rgb);

                    LCD_WR_DATA( rgb);

                    pColor+=2;
                }
            }
            break;
        }

        //单色BMP，文件后头，还有2个RGBQUAD的调色板颜色，表示 0 和 1 分别对应的颜色。
        //一般是 0为黑 ， 1 为 白
        case 1:         //单色
        {
            //读完文件头后，马上读 调试板
            res=f_read(&fsrc,BMP_Buffer,2*sizeof(RGBQUAD),&br);
            uint16 color[2];
            color[0] = RGB24_RGB565(BMP_Buffer[2],BMP_Buffer[1],BMP_Buffer[0]);
            color[1] = RGB24_RGB565(BMP_Buffer[6],BMP_Buffer[5],BMP_Buffer[4]);

            //开始画图
            res=f_lseek(&fsrc,OffBits);         //找到bmp颜色数据起始位置

            Width = (Width+7)/8;                //8个像素一个点(+7是进1)

            for(;(y<Height)&&(y>=0);y+=Step)
            {
                res=f_read(&fsrc,BMP_Buffer,BytesPerLine,&br);
                if(res)goto bmp_exit_false;
                pColor = BMP_Buffer;

                //由于调用液晶顶层接口效率不高，这里直接调用底层
                site.y = site0.y +y;
                LCD_PTLON(site, size);              //开窗
                LCD_RAMWR();                        //写内存

                for(x=0;x<Width;x++)
                {
                    rgb = color[(*pColor >> 7)&0x01];
                    LCD_WR_DATA( rgb);
                    rgb = color[(*pColor >> 6)&0x01];
                    LCD_WR_DATA( rgb);
                    rgb = color[(*pColor >> 5)&0x01];
                    LCD_WR_DATA( rgb);
                    rgb = color[(*pColor >> 4)&0x01];
                    LCD_WR_DATA( rgb);
                    rgb = color[(*pColor >> 3)&0x01];
                    LCD_WR_DATA( rgb);
                    rgb = color[(*pColor >> 2)&0x01];
                    LCD_WR_DATA( rgb);
                    rgb = color[(*pColor >> 1)&0x01];
                    LCD_WR_DATA( rgb);
                    rgb = color[(*pColor >> 0)&0x01];
                    LCD_WR_DATA( rgb);

                    pColor+=1;
                }
            }
    
            break;

        }
        
        default:
        DEBUG_PRINTF("\r\n不支持该图片显示！");
        goto bmp_exit_false;

    }

    f_close(&fsrc);
    f_mount(0,NULL);

    return TRUE;

bmp_exit_false:
    f_close(&fsrc);
    f_mount(0,NULL);

    return FALSE;
}
/*************************** End of file ****************************/
