#include <stdint.h>

class PID
{
private:
    float _Kp;
    float _Ki;
    float _Kd;
    float Umax; // 输出限幅
    float Ts;   // PID调节的周期，1ms

    float I; // 积分项计算结果

    float last_error;        // 上次误差
    uint8_t saturation_flag; // 积分饱和标志位

    PID(float Kp, float Ki, float Kd)
    {
        this->_Kp = Kp;
        this->_Ki = Ki;
        this->_Kd = Kd;

        this->Umax = 3.0f;
        this->Ts = 0.001f;
        this->saturation_flag = 0;
    }

public:
    float PIDCalculate(float error);
};

float PID::PIDCalculate(float error)
{
    // 限制积分项的增长
    // 因为当输出永远跟不上输入时，积分项会无限增长，可能会超出浮点数的范围
    if (saturation_flag == 0)
    {
        I += _Ki * error * Ts;
    }

    float P = _Kp * error;
    float D = _Kd * (error - last_error) * Ts;
    float Uout = P + I + D;

    last_error = error; // 存储当前误差

    // 限幅处理
    if (Uout > Umax)
    {
        saturation_flag = 1;
        Uout = Umax;
    }
    else if (Uout < -Umax)
    {
        saturation_flag = 1;
        Uout = -Umax;
    }
    else
        saturation_flag = 0;

    return Uout;
}
