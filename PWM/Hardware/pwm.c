#include "stm32f10x.h"                  // Device header
void PwmInit(void)
{
//      1：开启要使用的定时器时钟（RCC_APB1PeriphClockCmd)
//      2:配置时基单元时钟：
//	    内部时钟： TIM_InternalClockConfig
//	    外部时钟2：TIM_ETRClockMode2Config	ETR直通，不经过触发输入控制器
//      外部时钟1：TIM_ITRxExternalClockConfig （其他定时器的ITRx信号定时器级联，从模式定时器跟随主定时器）
//                 TIM_TIxExternalClockConfig	 （外部输入通道（TI1/TI2），通过IO引脚输入外部时钟信号）
//                 TIM_ETRClockMode1Config	外部触发输入（ETR）	模式1：ETR信号需经过触发输入控制器
//	    3：配置时基单元：TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure；TIM_TimeBaseInit；
//                     利用结构体传递参数配置时基单元。
//重要参数：TIM_TimeBaseInitStructure.TIM_Period（ARR自动重装值）计数器频率/（ARR+1）=计数器溢出频率=触发中断频率=PWM频率
//TIM_TimeBaseInitStructure.TIM_Prescaler（PSC预分频器值）时钟源频率/(PSC+1)=COUNT计数器频率
//      4：初始化输出比较单元
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin= GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_InternalClockConfig(TIM2);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1 ;
	TIM_TimeBaseInitStructure.TIM_CounterMode= TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=100-1;
	TIM_TimeBaseInitStructure.TIM_Prescaler= 720-1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;//输出比较模式
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;//输出比较极性选择：1：high有效电平为高电平，2：low与1相反
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;//使能或失能捕获通道
	TIM_OCInitStructure.TIM_Pulse=50;//CCR
	TIM_OC1Init(TIM2,&TIM_OCInitStructure);
	
	TIM_Cmd(TIM2,ENABLE);
	
	
}

	void Pwm_SetPrescaler(uint16_t PSC)
	{
		TIM_PrescalerConfig(TIM2,PSC,TIM_PSCReloadMode_Immediate);
	}
	
