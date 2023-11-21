#include <stdio.h>
#include "usart.h"

// 重定向fputc函数
int fputc(int ch, FILE *f){
	// 参数1：串口句柄，参数2：要发送的数据；参数3：要发生数据的长度；参数4：超时等待时间
	HAL_UART_Transmit(&huart1,(uint8_t *)&ch, 1, 100);
	return ch;
}
