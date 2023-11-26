/*定时器TIM2输出通道引脚分配：
 * TIM2_CH1:PA0
 * TIM2_CH2:PA1
 * TIM2_CH3:PA2
 */
#include "Serial.h"
#include "tim.h"
#include "arm_math.h"
#include "math_utils.h"

const float sqrt3 = 1.73205080756f;

const int PWM_ARR = 2800; // PWM的计数周期
const float Udc = 12.4f;  // 电机的母线电压

// 使能TIMx的通道y
void PWM_Init(void)
{
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
}

// 输入参数0.0f ~ 1.0f，输出3路PWM
void Set_PWM(float _CCR1, float _CCR2, float _CCR3)
{
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, _CCR1 * PWM_ARR); // PA0
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, _CCR2 * PWM_ARR); // PA1
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, _CCR3 * PWM_ARR); // PA2
}

// FOC核心函数：输入Uq、Ud和电角度，输出三路PWM
void setPhaseVoltage(float Uq, float Ud, float angle_el)
{
	static float Ts = 1.0f;
	float Ta, Tb, Tc;
	float t1, t2, t3, t4, t5, t6, t7;
	float sum, k_svpwm;

	angle_el = _normalizeAngle(angle_el);

	// Park逆变换
	float U_alpha = -Uq * arm_sin_f32(angle_el) + Ud * arm_cos_f32(angle_el);
	float U_beta = Uq * arm_cos_f32(angle_el) + Ud * arm_sin_f32(angle_el);

	// 扇区判断
	float K = sqrt3 * Ts / Udc; // SVPWM调制比
	float u1 = U_beta * K;
	float u2 = (0.8660254f * U_alpha - 0.5f * U_beta) * K; // sqrt(3)/2 = 0.8660254
	float u3 = (-0.8660254f * U_alpha - 0.5f * U_beta) * K;

	uint8_t sector = (u1 > 0.0f) + ((u2 > 0.0f) << 1) + ((u3 > 0.0f) << 2); // sector = A + 2B + 4C

	// 非零矢量和零矢量作用时间的计算
	switch (sector)
	{
	case 3: // 扇区1
		t4 = u2;
		t6 = u1;
		sum = t4 + t6;
		if (sum > Ts) // 过调制处理
		{
			k_svpwm = Ts / sum;
			t4 *= k_svpwm;
			t6 *= k_svpwm;
		}
		t7 = (Ts - t4 - t6) / 2.0f;
		Ta = t4 + t6 + t7;
		Tb = t6 + t7;
		Tc = t7;
		break;
	case 1: // 扇区2
		t2 = -u2;
		t6 = -u3;
		sum = t2 + t6;
		if (sum > Ts)
		{
			k_svpwm = Ts / sum;
			t2 *= k_svpwm;
			t6 *= k_svpwm;
		}
		t7 = (Ts - t2 - t6) / 2.0f;
		Ta = t6 + t7;
		Tb = t2 + t6 + t7;
		Tc = t7;
		break;
	case 5: // 扇区3
		t2 = u1;
		t3 = u3;
		sum = t2 + t3;
		if (sum > Ts)
		{
			k_svpwm = Ts / sum;
			t2 *= k_svpwm;
			t3 *= k_svpwm;
		}
		t7 = (Ts - t2 - t3) / 2.0f;
		Ta = t7;
		Tb = t2 + t3 + t7;
		Tc = t3 + t7;
		break;
	case 4: // 扇区4
		t1 = -u1;
		t3 = -u2;
		sum = t1 + t3;
		if (sum > Ts)
		{
			k_svpwm = Ts / sum;
			t1 *= k_svpwm;
			t3 *= k_svpwm;
		}
		t7 = (Ts - t1 - t3) / 2.0f;
		Ta = t7;
		Tb = t3 + t7;
		Tc = t1 + t3 + t7;
		break;
	case 6: // 扇区5
		t1 = u3;
		t5 = u2;
		sum = t1 + t5;
		if (sum > Ts)
		{
			k_svpwm = Ts / sum;
			t1 *= k_svpwm;
			t5 *= k_svpwm;
		}
		t7 = (Ts - t1 - t5) / 2.0f;
		Ta = t5 + t7;
		Tb = t7;
		Tc = t1 + t5 + t7;
		break;
	case 2: // 扇区6
		t4 = -u3;
		t5 = -u1;
		sum = t4 + t5;
		if (sum > Ts)
		{
			k_svpwm = Ts / sum;
			t4 *= k_svpwm;
			t5 *= k_svpwm;
		}
		t7 = (Ts - t4 - t5) / 2.0f;
		Ta = t4 + t5 + t7;
		Tb = t7;
		Tc = t5 + t7;
		break;
	default:
		break;
	}

	// FOC_log("[Ta,Tb,Tc]:%f,%f,%f\r\n", Ta, Tb, Tc);
	Set_PWM(Ta, Tb, Tc); // 输出三路PWM，驱动无刷电机转动
}
