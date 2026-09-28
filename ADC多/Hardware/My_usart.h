#ifndef __MY_USART_H
#define __MY_USART_H
void Serial_SendByte(uint8_t Byte);
void My_usart_Init(void);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString(char *String);
void USART1_IRQHandler(void);
void Cmd_Poll(void);
void Serial_SendNumber(uint16_t num);
#endif