#pragma once
#include <WebSocketsServer.h>
#include "ControlModule.hpp"
#include <ArduinoJson.h>
class WebSocketModule
{
private:
    WebSocketsServer *webSocketServer; // WebSocket 服务器
    uint16_t webSocketPort;            // WebSocket 端口
    ControlModule *controlModule;

public:
    WebSocketModule(uint16_t webSocketPort, ControlModule *controlModule);
    ~WebSocketModule();
    void init();
    void sendText(uint8_t num, const char *message);                           // 发送文本消息
    void sendBin(uint8_t num, uint8_t *message, size_t length);                // 发送二进制消息
    void sendPing(uint8_t num);                                                // 发送 Ping 消息
    void onEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length); // 事件处理
    void loop();
};

WebSocketModule::WebSocketModule(uint16_t webSocketPort, ControlModule *controlModule)
{
    this->webSocketPort = webSocketPort;
    this->controlModule = controlModule;
}

WebSocketModule::~WebSocketModule()
{
    if (webSocketServer != nullptr)
    {
        delete webSocketServer;
    }
}

void WebSocketModule::init()
{
    webSocketServer = new WebSocketsServer(webSocketPort);
    Serial.println("WebSocketServer初始化成功");
    webSocketServer->begin();
    // 获取消息
    webSocketServer->onEvent([this](uint8_t num, WStype_t type, uint8_t *payload, size_t length)
                             { this->onEvent(num, type, payload, length); });
}

void WebSocketModule::sendText(uint8_t num, const char *message)
{
    webSocketServer->sendTXT(num, message); // 发送文本消息
}

void WebSocketModule::sendBin(uint8_t num, uint8_t *message, size_t length)
{
    webSocketServer->sendBIN(num, message, length); // 发送二进制消息
}

void WebSocketModule::sendPing(uint8_t num)
{
    webSocketServer->sendPing(num); // 发送 Ping 消息
}
void WebSocketModule::loop()
{
    webSocketServer->loop();
}

// WebSocket 事件处理函数
void WebSocketModule::onEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
    case WStype_CONNECTED:
    {
        Serial.println("WebSocket客户端连接成功");
        break;
    }
    case WStype_DISCONNECTED:
    {
        Serial.println("WebSocket客户端断开连接");
        break;
    }
    case WStype_TEXT:
    {
        String msg = String((char *)payload); // 将转为 String
        Serial.println(msg);                  // 打印转换后的字符串
        JsonDocument doc;
        DeserializationError err = deserializeJson(doc, msg);
        if (err)
        {
            Serial.println("JSON 解析失败");
            Serial.println(err.c_str());
            return;
        }
        // 获取命令字段
        String cmd = doc["cmd"]; // 字段不存在= nullptr
        if (!cmd)
        {
            Serial.println("缺少 cmd 字段");
            return;
        }

        if (cmd == "move")
        {
            int leftMotor = doc["leftMotor"];
            int rightMotor = doc["rightMotor"];
            controlModule->move(leftMotor, rightMotor);
        }

        break;
    }
    case WStype_BIN:
    {
        Serial.println("收到二进制消息");
        // 处理二进制消息
        break;
    }
    case WStype_PING:
    {
        Serial.println("收到 Ping 消息");
        break;
    }
    case WStype_PONG:
    {
        Serial.println("收到 Pong 消息");
        break;
    }
    default:
        break;
    }
}
