#include <stdio.h>
#include <math.h> // 用于验证sin、cos计算误差
#define ANGLE_ZOOM 100

float my_int_sin(int angle);
float my_int_cos(int angle);

typedef struct
{
    int angle;
    float sin_value;
    float cos_value;
} SIN_ITEM;

// 将角度值放大100倍进行存储
SIN_ITEM sin_table[] = {
    {9000, 1.0f, 0.0f},
    {4500, 0.707106781f, 0.707106781f},
    {3000, 0.5f, 0.866025404f},
    {500, 0.087155743f, 0.996194698f},
    {200, 0.034899497f, 0.999390827f},
    {100, 0.017452406f, 0.999847695f},
    {50, 0.008726535f, 0.999961923f},
    {20, 0.003490651f, 0.999993908f},
    {10, 0.001745328f, 0.999998477f},
    {5, 0.000872665f, 0.999999619f},
    {2, 0.000349066f, 0.999999939f},
    {1, 0.000174533f, 0.999999985f},
    {0, 0.0f, 1.0f}};

const int iterations = sizeof(sin_table) / sizeof(sin_table[0]);

// sin(A+B) = sin(A)cos(B) + cos(A)sin(B)
float my_int_sin(int angle)
{
    int i = 0;
    int A = 0, B = 0;

    for (i = 0; i < iterations; i++)
    {
        if (angle == sin_table[i].angle)
            return sin_table[i].sin_value;
        if (sin_table[i].angle <= angle && sin_table[i - 1].angle > angle)
            A = sin_table[i].angle;
    }

    if (A == 0)
        A = sin_table[0].angle;
    B = angle - A;
    return my_int_sin(A) * my_int_cos(B) + my_int_cos(A) * my_int_sin(B);
}

// cos(A+B) = cos(A)cos(B) - sin(A)*sin(B)
float my_int_cos(int angle)
{
    int i = 0;
    int A = 0, B = 0;

    for (i = 0; i < iterations; i++)
    {
        if (angle == sin_table[i].angle)
            return sin_table[i].cos_value;
        if (sin_table[i].angle <= angle && sin_table[i - 1].angle > angle)
            A = sin_table[i].angle;
    }

    if (A == 0)
        A = sin_table[0].angle;
    B = angle - A;

    return my_int_cos(A) * my_int_cos(B) - my_int_sin(A) * my_int_sin(B);
}

// 计算sin(x)
float cordic_sin(float x)
{
    int angle = x * ANGLE_ZOOM;
    int sign = 1;

    if (angle < 0)
    {
        sign = -1;
        angle = -angle;
    }

    angle = angle % (360 * ANGLE_ZOOM); // 角度限幅

    return sign * my_int_sin(angle);
}

// 计算cos(x)
float cordic_cos(float x)
{
    int angle = x * ANGLE_ZOOM;
    if (angle < 0)
        angle = -angle;
    angle = angle % (360 * ANGLE_ZOOM); // 角度限幅

    return my_int_cos(angle);
}

int main(void)
{
    float x = 0.0f;
    x = 60.0f;
    printf("angle = %f, sin = %.10f, Error = %.10f, cos = %.10f, Error = %.10f\n", x, cordic_sin(x), sin(x * 3.1415926f / 180.0f) - cordic_sin(x), cordic_cos(x), cos(x * 3.1415926f / 180.0f) - cordic_cos(x));
    x = -60.0f;
    printf("angle = %f, sin = %.10f, Error = %.10f, cos = %.10f, Error = %.10f\n", x, cordic_sin(x), sin(x * 3.1415926f / 180.0f) - cordic_sin(x), cordic_cos(x), cos(x * 3.1415926f / 180.0f) - cordic_cos(x));
    x = 120.0f;
    printf("angle = %f, sin = %.10f, Error = %.10f, cos = %.10f, Error = %.10f\n", x, cordic_sin(x), sin(x * 3.1415926f / 180.0f) - cordic_sin(x), cordic_cos(x), cos(x * 3.1415926f / 180.0f) - cordic_cos(x));
    x = -120.0f;
    printf("angle = %f, sin = %.10f, Error = %.10f, cos = %.10f, Error = %.10f\n", x, cordic_sin(x), sin(x * 3.1415926f / 180.0f) - cordic_sin(x), cordic_cos(x), cos(x * 3.1415926f / 180.0f) - cordic_cos(x));
    x = 270.0f;
    printf("angle = %f, sin = %.10f, Error = %.10f, cos = %.10f, Error = %.10f\n", x, cordic_sin(x), sin(x * 3.1415926f / 180.0f) - cordic_sin(x), cordic_cos(x), cos(x * 3.1415926f / 180.0f) - cordic_cos(x));
    x = -270.0f;
    printf("angle = %f, sin = %.10f, Error = %.10f, cos = %.10f, Error = %.10f\n", x, cordic_sin(x), sin(x * 3.1415926f / 180.0f) - cordic_sin(x), cordic_cos(x), cos(x * 3.1415926f / 180.0f) - cordic_cos(x));
    x = 400.12f;
    printf("angle = %f, sin = %.10f, Error = %.10f, cos = %.10f, Error = %.10f\n", x, cordic_sin(x), sin(x * 3.1415926f / 180.0f) - cordic_sin(x), cordic_cos(x), cos(x * 3.1415926f / 180.0f) - cordic_cos(x));
    x = -400.12f;
    printf("angle = %f, sin = %.10f, Error = %.10f, cos = %.10f, Error = %.10f\n", x, cordic_sin(x), sin(x * 3.1415926f / 180.0f) - cordic_sin(x), cordic_cos(x), cos(x * 3.1415926f / 180.0f) - cordic_cos(x));
    x = 132.34f;
    printf("angle = %f, sin = %.10f, Error = %.10f, cos = %.10f, Error = %.10f\n", x, cordic_sin(x), sin(x * 3.1415926f / 180.0f) - cordic_sin(x), cordic_cos(x), cos(x * 3.1415926f / 180.0f) - cordic_cos(x));
    x = -132.34f;
    printf("angle = %f, sin = %.10f, Error = %.10f, cos = %.10f, Error = %.10f\n", x, cordic_sin(x), sin(x * 3.1415926f / 180.0f) - cordic_sin(x), cordic_cos(x), cos(x * 3.1415926f / 180.0f) - cordic_cos(x));
    x = 555.56f;
    printf("angle = %f, sin = %.10f, Error = %.10f, cos = %.10f, Error = %.10f\n", x, cordic_sin(x), sin(x * 3.1415926f / 180.0f) - cordic_sin(x), cordic_cos(x), cos(x * 3.1415926f / 180.0f) - cordic_cos(x));
    x = -555.56f;
    printf("angle = %f, sin = %.10f, Error = %.10f, cos = %.10f, Error = %.10f\n", x, cordic_sin(x), sin(x * 3.1415926f / 180.0f) - cordic_sin(x), cordic_cos(x), cos(x * 3.1415926f / 180.0f) - cordic_cos(x));
    return 0;
}