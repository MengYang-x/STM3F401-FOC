#ifndef __MT6701_H
#define __MT6701_H

void MT6701_log(void);   // MT6701编码器数据输出
void Align_Sensor(void); // 电角度零位校准

float MT6701_GetRawAngle(void);        // 机械角度
float MT6701_GetElectricalAngle(void); // 电角度
float MT6701_GetVelocity(void);        // 转速
#endif
