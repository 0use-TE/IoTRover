#pragma once
#include <Arduino.h>
#include <WiFi.h>
class WifiModule
{
private:
    String wifiName;
    String wifiPassword;

    /* data */
public:
    WifiModule(const String &wifiName, const String &wifiPassword);
    ~WifiModule();
    bool init();
};

WifiModule::WifiModule(const String &wifiName, const String &wifiPassword)
{
    this->wifiName = wifiName;
    this->wifiPassword = wifiPassword;
}

WifiModule::~WifiModule()
{
}

bool WifiModule::init()
{
    WiFi.begin(wifiName, wifiPassword); // 启动连接
    Serial.printf("连接到Wifi: %s\n", wifiName);

    int retries = 0;
    const int maxRetries = 3;
    while (WiFi.status() != WL_CONNECTED && retries < maxRetries)
    {
        delay(500);
        Serial.print(".");
        retries++;
    }
    Serial.println();

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.print("IP: ");
        Serial.println(WiFi.localIP());

        return true;
    }
    else
    {
        Serial.printf("WiFi: %s 连接失败!\n", wifiName);
        return false;
    }
}
