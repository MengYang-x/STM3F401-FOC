/* MT6701引脚分配：
 * PB3:SCK
 * PB4:MISO
 * PB5:CS
 */
#include "spi.h"
#include "Serial.h"
#include <stdint.h>
#include "arm_math.h"
#include "math_utils.h"

#define MT6701_CS_Enable() HAL_GPIO_WritePin(MT6701_CS_GPIO_Port, MT6701_CS_Pin, GPIO_PIN_RESET)
#define MT6701_CS_Disable() HAL_GPIO_WritePin(MT6701_CS_GPIO_Port, MT6701_CS_Pin, GPIO_PIN_SET)

#define _2PI 6.28318530718f

const static uint16_t COUNT_PER_ROTATE = 16384; // 编码器分辨率,14bit
const uint8_t pole_pairs = 7;                   // 极对数

// 使用SPIx一次读取16bits数据
void MT6701_GetRawData(uint16_t *rawData)
{
    uint16_t txData = 0xFFFF;
    uint16_t timeOut = 200;

    while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY)
    {
        if (timeOut-- == 0)
        {
            FOC_log("SPI state error!\r\n");
            return; // 在超时时直接返回，避免继续执行后续代码
        }
    }

    MT6701_CS_Enable();

    HAL_StatusTypeDef spiStatus = HAL_SPI_TransmitReceive(&hspi1, (uint8_t *)&txData, (uint8_t *)rawData, 1, HAL_MAX_DELAY);
    if (spiStatus != HAL_OK)
    {
        MT6701_CS_Disable();
        FOC_log("MT6701 read data error!\r\n");
        return; // 在SPI传输错误时直接返回，避免继续执行后续代码
    }

    MT6701_CS_Disable();

    *rawData >>= 2; // 右移2位，取高位置14位角度数据，将结果写回rawData指向的内存
}

// 返回值：转子角度，无圈数累加
void MT6701_GetRawAngle(float *shatfAngle)
{
    uint16_t rawData = 0;
    MT6701_GetRawData(&rawData);
    *shatfAngle = (float)rawData / (float)COUNT_PER_ROTATE * _2PI;
}

// 返回值：转过的总角度，有圈数累加
void MT6701_GetSumAngle(float *sumAngle, int32_t *rotationCount)
{
    float angle = 0.0f;            // 当前轴角度
    static float angleLast = 0.0f; // 上次轴角度
    MT6701_GetRawAngle(&angle);
    float deltaAngle = angle - angleLast;
    if (fabs(deltaAngle) > (0.8f * _2PI))
        *rotationCount += (deltaAngle > 0.0f) ? -1 : 1;
    angleLast = angle;
    *sumAngle = *rotationCount * _2PI + angle;
}

// 电角度 = 机械角度 * 极对数
void MT6701_GetElectricalAngle(float *el_Angle)
{
    float shaftAngle = 0.0f;
    MT6701_GetRawAngle(&shaftAngle);
    *el_Angle = shaftAngle * pole_pairs;
}

// 计算转速(角速度)
void MT6701_GetVelocity(float *vel)
{
    float sumAngle = 0.0f; // 转过的总角度
    static float sumAngleLast = 0.0f;
    static int32_t rotationCount = 0; // 转过的圈数

    MT6701_GetSumAngle(&sumAngle, &rotationCount);

    *vel = (sumAngle - sumAngleLast) * 1000.0f; //  *vel = (sumAngle - sumAngleLast) / 0.001f;
    sumAngleLast = sumAngle;
}

void MT6701_log(void)
{
    // // static float shaftAngle = 0.0f;   // 机械角度
    // static float sumAngle = 0.0f;     // 转子转过的总角度
    // static int32_t rotationCount = 0; // 转过的圈数

    // // MT6701_GetRawAngle(&shaftAngle);
    // MT6701_GetSumAngle(&sumAngle, &rotationCount);
    // // FOC_log("rotationCount: %d, Angle:%.3f\r\n", rotationCount, shaftAngle);
    // FOC_log("rotationCount: %d, sumAngle:%.3f\r\n", rotationCount, sumAngle);

    float rawVel, velFilter;
    MT6701_GetVelocity(&rawVel);
    velFilter = rawVel; // 未滤波的转速

    LowPassFilter(&velFilter); // 对转速进行滤波

    FOC_log("[vel,vel_filter]:%f,%f\r\n", rawVel, velFilter);
}
