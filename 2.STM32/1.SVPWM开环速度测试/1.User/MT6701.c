//#include "arm_math.h"
//#include "spi.h"

//#define MT6701_Enable() HAL_GPIO_WritePin(MT6701_CS_GPIO_Port, MT6701_CS_Pin, GPIO_PIN_RESET)
//#define MT6701_Disable() HAL_GPIO_WritePin(MT6701_CS_GPIO_Port, MT6701_CS_Pin, GPIO_PIN_SET)

//#define _2PI 6.28318530718f

//volatile float angleLast;                       // 上次角度
//volatile float velocityLast;                    // 上次角度(用于计算转速)
//volatile int32_t rotationCount;                 // 转过的圈数
//volatile int32_t rotationCountLast;             // 上次转过的圈数
//const static uint16_t COUNT_PER_ROTATE = 16384; // 编码器分辨率,14bit
//const static float time = 0.001f;               // 转速计算间隔时间

//// 读取传感器原始数据
//uint16_t MT6701_GetRawData(void)
//{
//  uint16_t txData = 0xFFFF;
//  uint16_t rxData;
//  while (__HAL_SPI_GET_FLAG(&hspi1, SPI_FLAG_TXE) == RESET)
//    ;
//  MT6701_Enable();
//  HAL_SPI_Transmit(&hspi1, &txData, 1, 100);
//  while (__HAL_SPI_GET_FLAG(&hspi1, SPI_FLAG_RXNE) == RESET)
//    ;
//  HAL_SPI_Receive(&hspi1, &rxData, 1, 100);
//  MT6701_Disable();
//  return rxData >> 2;
//}

//// 返回原始角度数据
//float MT6701_GetRawAngle(void)
//{
//  return ((float)MT6701_GetRawData() / (float)COUNT_PER_ROTATE) * _2PI;
//}

//// 更新编码器的值
//void encoder_Update(void)
//{
//  float angle = MT6701_GetRawAngle();
//  float deltaAngle = angle - angleLast;

//  // If overflow happened track it as full rotation
//  if (fabs(deltaAngle) > (0.8f * _2PI))
//    rotationCountLast += (deltaAngle > 0) ? -1 : 1;
//  angleLast = angle;
//}

//void encoder_VarInit(void)
//{
//  velocityLast = MT6701_GetRawAngle();
//  HAL_Delay(1);
//  angleLast = MT6701_GetRawAngle();
//}

//// 返回单圈角度值
//float encoder_GetLapAngle(void)
//{
//  return angleLast;
//}

//float encoder_GetFullAngle(void)
//{
//  return (float)rotationCount * _2PI + angleLast;
//}

//int32_t encoder_GetRotationCount(void)
//{
//  return rotationCount;
//}

//float encoder_GetVelocity(void)
//{
//  // velocity calculation
//  float vel = ((float)(rotationCount - rotationCountLast) * _2PI + (angleLast - velocityLast)) / time;

//  // save variables for future pass
//  velocityLast = angleLast;
//  rotationCountLast = rotationCount;

//  return vel;
//}
