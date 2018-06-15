#ifndef __SCOPE_H_
#define __SCOPE_H_ 1
#include "common.h"

extern void DataOutPut(void);
extern short int OutData[8];
void Send_Scope(void *wareaddr, uint32_t waresize);
#endif