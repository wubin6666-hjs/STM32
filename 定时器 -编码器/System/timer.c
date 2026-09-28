#include "stm32f10x.h"                  // Device header
#include <stdint.h>
extern uint16_t num;
void TimerInit(void)
	{//定时器时基单元：计数器，PSC预分频器，ARR自dongle重装器，重复计数器（高级定时器）
//	配置定时器步骤：
//		1：开启要使用的定时器时钟（RCC_APB1PeriphClockCmd)
//      2:配置时基单元时钟：
//	    内部时钟： TIM_InternalClockConfig
//	    外部时钟2：TIM_ETRClockMode2Config	ETR直通，不经过触发输入控制器
//      外部时钟1：TIM_ITRxExternalClockConfig （其他定时器的ITRx信号定时器级联，从模式定时器跟随主定时器）
//                 TIM_TIxExternalClockConfig	 （外部输入通道（TI1/TI2），通过IO引脚输入外部时钟信号）
//                 TIM_ETRClockMode1Config	外部触发输入（ETR）	模式1：ETR信号需经过触发输入控制器
//	    3：配置时基单元：TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure；TIM_TimeBaseInit；
//                     利用结构体传递参数配置时基单元。
//      4:使能更新中断：TIM_ITConfig
//      5：配置NVIC：确定优先级分组 NVIC_PriorityGroupConfig
//                   初始化NVIC     NVIC_InitTypeDef NVIC_InitStructure
//                                  NVIC_Init(&NVIC_InitStructure)
//      6：启动定时器：
		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//开启tim2的时钟
		
	TIM_InternalClockConfig(TIM2);//配置时基单元的时钟，用内部时钟作为tim2的时钟
		
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_Channel_1;//指定时钟分频，如：一分频，二分频等，确定滤波器频率
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//计数器模式，向上，向下计数，中央对齐计数等
	TIM_TimeBaseInitStructure.TIM_Period= 7200 - 1;//周期，ARR自动重装器的值
	TIM_TimeBaseInitStructure.TIM_Prescaler=10000 - 1;//PSC预分频器的值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter= 0;
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn ;//中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM2,ENABLE);
}
	

////void TIM2_IRQHandler(void)
//{
//	if (TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)
//	{
//		num++;
//		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
//	}
//}

