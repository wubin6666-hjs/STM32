#include "stm32f10x.h"                  // Device header
#include "Delay.h"
void key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure1;
	GPIO_InitStructure1.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure1.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_11;
	GPIO_InitStructure1.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure1);

}
uint8_t key_Getnum(void)
{
	uint8_t keynum=0;
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)==0)
	{
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)==0);
		Delay_ms(20);
		keynum =1;
		
	}
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==0)
	{
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==0);
		Delay_ms(20);
		keynum =2;
		
	}
	return keynum;
}
