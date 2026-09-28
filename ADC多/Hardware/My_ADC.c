#include "stm32f10x.h"
#include "FIFO.h"
#include "My_ADC.h"
//volatile uint8_t EXTI_ADCsoft;
#define ADC_INJECT_THRESHOLD    2500U   //ADC阈值 0~4095，宏定义，不要魔法数字
uint16_t adc_dma_buffer[BUFFER_SIZE];
static Typedef_FIFO fifo;
static volatile uint16_t g_inject_latest = 0;
void My_ADC_Init(void)
{
	FIFO_Init(&fifo);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA| RCC_APB2Periph_AFIO,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOA, GPIO_Pin_11);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	// ==========关键修改：扫描+连续转换必须同时开启 ==========
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_ContinuousConvMode=DISABLE;   // 连续转换
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_T3_TRGO;
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;
	ADC_InitStructure.ADC_NbrOfChannel=2;
	ADC_InitStructure.ADC_ScanConvMode=ENABLE;        // 扫描模式
	ADC_Init(ADC1,&ADC_InitStructure);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);
	
	//注入通道CH2
	ADC_InjectedChannelConfig(ADC1,ADC_Channel_2,1,ADC_SampleTime_55Cycles5);
	ADC_InjectedSequencerLengthConfig(ADC1,1);
	ADC_ExternalTrigInjectedConvConfig(ADC1,ADC_ExternalTrigInjecConv_None);
	ADC_ExternalTrigInjectedConvCmd(ADC1,DISABLE);
	
	ADC_Cmd(ADC1, ENABLE);
	
	
	//ADC校准
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1)==SET);
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1)==SET);
	
	ADC_DMACmd(ADC1,ENABLE);
	
	ADC_ExternalTrigConvCmd(ADC1, ENABLE);
	
	//DMA1通道1
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_PeripheralBaseAddr=(uint32_t)&ADC1->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr=(uint32_t)adc_dma_buffer;
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize=BUFFER_SIZE;
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;
	DMA_InitStructure.DMA_Mode=DMA_Mode_Circular;  //循环模式
	DMA_InitStructure.DMA_Priority=DMA_Priority_High;
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);
	
	DMA_ClearFlag(DMA1_FLAG_TC1);
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	//PA5外部中断按键触发注入转换
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5);
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line=EXTI_Line5;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStructure);
	
	ADC_ITConfig(ADC1, ADC_IT_JEOC, ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel=ADC1_2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStructure);
	
	
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);
	TIM_Cmd(TIM3, ENABLE);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
//	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
//	NVIC_Init(&NVIC_InitStructure);

	//启动ADC规则组转换
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
	void Inject_ADC_Threshold_Check(uint16_t inject_raw)
	{
		if(inject_raw > ADC_INJECT_THRESHOLD)
		{
			GPIO_ResetBits(GPIOA, GPIO_Pin_11); //LED亮，超限
		}
		else
		{
			GPIO_SetBits(GPIOA, GPIO_Pin_11);   //LED灭，正常
		}
	}
void ADC1_2_IRQHandler(void)
{
	if(ADC_GetITStatus(ADC1,ADC_IT_JEOC)==SET)
	{
		ADC_ClearITPendingBit(ADC1,ADC_IT_JEOC);
		uint16_t v = ADC_GetInjectedConversionValue(ADC1, ADC_InjectedChannel_1);
        g_inject_latest = v;
        FIFO_Push(&fifo, v); 
	}
}

void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line5)==SET)
	{
		EXTI_ClearITPendingBit(EXTI_Line5);
		
		ADC_SoftwareStartInjectedConvCmd(ADC1,ENABLE);
	}
}
// 给外部调用，弹出数据
uint16_t Get_InjectedAdc(void)
{
    return g_inject_latest;
}

// 修复：删掉粘贴进来的测试代码，完整的平均函数
uint16_t Get_ADC_Regular_Average(uint8_t channel)
{
    uint32_t sum = 0;
    uint16_t count = 0;
    // CH0: i=0,2,4... ; CH1:i=1,3,5...
    for(int i = channel; i < BUFFER_SIZE; i += 2)
    {
        sum += adc_dma_buffer[i];
        count++;
    }
    if(count == 0) return 0;
    return (uint16_t)(sum / count);
}