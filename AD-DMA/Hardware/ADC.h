#ifndef __ADC_H
#define __ADC_H
#include <stdint.h>
void ADCC_Init(void);
void ADC_GetValue(void);
extern uint16_t ADC_Vaule[4];
void USART_SendString(char *str);
#endif
