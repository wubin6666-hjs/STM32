#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "..\Hardware\LED.h"
#include "..\Hardware\key.h"
#include "OLED.h"
#include "My_USART.h"
#include <stdint.h>
#include "ADC.h"
#include "My_ADC.h"
#include "FIFO.h"
static uint16_t last_adc2 = 0;
static uint8_t  first = 1;
int main (void)
{
	uint16_t adc0 = 0;
    uint16_t adc1 = 0;
    uint16_t adc2 = 0;
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    My_usart_Init();
    OLED_Init();
    My_ADC_Init();
    
    OLED_Clear();
    OLED_ShowString(1, 1, "ADC0:");
    OLED_ShowString(2, 1, "ADC1:");
    OLED_ShowString(3, 1, "DMA[0]:");
	OLED_ShowString(4,1,"ADC2(INJ):");
	
	
	while (1)
	{
		adc0 = Get_ADC_Regular_Average(0);
        adc1 = Get_ADC_Regular_Average(1);
        adc2 = Get_InjectedAdc();
        OLED_ShowNum(1, 7, adc0, 4);
        OLED_ShowNum(2, 7, adc1, 4);
    // 注意：循环DMA下 adc_dma_buffer[0] 会不停被覆盖，不建议直接显示它
        OLED_ShowNum(3, 8, adc_dma_buffer[0], 4);
		Inject_ADC_Threshold_Check(adc2);
        OLED_ShowNum(4, 12, adc2, 4);
		if (first || adc2 != last_adc2)
			{
				first = 0;
				last_adc2 = adc2;
				Serial_SendString("INJ=");
				Serial_SendNumber(adc2);
				Serial_SendString("\r\n");
			}
		Cmd_Poll(); 
        Delay_ms(100);
		
	}
}

