#include <stdint.h>

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

// PID参数设置
void PID_Init(PID_Typedef *pidHandle, float Kp, float Ki, float Kd, float Umax, float Ts)
{
    // PID调节周期1ms，Ts = 0.001f
    pidHandle->Kp = Kp;
    pidHandle->Ki = Ki;
    pidHandle->Kd = Kd;
    pidHandle->Umax = Umax;
    pidHandle->Ts = Ts;
}

// PID控制计算
float PIDCalculate(PID_Typedef *pidHandle, float SetValue, float ActualValue)
{
    float Error = SetValue - ActualValue;
    // 限制积分项的增长
    // 因为当输出永远跟不上输入时，积分项会无限增长，可能会超出浮点数的范围
    if (pidHandle->saturation_flag == 0)
    {

        pidHandle->I += pidHandle->Ki * Error * pidHandle->Ts; // I += Ki * Error * Ts
    }

    float P = pidHandle->Kp * Error;                                           // P = Kp * Error;
    float D = pidHandle->Kd * (Error - pidHandle->last_Error) / pidHandle->Ts; // D = Kd * (Error - last_Error) / Ts

    float Uo = P + pidHandle->I + D; // 输出Uout = P + I + D

    // 存储当前误差
    pidHandle->last_Error = Error;

    // 限幅处理
    if (Uo > pidHandle->Umax)
    {
        pidHandle->saturation_flag = 1;
        Uo = pidHandle->Umax;
    }
    else if (Uo < -pidHandle->Umax)
    {
        pidHandle->saturation_flag = 1;
        Uo = -pidHandle->Umax;
    }
    else
        pidHandle->saturation_flag = 0;
    return Uo;
}
