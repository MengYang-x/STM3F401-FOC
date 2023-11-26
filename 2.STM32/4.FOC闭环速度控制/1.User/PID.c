#include <stdint.h>

float _Kp; // Kp越大，电机的响应速度越快
float _Ki; // 调节稳态误差，随着时间的增加，让稳态误差逐渐减小到0
float _Kd; 
float Umax = 3.0f; // 输出限幅
float Ts = 0.001f; // PID调节的周期，1ms

float I; // 积分项计算结果

float last_error;        // 上次误差
uint8_t saturation_flag; // 积分饱和标志位

// PID参数设置
void PID_Init(const float Kp, const float Ki, const float Kd)
{
    _Kp = Kp;
    _Ki = Ki;
    _Kd = Kd;
}

// PID计算
float PIDCalculate(const float error)
{
    if (saturation_flag == 0)
    {
        I += _Ki * error * Ts;
    }

    float P = _Kp * error;
    float D = _Kd * (error - last_error) * Ts;
    float Uo = P + I + D;

    last_error = error; // 存储当前误差

    // 限幅处理
    if (Uo > Umax)
    {
        saturation_flag = 1;
        Uo = Umax;
    }
    else if (Uo < -Umax)
    {
        saturation_flag = 1;
        Uo = -Umax;
    }
    else
        saturation_flag = 0;

    return Uo;
}
