#pragma once

void BeginSensor();
float getAngle();               // 得到编码器的值，带圈数，值一直增加,弧度制
float getAngle_Without_track(); // 不带圈数，读取AS5600检测到的单圈值
