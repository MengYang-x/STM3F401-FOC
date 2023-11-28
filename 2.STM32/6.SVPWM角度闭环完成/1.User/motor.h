#ifndef __MOTOR_H
#define __MOTOR_H

// 函数声明
void OpenVelocity1(float target); // 开环运行测试，模拟电角度
void OpenVelocity2(float Uq);     // 开环转速测试，编码器反馈电角度，无转速控制

void velocity_PID_Config(void);    // 速度坏PID参数配置
void close_Velocity_Control(void); // 速度环PID控制
void close_Velocity_LOG(void);     // 串口输出目标转速，实际转速

void angle_PID_Config(void);    // 位置环PID参数配置
void close_Angle_Control(void); // 位置环PID控制
void close_Angle_LOG(void);     // 位置环串口输出目标角度、实际角度

#endif
