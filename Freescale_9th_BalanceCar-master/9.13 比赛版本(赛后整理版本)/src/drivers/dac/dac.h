
#ifndef     _DAC_H_
#define     _DAC_H_ 1u



typedef enum DACn       //DAC模块
{
    DAC0,
    DAC1
} DACn;

typedef enum VREF       //参考电压
{
    VREF_OUT ,          //内部参考电压，1.2v
    VDDA                //3.3v
} VREF;


extern volatile struct DAC_MemMap *DACx[2];


/*     DAC一次转换的函数列表       */
extern void dac_once_init   (DACn, VREF);               //DAC一次转换初始化
extern void dac_once_convert(DACn, u16 val);            //DAC一次转换操作
extern u16  dac_once_get    (DACn);                     //DAC一次转换里，获取当前转换的数字量




/*     内部函数       */
void VREF_init(void);                            //初始化内部参考电压

#endif  //_DAC_H_

