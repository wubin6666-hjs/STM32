#include "stm32f10x.h"                  // Device header
extern uint16_t num;
void AD_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;//模拟输入
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init (GPIOA,&GPIO_InitStructure);
	//
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	//配置ADC模块
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//配置ADCCCLK
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,1,ADC_SampleTime_7Cycles5);//ADCX，通道与引脚有关，次序1-16，采样时间
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_ContinuousConvMode=DISABLE;
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;//数据对齐
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;//触发源选择
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;
	ADC_InitStructure.ADC_NbrOfChannel=1;
	ADC_InitStructure.ADC_ScanConvMode=DISABLE;
	ADC_Init( ADC1,&ADC_InitStructure);
	ADC_Cmd(ADC1,ENABLE);//启动ADC上电
	
	//ADC校准
	ADC_ResetCalibration(ADC1);//开始复位，标志位置1
	while(ADC_GetResetCalibrationStatus(ADC1)==SET);//ADC_GetResetCalibrationStatus获取状态，如果是1，则一直在循环，置0则跳出循环，说明复位完成
	 ADC_StartCalibration(ADC1);//启动校准
	 while(ADC_GetCalibrationStatus(ADC1)==SET);//和复位相同
	 
	 ADC_SoftwareStartConvCmd(ADC1,ENABLE);//软件触发，开始转换
	 while(ADC_GetITStatus(ADC1,ADC_IT_EOC)==RESET);//获取标志位
	 num=ADC_GetConversionValue(ADC1);
}
