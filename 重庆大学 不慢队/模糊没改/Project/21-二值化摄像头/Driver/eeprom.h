#ifndef _EEPROM_H_
#define _EEPROM_H_

#define SECTOR_NUM  (FLASH_SECTOR_NUM-1) 
#define BLOCK_SIZE  64
#define BLOCK_NUM   FLASH_SECTOR_SIZE/BLOCK_SIZE

void EEPROM_init();
void EEPROM_Save();
void EEPROM_Read();

#endif /* _EEPROM_H_ */