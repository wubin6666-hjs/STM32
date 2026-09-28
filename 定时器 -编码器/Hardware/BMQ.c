#include "stm32f10x.h"                  // Device header
void BMQ_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	TIM_EncoderInterfaceConfig(TIM2,TIM_EncoderMode_TI12,
                                TIM_ICPolarity_Falling, TIM_ICPolarity_Falling);
	TIM_PrescalerConfig(TIM2,0,TIM_PSCReloadMode_Update);
	TIM_SetAutoreload(TIM2, 100);
//	TIM_InternalClockConfig(TIM2);
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
//	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
//	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
//	TIM_TimeBaseInitStructure.TIM_Period=10000-1;
//	TIM_TimeBaseInitStructure.TIM_Prescaler=720-1;
//	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;
//	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	TIM_Cmd(TIM2,ENABLE);
	//
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	//
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructure);
	
}


volatile int32_t overflow_count = 0;   // 溢出次数（带方向）

// TIM2 中断：记录溢出
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        // 根据当前计数器值判断溢出方向
        if (TIM_GetCounter(TIM2) < (100 / 2))
            overflow_count++;      // 正转溢出（从 ARR 到 0）
        else
            overflow_count--;      // 反转溢出（从 0 到 ARR）
        
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}

// 获取总脉冲数（这就是你要记录的核心数据）
int32_t GetTotalPulse(void)
{
    return overflow_count * 100 + TIM_GetCounter(TIM2);
}

