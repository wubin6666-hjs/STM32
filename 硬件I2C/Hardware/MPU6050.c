#include "stm32f10x.h"                  // Device header
#include "MPU6050_Reg.h"
#include "MPU6050.h" 


#define I2C_TIMEOUT  20000  // 事件超时阈值，72MHz主频下足够

/* 强制复位I2C总线，解决上电BUSY锁死的经典问题 */
static void I2C2_BusReset(void)
{
    // 1. 关闭I2C外设
    I2C_Cmd(I2C2, DISABLE);
    
    // 2. 引脚切为普通开漏输出，手动模拟时钟释放总线
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    // 先拉高SDA、SCL
    GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);
    for(volatile uint32_t j=0; j<200; j++);
    
    // 发9个时钟脉冲，强制从机释放SDA
    for(uint8_t i=0; i<9; i++)
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_10);
        for(volatile uint32_t j=0; j<100; j++);
        GPIO_SetBits(GPIOB, GPIO_Pin_10);
        for(volatile uint32_t j=0; j<100; j++);
    }
    
    // 3. 引脚切回复用开漏
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    // 4. 软件复位I2C外设寄存器
    I2C_SoftwareResetCmd(I2C2, ENABLE);
    I2C_SoftwareResetCmd(I2C2, DISABLE);
}
uint8_t MPU6050_WriteByte(uint8_t address,uint8_t Data)
{
	uint32_t timeout;
	I2C_GenerateSTART(I2C2,ENABLE);
	timeout = I2C_TIMEOUT;
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS)//ev5
	if((timeout--) == 0) { I2C_GenerateSTOP(I2C2, ENABLE); return 1; }
	
	I2C_Send7bitAddress(I2C2,0xD0,I2C_Direction_Transmitter);
	timeout = I2C_TIMEOUT;
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS)//ev6
	if((timeout--) == 0) { I2C_GenerateSTOP(I2C2, ENABLE); return 1; }
	

	I2C_SendData(I2C2,address);
	timeout = I2C_TIMEOUT;
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTING) != SUCCESS)//ev8
	if((timeout--) == 0) { I2C_GenerateSTOP(I2C2, ENABLE); return 1; }
		
	I2C_SendData(I2C2,Data);
	timeout = I2C_TIMEOUT;
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS)//ev8_2
	if((timeout--) == 0) { I2C_GenerateSTOP(I2C2, ENABLE); return 1; }

	I2C_GenerateSTOP(I2C2,ENABLE);
	return 0;
}
uint8_t MPU6050_Write(uint8_t address,uint8_t *Data,uint8_t len)//指定地址写
{
	uint32_t timeout;
	if(Data == 0 || len == 0)
	{
		return 1;
	}
	uint8_t i;
	
    I2C_GenerateSTART(I2C2,ENABLE);
	timeout = I2C_TIMEOUT;
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS)//ev5
	if((timeout--) == 0) { I2C_GenerateSTOP(I2C2, ENABLE); return 1; }
	
	I2C_Send7bitAddress(I2C2,0xD0,I2C_Direction_Transmitter);
	timeout = I2C_TIMEOUT;
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS)//ev6
	

	I2C_SendData(I2C2,address);
	timeout = I2C_TIMEOUT;
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTING) != SUCCESS)//ev8
	if((timeout--) == 0) { I2C_GenerateSTOP(I2C2, ENABLE); return 1; }
	
	for(i=0;i<len;i++)
	{
		I2C_SendData(I2C2,Data[i]);
		if(i != len-1)
			{
				while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTING) != SUCCESS)//ev8
				if((timeout--) == 0) { I2C_GenerateSTOP(I2C2, ENABLE); return 1; }
			}
			else
				{
					I2C_SendData(I2C2,Data[i]);
				    while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS)//ev8_2
					if((timeout--) == 0) { I2C_GenerateSTOP(I2C2, ENABLE); return 1; }
				}
           
		
	}	
	I2C_GenerateSTOP(I2C2,ENABLE);
	return 0;
}

uint8_t MPU6050_Read(uint8_t address,uint8_t*Data,uint8_t len)//指定地址读
{
	uint32_t timeout;
	if(Data == 0 || len == 0)
	{
		return 1;
	}
	I2C_GenerateSTART(I2C2,ENABLE);
	timeout = I2C_TIMEOUT;
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS)//ev5
	if((timeout--) == 0) { I2C_GenerateSTOP(I2C2, ENABLE); return 1; }
	I2C_Send7bitAddress(I2C2,0xD0,I2C_Direction_Transmitter);
	timeout = I2C_TIMEOUT;
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS)//ev6
	if((timeout--) == 0) { I2C_GenerateSTOP(I2C2, ENABLE); return 1; }

	I2C_SendData(I2C2,address);
	timeout = I2C_TIMEOUT;
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS)//ev8
	if((timeout--) == 0) { I2C_GenerateSTOP(I2C2, ENABLE); return 1; }
	I2C_GenerateSTART(I2C2,ENABLE);//sr
	timeout = I2C_TIMEOUT;
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS)//ev5
		if((timeout--) == 0) { I2C_GenerateSTOP(I2C2, ENABLE); return 1; }
	I2C_Send7bitAddress(I2C2,0xD0,I2C_Direction_Receiver);
	timeout = I2C_TIMEOUT;
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) != SUCCESS)//ev6
		if((timeout--) == 0) { I2C_GenerateSTOP(I2C2, ENABLE); return 1; }
	for(uint8_t i=0;i<len-1;i++)
		{
			timeout = I2C_TIMEOUT;
			while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED) != SUCCESS)
			if((timeout--) == 0) { I2C_GenerateSTOP(I2C2, ENABLE); return 1; }
			Data[i]=I2C_ReceiveData(I2C2);
			if(i == len - 2)
            {
                I2C_AcknowledgeConfig(I2C2, DISABLE);
                I2C_GenerateSTOP(I2C2, ENABLE);
            }
		}
	timeout = I2C_TIMEOUT;
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED) != SUCCESS)
		if((timeout--) == 0) { I2C_GenerateSTOP(I2C2, ENABLE); return 1; }
	Data[len-1]=I2C_ReceiveData(I2C2);
	I2C_AcknowledgeConfig(I2C2,ENABLE);
	
	return 0;

}

