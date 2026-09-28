#include "stm32f10x.h"                  // Device header
#include "FreeRTOS.h"
#include "task.h"
#include "Delay.h"
#include "..\Hardware\LED.h"
#include "..\Hardware\key.h"
#include "OLED.h"
#include "My_USART.h"
#include <stdint.h>
#include "ADC.h"
#include "My_ADC.h"
#include "FIFO.h"
#include "queue.h"
#include "semphr.h"
QueueHandle_t xCmdQueue;// 全局定义一个队列句柄
SemaphoreHandle_t xUartMutex;
//static uint16_t last_adc2 = 0;
//static uint8_t  first = 1;
void TestTask(void *pvParameters)
{
    while (1)
    {
        LED1_on();                       // 点亮 LED
        vTaskDelay(pdMS_TO_TICKS(500));  // 延时 500ms
        LED1_off();                      // 熄灭 LED（如果函数名不同，自己替换）
        vTaskDelay(pdMS_TO_TICKS(500));  // 延时 500ms
    }
}
void CommandTask(void *pvParameters)
{
	char cmd[32];
	while(1)
	{
		if(xQueueReceive(xCmdQueue,cmd,portMAX_DELAY) == pdTRUE)
		{
			Cmd_Pocess(cmd);
		}
	}
}
void Task_Display(void *pvParameters)
{
	uint16_t adc0 = 0;
    uint16_t adc1 = 0;
    uint16_t adc2 = 0;
	uint16_t last_adc2 = 0;
    uint8_t  first = 1;
  
    OLED_Clear();
    OLED_ShowString(1, 1, "ADC0:");
    OLED_ShowString(2, 1, "ADC1:");
    OLED_ShowString(3, 1, "DMA[0]:");
	OLED_ShowString(4,1,"ADC2(INJ):");
	while(1)
	{
		taskENTER_CRITICAL();
		adc0 = Get_ADC_Regular_Average(0);
        adc1 = Get_ADC_Regular_Average(1);
		taskEXIT_CRITICAL();
		
        adc2 = Get_InjectedAdc();
		
        OLED_ShowNum(1, 7, adc0, 4);
        OLED_ShowNum(2, 7, adc1, 4);
		
        OLED_ShowNum(3, 8, adc_dma_buffer[0], 4);
		
		Inject_ADC_Threshold_Check(adc2);
		
        OLED_ShowNum(4, 12, adc2, 4);
		
		if (first || adc2 != last_adc2)
			{
				first = 0;
				last_adc2 = adc2;
				// 发送前获取互斥量（防止和命令回复冲突）
            if(xSemaphoreTake(xUartMutex, pdMS_TO_TICKS(10)) == pdTRUE)
				{
					Serial_SendString("INJ=");
					Serial_SendNumber(adc2);
					Serial_SendString("\r\n");
					xSemaphoreGive(xUartMutex);
				}
			}
		
		vTaskDelay(pdMS_TO_TICKS(100));
	}
}
void StartTask(void *pvParameters)
{

    // ① 第一步：创建所有内核对象（队列、互斥量）
    // 必须在开中断之前创建完成
    xCmdQueue = xQueueCreate(5, 32);
    xUartMutex = xSemaphoreCreateMutex();

    // ② 第二步：初始化所有硬件外设
    LED_Init();
    OLED_Init();
    My_ADC_Init();
    My_usart_Init();  // 开串口接收中断，此时队列已就绪

    // ③ 第三步：创建所有业务任务
    xTaskCreate(TestTask,    "Test",    128,  NULL, 1, NULL);  // 闪灯
    xTaskCreate(CommandTask, "Command", 256,  NULL, 3, NULL);  // 串口命令
    xTaskCreate(Task_Display,"Disp",    256,  NULL, 2, NULL);  // ADC+OLED显示

    // ④ 第四步：初始化完成，删除启动任务自己
    vTaskDelete(NULL);
}
int main(void)
{
    // 优先级分组必须第一个设置（FreeRTOS 强制 Group 4）
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    // 只创建启动任务这一个任务
    xTaskCreate(StartTask, "Start", 256, NULL, 1, NULL);

    // 启动调度器
    vTaskStartScheduler();

    // 正常永远不会到这里
    while(1);
}


/* 当任务栈溢出时，会进入这里 */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    (void)xTask;
    (void)pcTaskName;
    taskDISABLE_INTERRUPTS();
    for(;;); // 死循环，方便调试器捕捉
}

/* 当堆内存分配失败时（比如 configTOTAL_HEAP_SIZE 设小了），会进入这里 */
void vApplicationMallocFailedHook(void)
{
    taskDISABLE_INTERRUPTS();
    for(;;); // 死循环，方便调试器捕捉
}