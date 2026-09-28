#include "stm32f10x.h"                  // Device header
#include "Delay.h"
void SCL_W(uint8_t Bitvalue)
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_10,(BitAction)Bitvalue);
	Delay_us(10);
}
void SDA_W(uint8_t Bitvalue)
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_11,(BitAction)Bitvalue);
	Delay_us(10);
}
 uint8_t  SDA_R(void)
{
	uint8_t Bitvalue;
	Bitvalue=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11);
	Delay_us(10);
	return Bitvalue;
}
void MyI2C_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_10 | GPIO_Pin_11);
}
void MyI2C_Start(void)
{
	SDA_W(1);
	SCL_W(1);
	SDA_W(0);
//	Delay_us(10); 
	SCL_W(0);
	
}
void MyI2C_Stop(void)
{
	SDA_W(0);
	SCL_W(1);
	SDA_W(1);
}
void MyI2C_SendByte(uint8_t Byte)
{
	for(uint8_t i=0;i<8;i++)
	{
	SDA_W(Byte &(0x80 >> i));
//	Delay_us(5);
	SCL_W(1);
//	Delay_us(5);
	SCL_W(0);
	}
	
}
uint8_t MyI2C_Receive(void)
{
	uint8_t Byte=0x00,i;
	SDA_W(1);
	for(i=0;i<8;i++)
	{
	SCL_W(1);
	if(SDA_R() == 1){Byte |= (0x80 >> i);}
	SCL_W(0);
	}
	return Byte;
}
void MyI2C_SendACK(uint8_t ACK)
{
	SDA_W(ACK);
	SCL_W(1);
	SCL_W(0);
}
uint8_t MyI2C_ReceiveACK(void)
{
	uint8_t ACK;
	SDA_W(1);
	SCL_W(1);
	ACK=SDA_R();
	SCL_W(0);
	return ACK;
}