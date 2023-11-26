#ifndef __MATH_UTILS_H
#define __MATH_UTILS_H

#include "math.h"

float _normalizeAngle(float angle); // normalizing radian angle to [0, 2pi]
// float LowPassFilter(float value);   // 低通滤波器
void LowPassFilter(float *value); // 低通滤波器
#endif
