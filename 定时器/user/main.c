#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "..\Hardware\LED.h"
#include "..\Hardware\key.h"
#include "OLED.h"
#include "count.h"
#include <stdint.h>
#include "TIMER.h"
uint16_t num;

int main (void)
{
	OLED_Init();
	OLED_ShowString(1,1,"num");
	TimerInit();
	while (1)
	{	
		OLED_ShowNum(1,5,num,10);
	}
}

