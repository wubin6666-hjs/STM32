#include "stm32f10x.h"                  // Device header
#include "FIFO.h"
//#define buffersize 16
//typedef struct
//	{
//		uint16_t head;
//		uint16_t tail;
//		uint16_t count;
//		uint16_t buffer[buffersize];
//	}Typedef_FIFO;

void FIFO_Init(Typedef_FIFO *fifo)
{
	fifo->count=0;
	fifo->head=0;
	fifo->tail=0;
}

uint16_t FIFO_Push(Typedef_FIFO *fifo,uint16_t data)
{
	if(fifo->count>=buffersize)
	{
		return 0;
	}
	fifo->buffer[fifo->head]=data;
	fifo->head=(fifo->head+1)%buffersize;
	fifo->count++;
	return 1;
}
uint16_t FIFO_Pop(Typedef_FIFO *fifo)
{
	if(fifo->count==0)
	{
		return 0;
	}
	uint16_t data=fifo->buffer[fifo->tail];
	fifo->tail=(fifo->tail+1)%buffersize;
	fifo->count--;
	return data;
}
