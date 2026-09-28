#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "..\Hardware\LED.h"
#include "..\Hardware\key.h"
#include "OLED.h"
#include "count.h"
#include <stdint.h>
#include "ADC.h"

extern uint16_t ADC_Vaule[4];
int main (void)
{
	
	OLED_Init();
	
	while (1)
	{
		
	}
}

