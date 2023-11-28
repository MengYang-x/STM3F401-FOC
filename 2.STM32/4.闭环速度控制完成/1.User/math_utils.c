/*
 * 常用数学函数实现
 */
#include "math_utils.h"

// normalizing radian angle to [0, 2pi]
float _normalizeAngle(float angle)
{
    float a = fmod(angle, _2PI); // fmod()函数用于浮点数的取余运算
    return a >= 0.0f ? a : (a + _2PI);
}

// 低通滤波
// float LowPassFilter(float value)
// {
//     static float valueLast = 0.0f;
//     value = 0.9f * valueLast + 0.1f * value;
//     valueLast = value;
//     return value;
// }

// 低通滤波
void LowPassFilter(float *value)
{
    static float valueLast = 0.0f;
    *value = 0.9f * valueLast + 0.1f * *value;
    valueLast = *value;
}
