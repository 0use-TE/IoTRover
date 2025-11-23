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

public:
    ControlModule(/* args */);
    ~ControlModule();
    void init();
};

void ControlModule::init()
{
    // 初始化电机引脚
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
}

ControlModule::ControlModule(/* args */)
{
}

ControlModule::~ControlModule()
{
}
