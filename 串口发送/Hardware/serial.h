#ifndef __SERIAL_H
#define __SERIAL_H
#include <stdio.h>
void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArry(uint8_t *arry,uint16_t length);
void Serial_SendString(uint8_t *buf);
void Serial_Digit(uint16_t n);
void Serial_SendNumber(uint32_t number);
int fputc(int ch,FILE *f);
void Serial_Printf(const char *fmt,... );
uint8_t USART1_ReadByte(void);
#endif

