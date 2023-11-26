#ifndef __MOTOR_H
#define __MOTOR_H

// 函数声明
void OpenVelocity1(float target); // 开环运行测试，模拟电角度
void OpenVelocity2(float Uq);     // 开环转速测试，编码器反馈电角度，无转速控制
void CloseVelocity(void);         // 闭环速度控制

void FOC_velocity_Log(void); // 串口输出目标转速，实际转速

#endif
