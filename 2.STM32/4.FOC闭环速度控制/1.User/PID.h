#ifndef __PID_H
#define __PID_H

void PID_Init(const float Kp, const float Ki, const float Kd); // PID参数设置
float PIDCalculate(const float error); // PID计算

#endif
