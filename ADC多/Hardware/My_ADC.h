#ifndef __My_ADC_H
#define __My_ADC_H
#define BUFFER_SIZE 128  // 在这里定义缓冲区大小
extern uint16_t adc_dma_buffer[BUFFER_SIZE];  // 声明外部变量
void My_ADC_Init(void);
void ADC1_2_IRQHandler(void);
void EXTI9_5_IRQHandler(void);
uint16_t Get_ADC_Regular_Average(uint8_t channel);
uint16_t Get_InjectedAdc(void);
void Inject_ADC_Threshold_Check(uint16_t inject_raw);
#endif