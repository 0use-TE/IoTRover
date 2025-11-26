#pragma once
#include <WiFi.h>
class APModule
{
private:
    String apName, apPassword;

public:
    APModule(String apName, String apPassword);
    ~APModule();
    void init();
};

APModule::APModule(String apName, String apPassword)
{
    this->apName = apName;
    this->apPassword = apPassword;
}

APModule::~APModule()
{
}

void APModule::init()
{
    if (WiFi.softAP(apName, apPassword,11)) // 启动热点
    {
        // 打印热点的 IP 地址
        Serial.println("ESP32 热点已启动");
        WiFi.setTxPower(WIFI_POWER_19_5dBm);
        Serial.print("热点 IP 地址: ");
        // 打印热点的 IP 地址
        Serial.println(WiFi.softAPIP()); 
    }
    else{
        Serial.println("打开AP失败,进入休眠状态");
        esp_deep_sleep_start();
    }
}