uint8_t MPU6050_ReadByte(uint8_t address)//指定地址读
{
	uint32_t timeout;
	uint8_t Data;
	
	I2C_GenerateSTART(I2C2,ENABLE);
	timeout = I2C_TIMEOUT;
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS)//ev5
	if((timeout--) == 0) { I2C_GenerateSTOP(I2C2, ENABLE); return 1; }
	I2C_Send7bitAddress(I2C2,0xD0,I2C_Direction_Transmitter);
	timeout = I2C_TIMEOUT;
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS)//ev6
	if((timeout--) == 0) { I2C_GenerateSTOP(I2C2, ENABLE); return 1; }

	I2C_SendData(I2C2,address);
	timeout = I2C_TIMEOUT;
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS)//ev8
	if((timeout--) == 0) { I2C_GenerateSTOP(I2C2, ENABLE); return 1; }
	I2C_GenerateSTART(I2C2,ENABLE);//sr
	timeout = I2C_TIMEOUT;
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS)//ev5
		if((timeout--) == 0) { I2C_GenerateSTOP(I2C2, ENABLE); return 1; }
	I2C_Send7bitAddress(I2C2,0xD0,I2C_Direction_Receiver);
	timeout = I2C_TIMEOUT;
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) != SUCCESS)//ev6
		if((timeout--) == 0) { I2C_GenerateSTOP(I2C2, ENABLE); return 1; }
	
	I2C_AcknowledgeConfig(I2C2,DISABLE);
	I2C_GenerateSTOP(I2C2,ENABLE);
	
	timeout = I2C_TIMEOUT;
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED) != SUCCESS)//ev7
		if((timeout--) == 0) { I2C_GenerateSTOP(I2C2, ENABLE); return 1; }
	Data=I2C_ReceiveData(I2C2);
	I2C_AcknowledgeConfig(I2C2,ENABLE);
		return Data;
}

uint8_t MPU6050_GetID(void)
{
	return MPU6050_ReadByte(MPU6050_WHO_AM_I);
}
void MPU6050_Init(void)
{

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	I2C2_BusReset();
	
	I2C_InitTypeDef I2C_InitStructure;
	I2C_InitStructure.I2C_Ack=I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress_7bit ;
	I2C_InitStructure.I2C_ClockSpeed=50000;
	I2C_InitStructure.I2C_DutyCycle=I2C_DutyCycle_2;
	I2C_InitStructure.I2C_Mode=I2C_Mode_I2C;
	I2C_InitStructure.I2C_OwnAddress1=0x00;
	I2C_Init(I2C2,&I2C_InitStructure);
	I2C_Cmd(I2C2,ENABLE);

	MPU6050_WriteByte(MPU6050_PWR_MGMT_1,0x01);//解除睡眠，选择陀螺仪时钟
	MPU6050_WriteByte(MPU6050_PWR_MGMT_2,0x00);//六轴不待机
	MPU6050_WriteByte(MPU6050_SMPLRT_DIV,0x00);//采样频率分频系数
	MPU6050_WriteByte(MPU6050_CONFIG,0x06);//配置寄存器
	MPU6050_WriteByte(MPU6050_GYRO_CONFIG,0x18);//陀螺仪配置寄存器
	MPU6050_WriteByte(MPU6050_ACCEL_CONFIG,0x18);//加速度计配置寄存器
}

uint8_t MPU6050_GetValue(MPU6050_Raw *Data)
{
	if(Data == 0)
	{
		return 1;
	}
	uint8_t buf[14];
	if(MPU6050_Read(0x3B,buf,14) == 1)
	{return 1;}
	Data->ax=(int16_t)(buf[0] << 8 | buf[1]);
	Data->ay=(int16_t)(buf[2] << 8 | buf[3]);
	Data->az=(int16_t)(buf[4] << 8 | buf[5]);
	Data->temp=(int16_t)(buf[6] << 8 | buf[7]);
	Data->gx=(int16_t)(buf[8] << 8 | buf[9]);
	Data->gy=(int16_t)(buf[10] << 8 | buf[11]);
	Data->gz=(int16_t)(buf[12] << 8 | buf[13]);
	return 0;
}