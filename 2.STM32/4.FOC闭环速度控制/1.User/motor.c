#include "FOC.h"
#include "math_utils.h"
#include "MT6701.h"
#include "PID.h"
#include "Serial.h"

// 开环运行测试，模拟电角度
void OpenVelocity1(float target)
{
    const float voltageLimit = 0.5f;    // 开环电压限制
    static float _estimateAngle = 0.0f; // 开环虚拟机械角度
    const float deltaT = 0.00625f;      // 开环运行时间间隔
    _estimateAngle = _normalizeAngle(_estimateAngle + target * deltaT);
    setPhaseVoltage(voltageLimit, 0.0f, _estimateAngle * 7);
}

// 开环转速测试，编码器反馈电角度，无转速控制
void OpenVelocity2(float Uq)
{
    float el_Angle = 0.0f;
    MT6701_GetElectricalAngle(&el_Angle);
    setPhaseVoltage(Uq, 0, el_Angle);
}

float velocity_Target = 30.0f; // 目标转速
float velocity_Current;        // 当前转速

// 闭环速度控制
void CloseVelocity(void)
{
    float el_Angle; // 当前电角度

    PID_Init(-0.1f, -1.2f, 0.0f);

    MT6701_GetElectricalAngle(&el_Angle);
    MT6701_GetVelocity(&velocity_Current);
    LowPassFilter(&velocity_Current); // 对当前转速进行滤波处理

    float error = velocity_Target - velocity_Current; // 目标转速 - 滤波后的转速

    float Uq = PIDCalculate(error);
    setPhaseVoltage(Uq, 0.0f, el_Angle);
}

// 串口输出目标转速，实际转速
void FOC_velocity_Log(void)
{
    FOC_log("[vel_T,vel_C]:%f,%f\r\n", velocity_Target, velocity_Current);
}
