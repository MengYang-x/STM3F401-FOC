#include "FOC.h"
#include "math_utils.h"
#include "MT6701.h"
#include "PID.h"
#include "Serial.h"

/*********************************************************************/
// 开环运行测试1，模拟电角度
void OpenVelocity1(float target)
{
    const float voltageLimit = 0.5f;    // 开环电压限制
    static float _estimateAngle = 0.0f; // 开环虚拟机械角度
    const float deltaT = 0.00625f;      // 开环运行时间间隔
    _estimateAngle = _normalizeAngle((_estimateAngle + target * deltaT) * 7);
    setPhaseVoltage(voltageLimit, 0.0f, _estimateAngle);
}

// 开环速度控制测试，使用MT6701反馈的电角度
void OpenVelocity2(float Uq)
{
    float el_Angle = MT6701_GetElectricalAngle();
    setPhaseVoltage(Uq, 0.0f, el_Angle);
}

// // 开环转速测试，编码器反馈电角度，无转速控制
// void OpenVelocity2(float Uq)
// {
//     float el_Angle = 0.0f;
//     MT6701_GetElectricalAngle(&el_Angle);
//     setPhaseVoltage(Uq, 0, el_Angle);
// }

// /************闭环速度控制**********************************************/
// float Set_Velocity = 30.0f; // 目标转速
// float Actual_Velocity;      // 当前转速

// PID_Typedef velocity_PID; // 速度环PID配置

// // 闭环速度PID控制参数设置
// void Close_Velocity_Init(void)
// {
//     velocity_PID.Kp = -0.1f;
//     velocity_PID.Ki = -1.0f;
//     velocity_PID.Kd = 0.0f;
//     velocity_PID.Ts = 0.001f;
//     velocity_PID.Umax = 3.0f;
//     PID_Init(&velocity_PID, velocity_PID.Kp, velocity_PID.Ki, velocity_PID.Kd, velocity_PID.Umax, velocity_PID.Ts);
// }

// // 闭环速度控制
// void CloseVelocity(void)
// {
//     float el_Angle; // 当前电角度

//     MT6701_GetElectricalAngle(&el_Angle);
//     MT6701_GetVelocity(&Actual_Velocity);
//     LowPassFilter(&Actual_Velocity); // 对当前转速进行滤波处理

//     float Uq = PIDCalculate(&velocity_PID, Set_Velocity, Actual_Velocity);
//     setPhaseVoltage(Uq, 0.0f, el_Angle);
// }

// // 串口输出目标转速，实际转速
// void FOC_velocity_Log(void)
// {
//     FOC_log("[SetValue,ActualValue,Error]:%f,%f,%f\r\n", Set_Velocity, Actual_Velocity, Set_Velocity - Actual_Velocity);
// }

// /************闭环位置控制**********************************************/
