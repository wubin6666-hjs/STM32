#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include  "MPU6050.h"
MPU6050_Raw mpu_data; 
int main(void)
{

	OLED_Init();
	MPU6050_Init();
	uint8_t ID;
	ID=MPU6050_GetID();
//	MPU6050_WriteByte(0x6B, 0x00);
//	MPU6050_WriteByte(0x19, 0x66);
//	uint8_t Data[10]={0};
//	uint8_t ret;
//	ret =MPU6050_Read(0x19,Data,1);
//	MPU6050_Read(0x19,Data,1);
//	OLED_ShowNum(1,1,ret,3);
//	OLED_ShowHexNum(2,1,Data[0],3);
//	OLED_ShowNum(3,1,Data[1],3);
	while (1)
	{
		
		 if(MPU6050_GetValue(&mpu_data) == 0)
        {
            //通信成功，直接访问结构体成员
            //加速度
            mpu_data.ax;
            mpu_data.ay;
            mpu_data.az;
            //温度
            mpu_data.temp;
            //陀螺仪
            mpu_data.gx;
            mpu_data.gy;
            mpu_data.gz;
        }
		OLED_ShowString(1,1,"ax:");
		OLED_ShowString(2,1,"ay:");
		OLED_ShowString(3,1,"az:");
		OLED_ShowSignedNum(1,5,mpu_data.ax,6);
		OLED_ShowSignedNum(2,5,mpu_data.ay,6);
		OLED_ShowSignedNum(3,5,mpu_data.az,6);
		OLED_ShowHexNum(4,1,ID,3);
	}
}
