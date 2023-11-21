#include "FOC.h"
#include "math_utils.h"

const static int polePairs = 7; // 电机的极对数
// float shaft_angle;              // 机械角度(轴角)

const static float voltageLimit = 0.5f; // 开环电压限制

// // 电角度 = 机械角度 * 极对数
// float _electricalAngle(float shaft_angle)
// {
//     return (shaft_angle * polePairs);
// }

// 开环运行测试
void OpenVelocity(float target)
{
   static float _estimateAngle = 0.0f;                                 // 开环虚拟机械角度
   static float deltaT = 0.00625f;                                     // 开环运行时间间隔
   _estimateAngle = _normalizeAngle(_estimateAngle + target * deltaT); 
   setPhaseVoltage(voltageLimit, 0.0f, _estimateAngle * polePairs);
}
