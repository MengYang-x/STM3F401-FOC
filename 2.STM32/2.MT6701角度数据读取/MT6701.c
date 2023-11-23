
/* MT6701引脚分配：
 * PB3:SCK
 * PB4:MISO
 * PB5:CS
 */
#include "spi.h"
#include "Serial.h"
#include <stdint.h>
#include "arm_math.h"

#define MT6701_CS_Enable() HAL_GPIO_WritePin(MT6701_CS_GPIO_Port, MT6701_CS_Pin, GPIO_PIN_RESET)
#define MT6701_CS_Disable() HAL_GPIO_WritePin(MT6701_CS_GPIO_Port, MT6701_CS_Pin, GPIO_PIN_SET)

#define _2PI 6.28318530718f

volatile float velocityLast;                    // 上次角度(用于计算转速)
volatile int32_t rotationCount;                 // 转过的圈数
volatile int32_t rotationCountLast;             // 上次转过的圈数
const static uint16_t COUNT_PER_ROTATE = 16384; // 编码器分辨率,14bit
// const static float time = 0.001f;               // 转速计算间隔时间

// 使用SPIx一次读取16bits数据
uint16_t MT6701_GetRawData(void)
{
    uint16_t rawData;
    uint16_t txData = 0xFFFF;
    while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY)
        ;
    MT6701_CS_Enable();
    HAL_SPI_TransmitReceive(&hspi1, (uint8_t *)&txData, (uint8_t *)&rawData, 1, HAL_MAX_DELAY);
    MT6701_CS_Disable();
    return rawData >> 2;
}

// 将传感器原始数据转化为弧度
float MT6701_GetRawAngle(void)
{
    uint16_t rawData = MT6701_GetRawData();
    return (float)rawData / (float)COUNT_PER_ROTATE * _2PI;
}

// 编码器数值更新
void encoder_Update(void)
{
    static float angleLast = 0.0f;
    float angle = MT6701_GetRawAngle();
    float deltaAngle = angle - angleLast;

    // If overflow happened track it as full rotation
    if (fabs(deltaAngle) > (0.8f * _2PI))
        rotationCount += (deltaAngle > 0) ? -1 : 1; // 转过的圈数统计
    angleLast = angle;
}

void MT6701_log(void)
{
    float angle = MT6701_GetRawAngle();
    printf("angle:%.3f\r\n", angle);
}
