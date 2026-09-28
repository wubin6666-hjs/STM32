#ifndef __ADC_H
#define __ADC_H
#include <stdint.h>
void ADCC_Init(void);
uint16_t ADC_GetValue(uint8_t ADC_Channel);
#endif
