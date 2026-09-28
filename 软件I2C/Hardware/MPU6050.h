#ifndef __MPU6050_H
#define __MPU6050_H
#include "stm32f10x.h" 
typedef struct
{
	int16_t ax;
	int16_t ay;
	int16_t az;
	int16_t temp;
	int16_t gx;
	int16_t gy;
	int16_t gz;
	
}MPU6050_Raw;
void MPU6050_Init(void);
uint8_t MPU6050_Read(uint8_t address,uint8_t*Data,uint8_t len);//指定地址读;
uint8_t MPU6050_Write(uint8_t address,uint8_t *Data,uint8_t len);//指定地址写
uint8_t MPU6050_WriteByte(uint8_t address,uint8_t Data);
uint8_t MPU6050_GetValue(MPU6050_Raw *Data);
uint8_t MPU6050_ReadByte(uint8_t address);//指定地址读
uint8_t MPU6050_GetID(void);
#endif
