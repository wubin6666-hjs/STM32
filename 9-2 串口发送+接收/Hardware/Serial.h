#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>
extern uint8_t Serial_RxData[4];
extern uint8_t Serial_TxData[4];
void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString(char *String);
void Serial_SendNumber(uint32_t Number, uint8_t Length);
void Serial_Printf(char *format, ...);
void Serial_SendData(void);
uint8_t Serial_GetRxFlag(void);

#endif
