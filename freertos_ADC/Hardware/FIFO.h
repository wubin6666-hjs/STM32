#ifndef __FIFO_H
#define __FIFO_H
#define buffersize 16
typedef struct
	{
		uint16_t head;
		uint16_t tail;
		uint16_t count;
		uint16_t buffer[buffersize];
	}Typedef_FIFO;
void FIFO_Init(Typedef_FIFO *fifo);
uint16_t FIFO_Pop(Typedef_FIFO *fifo);
uint16_t FIFO_Push(Typedef_FIFO *fifo,uint16_t data);
#endif