#ifndef __EEPROM_H
#define __EEPROM_H 

#define SECTOR_NUM  (FLASH_SECTOR_NUM-1) 
#define BLOCK_SIZE  64
void EEPROM_init();
void EEPROM_Save();
void EEPROM_Read();


#endif 

