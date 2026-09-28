#include "stm32f10x.h"                  // Device header
#include <string.h>
#include "FreeRTOS.h"
#include "queue.h"
#define LINE_BUF_LEN 32
extern QueueHandle_t xCmdQueue;   // 声明这个队列是别的文件定义的
static char    line_buf[LINE_BUF_LEN];
static uint8_t line_len = 0;
static uint8_t rx_state = 0;          // 0=IDLE, 1=RECV
//static volatile uint8_t g_line_ready = 0;
void My_usart_Init(void)
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE );
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//将PA9引脚初始化为复用推挽输出
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//将PA10引脚初始化为上拉输入
	
	/*usart初始化*/
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate=9600;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;//工作模式
	USART_InitStructure.USART_Parity=USART_Parity_No;//校验位
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART1,&USART_InitStructure);
	
	USART_Cmd(USART1, ENABLE);
	
	/*开启接收中断*/
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	/*NVIC配置*/
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;//中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=6;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStructure);
	
	
}
void Serial_SendByte(uint8_t Byte)
{
	
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	//等待发送完成
	USART_SendData(USART1, Byte);		//将字节数据写入数据寄存器，写入后USART自动生成时序波形
}

/**
  * 函    数：串口发送一个数组
  * 参    数：Array 要发送数组的首地址
  * 参    数：Length 要发送数组的长度
  * 返 回 值：无
  */
void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)		//遍历数组
	{
		Serial_SendByte(Array[i]);		//依次调用Serial_SendByte发送每个字节数据
	}
}
void Serial_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)//遍历字符数组（字符串），遇到字符串结束标志位后停止
	{
		Serial_SendByte(String[i]);		//依次调用Serial_SendByte发送每个字节数据
	}
}

// ---------------- 中断：只做帧同步 ----------------
void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
    {
        char c = (char)USART_ReceiveData(USART1);
        if (rx_state == 0)
        {
            if (c != '\r' && c != '\n')
            {
                line_buf[line_len++] = c;
                rx_state = 1;
            }
        }
        else
        {
            if (c == '\r' || c == '\n')
            {
                line_buf[line_len] = '\0';
//                g_line_ready = 1;
				
				BaseType_t xHigherPriorityTaskWoken =pdFALSE;
				xQueueSendFromISR(xCmdQueue,line_buf,&xHigherPriorityTaskWoken);
				portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
				
                line_len = 0;
                rx_state = 0;
            }
            else
            {
                if (line_len < LINE_BUF_LEN - 1)
                {
                    line_buf[line_len++] = c;
                }
                else
                {
                    line_len = 0;
                    rx_state = 0;
                }
            }
        }

      
    }
	  if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)
        {
            (void)USART_ReceiveData(USART1);
        }
}

void Cmd_Pocess(char *line)
{

    char *p = line;
    while (*p == ' ') p++;

    // 第一段：cmd
    char *tok = p;
    while (*p != '\0' && *p != ' ') p++;
    if (*p == ' ') { *p = '\0'; p++; }
    else            return;
    if (strcmp(tok, "cmd") != 0) return;

    while (*p == ' ') p++;

    // 第二段：INJ 或 RATE
    tok = p;
    while (*p != '\0' && *p != ' ') p++;
    if (*p == ' ') { *p = '\0'; p++; }
    else            p = NULL;

    if (strcmp(tok, "INJ") == 0)
    {
        if (p != NULL && *p != '\0') return;   // cmd INJ 后面不能有参数
        ADC_SoftwareStartInjectedConvCmd(ADC1, ENABLE);
        Serial_SendString("OK\r\n");
        return;
    }

    if (strcmp(tok, "RATE") == 0)
    {
        if (p == NULL) return;
        while (*p == ' ') p++;

        uint16_t arr;
        if      (strcmp(p, "50")  == 0) arr = 10000 / 50;    // 200
        else if (strcmp(p, "100") == 0) arr = 10000 / 100;   // 100
        else    return;

        TIM_SetAutoreload(TIM3, (uint16_t)(arr - 1));
        Serial_SendString("OK\r\n");
        return;
    }
}
void Serial_SendNumber(uint16_t num)
{
    char buf[12];
    int8_t i = 0;

    if (num == 0)
    {
        Serial_SendByte('0');
        return;
    }

    while (num > 0 && i < 11)
    {
        buf[i++] = (char)('0' + num % 10);
        num /= 10;
    }

    while (i > 0)
    {
        Serial_SendByte((uint8_t)buf[--i]);
    }
}