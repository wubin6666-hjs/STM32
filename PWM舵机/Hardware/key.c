#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include <stdint.h>
u16 count=0;
u16 num=500;
u16 t=499;
void key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure1;
	GPIO_InitStructure1.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure1.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStructure1.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure1);

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
uint16_t key1()
{
    // 检测按键 PB0 是否按下
    if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0)  // 按下为低电平
    {
        Delay_ms(200);  // 消抖
        if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0)
        {
            // 等待按键释放
            while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0);
            Delay_ms(200);
            
            // 修改 PWM 占空比
            if(count < 5)
            {
                t += num;
                count += 1;
                TIM_SetCompare1(TIM2, t);
            }
            else
            {
                count = 0;
                t = 499;
                TIM_SetCompare1(TIM2, t);
            }
        }
    }
    return 0;  // 添加返回值
}
