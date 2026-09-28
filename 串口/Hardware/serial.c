#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
void Serial_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate=9600;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Tx;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);
	
	USART_Cmd(USART1,ENABLE);
}

void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1,Byte);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
}
void Serial_SendArry(uint8_t *arry,uint16_t length)
{
	for(uint16_t i=0;i<length;i++)
	{
		Serial_SendByte(arry[i]);
	}
}
	
void Serial_SendString(uint8_t *buf)
{
	while(*buf !='\0')//发送字符串 Serial_SendString：依靠结束符 '\0'（0）终止遍历，不需要传入长度
	{
		USART_SendData(USART1,*buf);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);//等待发送完成
		buf++;// 指针往后移动一个字节
		
	}
}

void Serial_Digit(uint16_t n)
{
	if(n==0)return;
	Serial_Digit(n/10);
	Serial_SendByte((n%10)+'0');
}
void Serial_SendNumber(uint32_t number)
{
	if(number==0)
	{Serial_SendByte('0');
	return;}
	Serial_Digit(number);
}

int fputc(int ch,FILE *f)
{
	Serial_SendByte(ch);
	return ch;
}
void Serial_Printf(const char *fmt,... )
{
	va_list ap;
	va_start(ap,fmt);
	char buf[64];
	vsnprintf(buf,sizeof(buf),fmt,ap);
	va_end(ap);
	Serial_SendString((uint8_t *)buf);
}


	
