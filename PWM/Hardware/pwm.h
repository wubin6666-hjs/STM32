#ifndef __PWM_H
#define __PWM_H
#include <stdint.h>
void PwmInit(void);

void Pwm_SetPrescaler(uint16_t PSC);
#endif
