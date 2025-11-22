#pragma once
#include <Arduino.h>
#include <LittleFS.h>
class FSModule
{
private:
    /* data */
public:
    void init();
};

void FSModule::init()
{
    // 1. 先尝试正常挂载（不格式化）
    if (LittleFS.begin(false))
    { 
        Serial.println("LittleFS 文件系统初始化成功!");
        return;
    }

    // 2. 正常挂载失败 → 开始最多尝试格式化 3 次
    Serial.println("初次挂载失败，正在尝试格式化...");
    const int maxRetries = 3;
    for (int i = 1; i <= maxRetries; i++)
    {
        Serial.printf("第 %d 次格式化尝试...\n", i);

        if (LittleFS.begin(true))
        { // true = 强制格式化
            Serial.println("格式化成功，LittleFS 已挂载！");
            return; // 成功 → 直接返回
        }

        Serial.printf("第 %d 次格式化失败\n", i);
        delay(500); // 稍等一下再重试，防止 flash 过热
    }

    // 3. 3 次都失败 → 致命错误，直接死机或重启
    Serial.println("FATAL: LittleFS 多次格式化均失败，系统无法继续运行！");
    Serial.println("正在关机/死机...");

    // 永久深度睡眠
    esp_deep_sleep_start();       
}