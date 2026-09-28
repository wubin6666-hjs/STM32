#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "..\Hardware\LED.h"
#include "..\Hardware\key.h"
#include "OLED.h"
#include "count.h"
#include <stdint.h>
#include "MYDMA.h"
#include "Serial.h"


int main (void)
{
	OLED_Init();
	Serial_Init();
//	Serial_SendByte(0x41);
	uint8_t res_data;
	res_data=USART1_ReadByte();
	OLED_ShowHexNum(1,1,res_data,2);
	while (1)
	{
		
		
	
	}
}

