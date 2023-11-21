/*AS5600角度读取测试
 *
 */
#include <Arduino.h>
#include "AS5600.h"

int DIR = -1; // 编码器的方向需要自己测试得到，逆时针(-1)，顺时针(1)

void setup()
{
  Serial.begin(9600);

  BeginSensor(); // 配置AS5600编码器的引脚
}

void loop()
{
  // 编码器读取测试
  Serial.printf("%f\n", getAngle()); // 角度递增
  // Serial.printf("%f\n", getAngle_Without_track());
}
