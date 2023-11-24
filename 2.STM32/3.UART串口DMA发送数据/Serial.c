#include <stdio.h>
#include "usart.h"
#include <string.h>
#include <stdarg.h>

// 重定向fputc函数，使用printf()发送数据
int fputc(int ch, FILE *f)
{
	// 参数1：串口句柄，参数2：要发送的数据；参数3：要发生数据的长度；参数4：超时等待时间
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 100);
	return ch;
}

// 堵塞模式
void log(const char *format, ...)
{
	va_list args;			// 定义参数列表变量
	va_start(args, format); // 从format位置开始接收参数表，放在arg里面

	char strBuf[256];				// 定义输出的字符串
	vsprintf(strBuf, format, args); // 使用vsprintf将格式化的数据写入缓冲区
	va_end(args);					// 结束可变参数的使用
	HAL_UART_Transmit(&huart1, (uint8_t *)strBuf, strlen(strBuf), HAL_MAX_DELAY);
}

// 中断模式
void log_IT(const char *format, ...)
{
	va_list args;			// 定义参数列表变量
	va_start(args, format); // 从format位置开始接收参数表，放在arg里面

	char strBuf[256];				// 定义输出的字符串
	vsprintf(strBuf, format, args); // 使用vsprintf将格式化的数据写入缓冲区
	va_end(args);					// 结束可变参数的使用

	// 等待上次的数据发送完成，避免新的数据覆盖正在传输的数据，导致混乱
	while (HAL_UART_GetState(&huart1) == HAL_UART_STATE_BUSY_TX)
	{
		// Wait for transfer to complete
	}

	HAL_UART_Transmit_IT(&huart1, (uint8_t *)strBuf, strlen(strBuf));
}

// DMA模式
void log_DMA(const char *format, ...)
{
	va_list args;			// 定义参数列表变量
	va_start(args, format); // 从format位置开始接收参数表，放在arg里面

	char strBuf[256];				// 定义输出的字符串
	vsprintf(strBuf, format, args); // 使用vsprintf将格式化的数据写入缓冲区
	va_end(args);					// 结束可变参数的使用

	// 等待上次的数据发送完成，避免新的数据覆盖正在传输的数据，导致混乱
	while (HAL_UART_GetState(&huart1) == HAL_UART_STATE_BUSY_TX)
	{
		// Wait for DMA transfer to complete
	}

	HAL_UART_Transmit_DMA(&huart1, (uint8_t *)strBuf, strlen(strBuf));
}
