#include "stm32f10x.h"                  // Device header
#include "MyI2C.h"
#include "MPU6050_Reg.h"
#include "MPU6050.h" 
uint8_t MPU6050_WriteByte(uint8_t address,uint8_t Data)
{
	MyI2C_Start();
	MyI2C_SendByte(0xD0);
	if(MyI2C_ReceiveACK() == 1)
	{
		MyI2C_Stop();
		return 1;
	}
	MyI2C_SendByte(address);
	if(MyI2C_ReceiveACK() == 1)
	{
		MyI2C_Stop();
		return 1;
	}
	MyI2C_SendByte(Data);
	if(MyI2C_ReceiveACK() == 1)
	{
		MyI2C_Stop();
		return 1;
	}
	MyI2C_Stop();
	return 0;
}
uint8_t MPU6050_Write(uint8_t address,uint8_t *Data,uint8_t len)//指定地址写
{
	if(Data == 0 || len == 0)
	{
		return 1;
	}
	uint8_t i=0;
	MyI2C_Start();
	MyI2C_SendByte(0xD0);
	if(MyI2C_ReceiveACK() == 1)
	{
		MyI2C_Stop();
		return 1;
	}
	
	MyI2C_SendByte(address);
	if(MyI2C_ReceiveACK() == 1)
	{
		MyI2C_Stop();
		return 1;
	}
	
	for(i=0;i<len;i++)
	{
	MyI2C_SendByte(Data[i]);
	
	if(MyI2C_ReceiveACK() == 1)
	{
		MyI2C_Stop();
		return 1;
	}
	
	}
	MyI2C_Stop();
	return 0;
}

uint8_t MPU6050_Read(uint8_t address,uint8_t*Data,uint8_t len)//指定地址读
{
	if(Data == 0 || len == 0)
	{
		return 1;
	}
	MyI2C_Start();
	
	MyI2C_SendByte(0xD0);//写
	if(MyI2C_ReceiveACK() == 1)
	{
		MyI2C_Stop();
		return 1;
	}
	
	MyI2C_SendByte(address);
	if(MyI2C_ReceiveACK() == 1)
	{
		MyI2C_Stop();
		return 1;
	}
	
	MyI2C_Start();//sr
	
	MyI2C_SendByte(0xD1);//读
	if(MyI2C_ReceiveACK() == 1)
	{
		MyI2C_Stop();
		return 1;
	}
	
	for(uint8_t i=0;i<len;i++)
	{
		
	if(i<len-1)
	{
	Data[i]=MyI2C_Receive();
	MyI2C_SendACK(0);
	}
	else
    {
		Data[i]=MyI2C_Receive();
		MyI2C_SendACK(1);
	}
	
	}
	
	MyI2C_Stop();
	return 0;
}

uint8_t MPU6050_ReadByte(uint8_t address)//指定地址读
{
	uint8_t data;
	MyI2C_Start();
	MyI2C_SendByte(0xD0);//写
	if(MyI2C_ReceiveACK() == 1)
	{
		MyI2C_Stop();
		return 1;
	}
	
	MyI2C_SendByte(address);
	if(MyI2C_ReceiveACK() == 1)
	{
		MyI2C_Stop();
		return 1;
	}
	
	MyI2C_Start();//sr
	
	MyI2C_SendByte(0xD1);//读
	if(MyI2C_ReceiveACK() == 1)
	{
		MyI2C_Stop();
		return 1;
	}
	data=MyI2C_Receive();
	MyI2C_SendACK(0);
	MyI2C_Stop();
	return data;
}

uint8_t MPU6050_GetID(void)
{
	return MPU6050_ReadByte(MPU6050_WHO_AM_I);
}
void MPU6050_Init(void)
{
	MyI2C_Init();
	//寄存器初始化配置
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