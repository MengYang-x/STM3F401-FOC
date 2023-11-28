#ifndef __PID_H
#define __PID_H

typedef struct
{
    float Kp;
    float Ki;
    float Kd;
    float Umax;              // 输出限幅
    float I;                 // 积分项计算结果
    float last_Error;        // 上次误差
    uint8_t saturation_flag; // 积分饱和标志位
    float Ts;                // PID调节周期
} PID_Typedef;

void PID_Init(PID_Typedef *pidHandle, float Kp, float Ki, float Kd, float Umax, float Ts); // PID参数设置
float PIDCalculate(PID_Typedef *pidHandle, float SetValue, float ActualValue);             // PID计算

#endif
