#include "stm32f10x.h"                  // Device header
uint8_t counts;
void count_Init(void)
{
	//配置GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//开启时钟；
	GPIO_InitTypeDef GPIO_Structure;//定义结构体
	GPIO_Structure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Structure.GPIO_Pin=GPIO_Pin_14;
	GPIO_Structure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_Structure);
	//配置AFIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//开启时钟；
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource14);//在EXTI14号线上选取GPIOB作为输入
	//配置EXTI外部中断（不需要手动开启时钟
	EXTI_InitTypeDef EXTI_InitStructure;//定义结构体
	EXTI_InitStructure.EXTI_Line= EXTI_Line14;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);
	//配置NVIC（不需要手动开启时钟）
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//确定中断分组
	NVIC_InitTypeDef NVIC_InitStructure;//定义结构体
	NVIC_InitStructure.NVIC_IRQChannel=EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructure);
} 

uint8_t get()
{
	return counts;
}
void EXTI15_10_IRQHandler(void)
{
		if (EXTI_GetITStatus(EXTI_Line14)==SET)//判断标志位
		{
			counts++;
			EXTI_ClearFlag(EXTI_Line14);//清除标志位
		}
			
}
