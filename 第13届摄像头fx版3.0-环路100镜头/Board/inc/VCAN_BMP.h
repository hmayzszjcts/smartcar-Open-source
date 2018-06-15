#ifndef _VCAN_BMP_H_
#define _VCAN_BMP_H_


#define MAX_BMP_H       240         //BMP 支持最高高度
#define MAX_BMP_W       320         //BMP 支持最高宽度
#define MAX_BMP_BIT     24          //BMP 支持最高位数


/*              BMP文件的组成结构                      */
//-------------------------------------------------------
//位图文件头（bitmap-file header）
//-------------------------------------------------------
//位图信息头（bitmap-information header）
//-------------------------------------------------------
//彩色表/调色板（color table）
//-------------------------------------------------------
//位图数据（bitmap-data）
//-------------------------------------------------------


typedef enum
{
    BI_RGB,             //无压缩
    BI_RLE8,            //每个象素8比特的RLE压缩编码，压缩格式由2字节组成(重复象素计数和颜色索引)；
    BI_RLE4,            //每个象素4比特的RLE压缩编码，压缩格式由2字节组成
    BI_BITFIELDS,       //每个象素的比特由指定的掩码决定。
}biCompression_e;

#if defined(__ICCARM__)
#pragma   pack(2)
#endif
typedef struct              //必须双字节对齐(或者单字节对齐)
{
    //BMP位图文件头
    uint16 bfType;         //偏移:0x00,文件标志.'BM',用来识别BMP位图类型
    uint32 bfSize;         //偏移:0x02,文件大小
    uint16 bfReserved1;    //偏移:0x06,保留
    uint16 bfReserved2;    //偏移:0x08,保留
    uint32 bfOffBits;      //偏移:0x0A,从文件开始到位图数据(bitmap data)开始之间的的偏移量

    //BMP位图信息头
    uint32 biSize;         //偏移:0x0E,位图信息头字节数量。
    int32  biWidth;        //偏移:0x12,图象宽度
    int32  biHeight;       //偏移:0x16,图象高度，如果是一个正数，说明图像数据是从图像左下角开始扫描，负数则从左上角
    uint16 biPlanes;       //偏移:0x1A,为目标设备说明位面数，其值将总是被设为1
    uint16 biBitCount;     //偏移:0x1C,说明比特数/象素，其值为1、4、8、16、24、或32
    uint32 biCompression;  //偏移:0x1E,说明图象数据压缩的类型。其值类型为 biCompression_e
    uint32 biSizeImage;    //偏移:0x22,说明图象的大小，以字节为单位。当用BI_RGB格式时，可设置为0
    uint32 biXPelsPerMeter;//偏移:0x26,说明水平分辨率，用象素/米表示,有符号整数
    uint32 biYPelsPerMeter;//偏移:0x2A,说明垂直分辨率，用象素/米表示,有符号整数
    uint32 biClrUsed;      //偏移:0x2E,说明位图实际使用的彩色表中的颜色索引数
    uint32 biClrImportant; //偏移:0x32,说明对图象显示有重要影响的颜色索引的数目，如果是0，表示都重要。

}BMP_FILEHEADER;
#if defined(__ICCARM__)
#pragma   pack()
#endif

//调色板 的颜色结构
typedef struct tagRGBQUAD {
    uint8    rgbBlue;
    uint8    rgbGreen;
    uint8    rgbRed;
    uint8    rgbReserved;
} RGBQUAD;



int8 SD2LCD_BMP(char *file,Site_t site);        //从SD卡获取bmp文件，并显示 (目前支持 BMP 图像的 24位真彩色/RGB555/单色)


#endif  //_VCAN_BMP_H_

