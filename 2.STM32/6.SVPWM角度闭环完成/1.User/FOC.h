#ifndef __FOC_H
#define __FOC_H

#include <stdint.h>

void PWM_Init(void);
void setPhaseVoltage(float Uq, float Ud, float angle_el);
void Set_PWM(float _CCR1, float _CCR2, float _CCR3);

#endif
