#ifndef __MATH_UTILS_H
#define __MATH_UTILS_H

#include "math.h"

#define _constraint(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))

#define _2PI 6.28318530718f
#define _3PI_2 4.71238898038f

float _normalizeAngle(float angle); // normalizing radian angle to [0, 2pi]
// float LowPassFilter(float value);   // 低通滤波器
void LowPassFilter(float *value); // 低通滤波器
#endif
