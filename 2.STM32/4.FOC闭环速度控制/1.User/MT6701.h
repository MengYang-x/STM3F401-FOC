#ifndef __MT6701_H
#define __MT6701_H

void MT6701_log(void);
void MT6701_GetElectricalAngle(float *el_Angle); // 计算电角度
void MT6701_GetVelocity(float *vel);             // 计算转速

#endif
