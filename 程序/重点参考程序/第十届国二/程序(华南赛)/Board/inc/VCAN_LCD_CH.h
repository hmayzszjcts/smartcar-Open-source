#ifndef _VCAN_LCD_CH_H_
#define _VCAN_LCD_CH_H_

//根据字库来定 （需要在 SD卡 根目录 下 放入 HZLIB.bin 字符文件 ）
#define LCD_CH_H    16
#define LCD_CH_W    16

#define LCD_CH_SIZE     (LCD_CH_W*LCD_CH_H/8)         //汉字占用空间大小



extern void LCD_Char_CH (Site_t site,const uint8 *str,uint16 Color,uint16 bkColor);
extern void LCD_Str_CH  (Site_t site,const uint8 *str, uint16 Color ,uint16 bkColor);   //纯汉字字符串
extern void LCD_Str_ENCH(Site_t site,const uint8 *str  , uint16 Color ,uint16 bkColor); //中英混合字符串

//下面的显示 函数，是调用 flash 里的字符数据显示汉字
extern void LCD_FChar_CH (Site_t site,const uint8 *strbuf,             uint16 Color,uint16 bkColor);
extern void LCD_FStr_CH  (Site_t site,const uint8 *strbuf,uint16 num,  uint16 Color,uint16 bkColor);

#define LCD_FSTR_CH(site,strbuf,Color,bkColor)   LCD_FStr_CH (site,strbuf,sizeof(strbuf)/LCD_CH_SIZE,Color,bkColor)


#endif  //ifndef _VCAN_LCD_CH_H_