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
//	Serial_SendByte('A');
//	uint8_t a[10]={0x31,1,2,3,4,5};
//	Serial_SendArry(a,10);
//	uint8_t buf[] = "ceshi";
//	Serial_SendString("你好\r\n");
//	Serial_SendString("a");
//	Serial_SendNumber(20);
//	printf("number=%d\r\n",666);
//	uint8_t string[100];
//	sprintf((char *)string,"number=%d\r\n",666);
//	Serial_SendString(string);
	Serial_Printf("val=%d%c\r\n",666,'n');
	
	while (1)
	{
		
		
	
	}
}

