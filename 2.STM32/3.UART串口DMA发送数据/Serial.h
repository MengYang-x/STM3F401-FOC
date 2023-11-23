#ifndef __Serial_H
#define __Serial_H

// #include <stdio.h>

void log(const char *format, ...);     // 堵塞式发送数据
void log_IT(const char *format, ...);  // 中断
void log_DMA(const char *format, ...); // DMA

#endif
