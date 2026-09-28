#ifndef __MYDMA_H
#define __MYDMA_H
#include <stdint.h>
void MYDMA_Init(uint32_t AddrA,uint32_t AddrB,uint32_t size);
void MYDMA_Transfer(void);
#endif
