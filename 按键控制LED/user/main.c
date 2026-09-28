#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "..\Hardware\LED.h"
#include "..\Hardware\key.h"
#include "OLED.h"
#include "count.h"
#include <stdint.h>


int main (void)
{
	count_Init();
	OLED_Init();
	OLED_ShowString(1,1,"count:");
	while (1)
	{
		OLED_ShowNum(1,7,get(),5);
	}
}

