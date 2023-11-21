/*
 * 常用数学函数实现
 */
#include "math_utils.h"

const float _2PI = 6.28318530717958f;

// normalizing radian angle to [0, 2pi]
float _normalizeAngle(float angle)
{
    float a = fmod(angle, _2PI); // fmod()函数用于浮点数的取余运算
    return a >= 0.0f ? a : (a + _2PI);
}
