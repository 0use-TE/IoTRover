// WebSocketModule.hpp
#pragma once
#include <Arduino.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include "ControlModule.hpp"

// ====================== 类声明 ======================
class WebSocketModule
{
private:
    WebSocketsServer webSocketServer; // 直接对象！不要 new！
    ControlModule *controlModule;     // 控制模块指针

    // 超大 JSON 缓冲区，做成成员
    JsonDocument doc;

public:
    WebSocketModule(uint16_t port, ControlModule *ctrl);

    void init();
    void loop();

    void sendText(uint8_t num, const char *message);
    void sendText(uint8_t num, const String &message);
    void sendPing(uint8_t num);

private:
    void handleEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length);
};

// ====================== 构造函数 ======================
WebSocketModule::WebSocketModule(uint16_t port, ControlModule *ctrl)
    : webSocketServer(port), controlModule(ctrl)
{
}

// ====================== 初始化 ======================
void WebSocketModule::init()
{
    webSocketServer.begin();
    webSocketServer.onEvent([this](uint8_t num, WStype_t type, uint8_t *payload, size_t length)
                            { this->handleEvent(num, type, payload, length); });
    Serial.println("[WebSocket] 服务器已启动");
}

// ====================== 主循环 ======================
void WebSocketModule::loop()
{
    webSocketServer.loop();
}

// ====================== 事件处理（核心）======================
void WebSocketModule::handleEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
    case WStype_CONNECTED:
        Serial.printf("[WebSocket] 客户端 %u 已连接\n", num);
        break;

    case WStype_DISCONNECTED:
        Serial.printf("[WebSocket] 客户端 %u 已断开\n", num);
        controlModule->move(0, 0);
        break;

    case WStype_TEXT:
    {
        // 防止超长消息直接炸
        if (length == 0 || length >= 1000)
        {
            Serial.println("[WebSocket] 收到空消息或超长消息，已忽略");
            return;
        }
        // 复用成员 doc，零栈压
        doc.clear();
        DeserializationError error = deserializeJson(doc, payload,length);
        // auto content = (char *)payload;
        // Serial.println(content);

        if (error)
        {
            Serial.print("[WebSocket] JSON 解析失败: ");
            Serial.println(error.c_str());
            return;
        }

        const char *cmd = doc["cmd"];
        if (!cmd)
        {
            Serial.println("[WebSocket] 缺少 cmd 字段");
            return;
        }

        if (strcmp(cmd, "move") == 0)
        {
            int left = doc["leftMotor"] | 0;
            int right = doc["rightMotor"] | 0;

            // 可加范围限制
            left = constrain(left, -255, 255);
            right = constrain(right, -255, 255);

            controlModule->move(left, right);
        }
        else if (strcmp(cmd, "stop") == 0)
        {
            controlModule->move(0, 0);
        }
        break;
    }

    case WStype_BIN:
        Serial.printf("[WebSocket] 收到二进制消息，长度: %u\n", length);
        // 如需处理二进制协议，再打开
        break;

    case WStype_PING:
        // Serial.println("[WebSocket] Ping");
        break;

    case WStype_PONG:
        // Serial.println("[WebSocket] Pong");
        break;

    default:
        break;
    }
}

void WebSocketModule::sendText(uint8_t num, const char *message)
{
    webSocketServer.sendTXT(num, message);
}

void WebSocketModule::sendText(uint8_t num, const String &message)
{
}

void WebSocketModule::sendPing(uint8_t num)
{
    webSocketServer.sendPing(num);
}