// Bootstrap.hpp
#pragma once
#include <Arduino.h>
#include "Settings.h"
#include "FSModule.hpp"
#include "WifiModule.hpp"
#include "APModule.hpp"
#include "WebServerModule.hpp"
#include "WebSocketModule.hpp"
#include "ControlModule.hpp"

class Bootstrap
{
private:
    
    FSModule fsModule;
    WifiModule wifiModule;
    APModule apModule;
    ControlModule controlModule;
    WebServerModule webServerModule;
    WebSocketModule webSocketModule;

    bool needAP = false;
    unsigned long lastMemPrint = 0;
    void printMemory()
    {
        Serial.println("╔═══════════════════════════════════");
        Serial.printf("║  可用堆内存 (Free Heap)    : %6d 字节\n", ESP.getFreeHeap());
        Serial.printf("║  最大单块堆 (Max Alloc)    : %6d 字节\n", ESP.getMaxAllocHeap());
        Serial.printf("║  最小曾有堆 (Min Free)     : %6d 字节\n", ESP.getMinFreeHeap());
        Serial.printf("║  总堆大小 (Total Heap)     : %6d 字节\n", ESP.getHeapSize());

        Serial.printf("║  当前栈水位 (Stack HWM)    : %6d 字节\n", uxTaskGetStackHighWaterMark(NULL));
        Serial.printf("║  总栈大小 (约)             : %6d 字节\n", CONFIG_ARDUINO_LOOP_STACK_SIZE);
        Serial.println("╚═══════════════════════════════════");
    }

public:
    Bootstrap()
        : wifiModule(WifiName, WifiPassword), apModule(APName, APPassword),
          controlModule(ENA, ENB, IN1, IN2, IN3, IN4, ENAChannel, ENBChannel),
          webServerModule(WebServerPort), webSocketModule(WebSocketPort, &controlModule)
    {
    }

    void setup();
    void loop();
};

// ====================== setup ======================
void Bootstrap::setup()
{
    Serial.begin(921600);
    while (!Serial)
        delay(10);
    Serial.println(F("\n\n========== ESP32 小车启动 =========="));

    // 1. 文件系统
    fsModule.init();
    // 2. 尝试连接 WiFi
    bool wifiOk = wifiModule.init();

    if (!wifiOk)
    {
        Serial.println(F("WiFi 连接失败 → 启动配网 AP"));
        needAP = true;
        apModule.init();
    }

    // 3. 电机、Web、WebSocket 无论如何都要启动
    controlModule.init();
    webServerModule.init();
    webSocketModule.init();

    // 4. 启动完成提示
    IPAddress ip = needAP ? WiFi.softAPIP() : WiFi.localIP();
    Serial.println(F("====================================="));
    Serial.printf("  系统启动完成！\n");
    if (needAP)
    {
        Serial.println(F("  当前为配网模式 (AP)"));
    }
    else
    {
        Serial.println(F("  当前为正常联网模式 (STA)"));
    }
    Serial.printf("  浏览器访问 → http://%s\n", ip.toString().c_str());
    Serial.println(F("=====================================\n"));
}

// ====================== loop ======================
void Bootstrap::loop()
{
    webSocketModule.loop();
    if (millis() - lastMemPrint > 10000)
    {
        lastMemPrint = millis();
        printMemory();
    }
}