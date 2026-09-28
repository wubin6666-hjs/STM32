#include "stm32f10x.h"                  // Device header

uint16_t MYDMAsize;

void MYDMA_Init(uint32_t AddrA,uint32_t AddrB,uint32_t size)
{
	MYDMAsize=size;
	  
}
void MYDMA_Transfer(void)
{
	DMA_Cmd(DMA1_Channel1,DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel1,MYDMAsize); 
	DMA_Cmd(DMA1_Channel1,ENABLE);
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1)==RESET);
	DMA_ClearFlag(DMA1_FLAG_TC1);	
}

