#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "..\Hardware\LED.h"
#include "..\Hardware\key.h"
#include "OLED.h"
#include "count.h"
#include <stdint.h>
#include "TIMER.h"
#include "pwm.h"
uint16_t num;
uint16_t pulse=50;
int16_t d=1;
int main (void)
{
	OLED_Init();
	PwmInit();
	while (1)
	{	
		
	}
}

