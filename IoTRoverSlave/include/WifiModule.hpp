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
    WiFi.mode(WIFI_STA);
    WiFi.begin(wifiName, wifiPassword);
    Serial.printf("连接到 Wifi: %s\n", wifiName);
    unsigned long startAttemptTime = millis();
    const unsigned long timeout = 10000;

    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < timeout)
    {
        Serial.print(".");
        delay(500);
    }
    Serial.println();

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.print("IP: ");
        Serial.println(WiFi.localIP());
        return true;
    }

    Serial.printf("WiFi: %s 连接失败!\n", wifiName);
    return false;
}
