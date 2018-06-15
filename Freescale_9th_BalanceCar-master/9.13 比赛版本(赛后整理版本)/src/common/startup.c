/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名       ：startup.c
 * 描述         ：kinetis的启动代码，把中断向量表、初始化数据、以__ramfunc声明的子函数复制到RAM区
 *
 * 实验平台     ：野火kinetis开发板
 * 库版本       ：
 *
 * 作者         ：飞思卡尔公司
 * 淘宝店       ：http://firestm32.taobao.com
 * 技术支持论坛 ：http://www.ourdev.cn/bbs/bbs_list.jsp?bbs_id=1008
**********************************************************************************/

#include "common.h"

#if (defined(IAR))
#pragma section = ".data"
#pragma section = ".data_init"
#pragma section = ".bss"
#pragma section = "CodeRelocate"
#pragma section = "CodeRelocateRam"
#endif

/********************************************************************/
//复制中断向量表、初始化数据、以__ramfunc声明的子函数复制到RAM区(对flash操作的函数必须这样)
void
common_startup(void)
{

#if (defined(CW))
    extern char __START_BSS[];
    extern char __END_BSS[];
    extern uint32 __DATA_ROM[];
    extern uint32 __DATA_RAM[];
    extern char __DATA_END[];
#endif

    /* Declare a counter we'll use in all of the copy loops */
    uint32 n;

    /* Declare pointers for various data sections. These pointers
     * are initialized using values pulled in from the linker file
     */
    uint8 *data_ram, * data_rom, * data_rom_end;
    uint8 *bss_start, * bss_end;


    /* 这两个数组的地址 在  链接器Linker文件，即ICF文件 定义 */
    extern uint32 __VECTOR_TABLE[];
    extern uint32 __VECTOR_RAM[];

    /* 检测是否需要 复制中断向量表  */
    if (__VECTOR_RAM != __VECTOR_TABLE)
    {
        for (n = 0; n < 0x410; n++)
            __VECTOR_RAM[n] = __VECTOR_TABLE[n];
    }

    /* 把 中断向量表地址 重新指向 RAM 里的 中断向量表 */
    write_vtor((uint32)__VECTOR_RAM);



    /*获取初始化数据地址*/
#if (defined(CW))
    data_ram = (uint8 *)__DATA_RAM;
    data_rom = (uint8 *)__DATA_ROM;
    data_rom_end  = (uint8 *)__DATA_END; /* This is actually a RAM address in CodeWarrior */
    n = data_rom_end - data_ram;
#elif (defined(IAR))
    data_ram = __section_begin(".data");
    data_rom = __section_begin(".data_init");
    data_rom_end = __section_end(".data_init");
    n = data_rom_end - data_rom;
#endif

    /* 复制初始化数据 */
    while (n--)
        *data_ram++ = *data_rom++;


    /* 获取 .bss 段的地址 (初始化数据为0) */
#if (defined(CW))
    bss_start = (uint8 *)__START_BSS;
    bss_end = (uint8 *)__END_BSS;
#elif (defined(IAR))
    bss_start = __section_begin(".bss");
    bss_end = __section_end(".bss");
#endif


    /* 清除初始化为0的数据段 */
    n = bss_end - bss_start;
    while(n--)
        *bss_start++ = 0;

    /* 把以__ramfunc声明的子函数复制到RAM区	 */
#if (defined(IAR))
    uint8 *code_relocate_ram  = __section_begin("CodeRelocateRam");
    uint8 *code_relocate      = __section_begin("CodeRelocate");
    uint8 *code_relocate_end  = __section_end("CodeRelocate");

    /* Copy functions from ROM to RAM */
    n = code_relocate_end - code_relocate;
    while (n--)
        *code_relocate_ram++ = *code_relocate++;
#endif
}
/********************************************************************/
