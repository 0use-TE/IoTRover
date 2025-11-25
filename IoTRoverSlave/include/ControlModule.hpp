#pragma once
#include <Arduino.h>

class ControlModule
{
private:
    u_int8_t ENA;
    u_int8_t ENB;
    u_int8_t IN1;
    u_int8_t IN2;
    u_int8_t IN3;
    u_int8_t IN4;
    u_int8_t ENAChannel;
    u_int8_t ENBChannel;
    u_int32_t freq = 10000;

public:
    ControlModule(u_int8_t ena, u_int8_t enb, u_int8_t in1, u_int8_t in2,
                  u_int8_t in3, u_int8_t in4, u_int8_t enaCh, u_int8_t enbCh);
    void init();
    void move(int leftSpeed, int rightSpeed); // 左右速度，-255~255
};

ControlModule::ControlModule(uint8_t ena, uint8_t enb, uint8_t in1, uint8_t in2, uint8_t in3, uint8_t in4, uint8_t enaCh, uint8_t enbCh)
    : ENA(ena), ENB(enb), IN1(in1), IN2(in2), IN3(in3), IN4(in4), ENAChannel(enaCh), ENBChannel(enbCh)
{
}

void ControlModule::init()
{
    // 初始化电机引脚
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    ledcSetup(ENAChannel, freq, 8);
    ledcSetup(ENBChannel, freq, 8);
    ledcAttachPin(ENA, ENAChannel);
    ledcAttachPin(ENB, ENBChannel);
    Serial.println("引脚初始化完毕!");
}
void ControlModule::move(int leftSpeed, int rightSpeed)
{
    Serial.println("左:" + String(leftSpeed) + "右:" + String(rightSpeed));

    // 直接控制PWM
    ledcWrite(ENAChannel, constrain(abs(leftSpeed), 0, 255)); // 绝对值，0~255范围
    ledcWrite(ENBChannel, constrain(abs(rightSpeed), 0, 255));

    // 对于电机的方向，只要设置好 PWM
    if (leftSpeed >= 0)
    {
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
    }
    else
    {
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
    }

    if (rightSpeed >= 0)
    {
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
    }
    else
    {
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
    }
}
