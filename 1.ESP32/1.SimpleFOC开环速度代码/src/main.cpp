/*
 * 日期：2023.7.22
 * 开环速度控制代码
 * 使用vofa+ 进行串口调试，波特率需要设置为9600
 * 电机A2212/15T的极对数：7
 */
#include <Arduino.h>

const int poles = 7;  // 电机的极对数

// PWM输出引脚定义
const char pwmA = 5;
const char pwmB = 18;
const char pwmC = 19;

const float voltagePowerSupply = 12.4;
float open_loop_timestamp = 0;
float shaft_angle = 0; // 机械角度
float zero_electric_angle = 0;
float Ualpha, Ubeta = 0;
float Ua = 0, Ub = 0, Uc = 0;
float dc_a = 0, dc_b = 0, dc_c = 0;

void setup()
{
    Serial.begin(9600);

    // PWM设置
    pinMode(pwmA, OUTPUT);
    pinMode(pwmB, OUTPUT);
    pinMode(pwmC, OUTPUT);
    ledcAttachPin(pwmA, 0);
    ledcAttachPin(pwmB, 1);
    ledcAttachPin(pwmC, 2);
    ledcSetup(0, 30000, 8); // pwm频道, 频率, 精度
    ledcSetup(1, 30000, 8); // pwm频道, 频率, 精度
    ledcSetup(2, 30000, 8); // pwm频道, 频率, 精度
    Serial.println("完成PWM初始化设置");
    delay(3000);
}

// 电角度 = 机械角度 * 极对数
float _electricalAngle(float shaft_angle, int pole_pairs)
{
    return (shaft_angle * pole_pairs);
}

// 角度归一化到[0, 2pi]，把输入的角度限制在[0, 2pi]
float _normalizeAngle(float angle)
{
    float a = fmod(angle, 2 * PI); // 取余，结果可能为负值
    return a >= 0 ? a : (a + 2 * PI);
}

// 设置PWM输出
void setPwm(float Ua, float Ub, float Uc)
{

    // 计算占空比，限制占空比从0到1
    dc_a = constrain(Ua / voltagePowerSupply, 0.0f, 1.0f);
    dc_b = constrain(Ub / voltagePowerSupply, 0.0f, 1.0f);
    dc_c = constrain(Uc / voltagePowerSupply, 0.0f, 1.0f);

    // 写入PWM到PWM 0 1 2 通道
    ledcWrite(0, dc_a * 255);
    ledcWrite(1, dc_b * 255);
    ledcWrite(2, dc_c * 255);
}

// 核心公式
void setPhaseVoltage(float Uq, float Ud, float angle_el)
{
    angle_el = _normalizeAngle(angle_el + zero_electric_angle); // 电角度

    // 帕克逆变换
    Ualpha = -Uq * sin(angle_el);
    Ubeta = Uq * cos(angle_el);

    // 克拉克逆变换
    Ua = Ualpha + voltagePowerSupply / 2;
    Ub = (sqrt(3) * Ubeta - Ualpha) / 2 + voltagePowerSupply / 2;
    Uc = (-Ualpha - sqrt(3) * Ubeta) / 2 + voltagePowerSupply / 2;
    setPwm(Ua, Ub, Uc);
}

// 开环速度函数，Uq和电角度生成器
float velocityOpenloop(float target_velocity)
{
    unsigned long now_us = micros(); // 获取从开启芯片以来的微秒数，它的精度是 4 微秒。 micros() 返回的是一个无符号长整型（unsigned long）的值

    // 计算当前每个Loop的运行时间间隔
    float Ts = (now_us - open_loop_timestamp) * 1e-6f;

    // 由于 micros() 函数返回的时间戳会在大约 70 分钟之后重新开始计数，在由70分钟跳变到0时，TS会出现异常，因此需要进行修正。如果时间间隔小于等于零或大于 0.5 秒，则将其设置为一个较小的默认值，即 1e-3f
    if (Ts <= 0 || Ts > 0.5f)
        Ts = 1e-3f;

    // 通过乘以时间间隔和目标速度来计算需要转动的机械角度，存储在 shaft_angle 变量中。在此之前，还需要对轴角度进行归一化，以确保其值在 0 到 2π 之间。
    shaft_angle = _normalizeAngle(shaft_angle + target_velocity * Ts);
    // 以目标速度为 10 rad/s 为例，如果时间间隔是 1 秒，则在每个循环中需要增加 10 * 1 = 10 弧度的角度变化量，才能使电机转动到目标速度。
    // 如果时间间隔是 0.1 秒，那么在每个循环中需要增加的角度变化量就是 10 * 0.1 = 1 弧度，才能实现相同的目标速度。因此，电机轴的转动角度取决于目标速度和时间间隔的乘积。

    // 使用早前设置的voltage_power_supply的1/3作为Uq值，这个值会直接影响输出力矩
    // 最大只能设置为Uq = voltage_power_supply/2，否则ua,ub,uc会超出供电电压限幅
    float Uq = voltagePowerSupply / 10;

    setPhaseVoltage(1.0f, 0, _electricalAngle(shaft_angle, poles)); // 极对数可以设置为常量

    open_loop_timestamp = now_us; // 用于计算下一个时间间隔

    return Uq;
}

void debug()
{
    Serial.printf("%f,%f,%f\n", dc_a, dc_b, dc_c);
}

void loop()
{
    velocityOpenloop(5);
    // debug();
}
