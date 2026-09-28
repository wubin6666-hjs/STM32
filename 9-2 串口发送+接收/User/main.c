#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "KEY.h"

uint8_t RxData;			//定义用于接收串口数据的变量
uint8_t KeyNum;

int main(void)
{
	/*模块初始化*/
	OLED_Init();		//OLED初始化
	/*串口初始化*/
	Serial_Init();		//串口初始化
	Key_Init();
	OLED_ShowString(1,1,"TxData");
	OLED_ShowString(3,1,"RxData");
	Serial_TxData[0]=0x01;
	Serial_TxData[1]=0x02;
	Serial_TxData[2]=0x03;
	Serial_TxData[3]=0x04;
//	Serial_SendData();
	
	while (1)
	{
		KeyNum=Key_GetNum();
		if(KeyNum==1)
		{
			Serial_SendData();
			Serial_TxData[0]++;
			Serial_TxData[1]++;
			Serial_TxData[2]++;
			Serial_TxData[3]++;
			OLED_ShowHexNum(2,1,Serial_TxData[0],2);
			OLED_ShowHexNum(2,4,Serial_TxData[1],2);
			OLED_ShowHexNum(2,7,Serial_TxData[2],2);
			OLED_ShowHexNum(2,10,Serial_TxData[3],2);
		}
		if(Serial_GetRxFlag()==1)
		{
			OLED_ShowHexNum(4,1,Serial_RxData[0],2);
			OLED_ShowHexNum(4,4,Serial_RxData[1],2);
			OLED_ShowHexNum(4,7,Serial_RxData[2],2);
			OLED_ShowHexNum(4,10,Serial_RxData[3],2);
		}

	}
}
