#pragma once
#include <WebSocketsServer.h>

class WebSocketModule {
private:
    WebSocketsServer *webSocketServer;  // WebSocket 服务器
    uint16_t webSocketPort;  // WebSocket 端口

public:
    WebSocketModule(uint16_t webSocketPort);
    ~WebSocketModule();
    void init();
    void sendText(uint8_t num, const char* message);   // 发送文本消息
    void sendBin(uint8_t num, uint8_t* message, size_t length);  // 发送二进制消息
    void sendPing(uint8_t num);  // 发送 Ping 消息
    void onEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length);  // 事件处理
    void loop();
};

WebSocketModule::WebSocketModule(uint16_t webSocketPort) {
    this->webSocketPort = webSocketPort;
}

WebSocketModule::~WebSocketModule() {
    if (webSocketServer != nullptr) {
        delete webSocketServer;
    }
}

void WebSocketModule::init() {
    webSocketServer = new WebSocketsServer(webSocketPort);
    Serial.println("WebSocketServer初始化成功");
    webSocketServer->begin();
    webSocketServer->onEvent([this](uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
        this->onEvent(num, type, payload, length);
    Serial.println("WebSocketServer启动成功");

    });
}

void WebSocketModule::sendText(uint8_t num, const char* message) {
    webSocketServer->sendTXT(num, message);  // 发送文本消息
}

void WebSocketModule::sendBin(uint8_t num, uint8_t* message, size_t length) {
    webSocketServer->sendBIN(num, message, length);  // 发送二进制消息
}

void WebSocketModule::sendPing(uint8_t num) {
    webSocketServer->sendPing(num);  // 发送 Ping 消息
}
void WebSocketModule::loop(){
    webSocketServer->loop();
}

// WebSocket 事件处理函数
void WebSocketModule::onEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
    switch (type) {
        case WStype_CONNECTED: {
            Serial.println("WebSocket客户端连接成功");
            break;
        }
        case WStype_DISCONNECTED: {
            Serial.println("WebSocket客户端断开连接");
            break;
        }
        case WStype_TEXT: {
            Serial.print("收到文本消息：");
            Serial.println((char*)payload);
            // 你可以在这里根据需求处理收到的消息
            break;
        }
        case WStype_BIN: {
            Serial.println("收到二进制消息");
            // 处理二进制消息
            break;
        }
        case WStype_PING: {
            Serial.println("收到 Ping 消息");
            break;
        }
        case WStype_PONG: {
            Serial.println("收到 Pong 消息");
            break;
        }
        default:
            break;
    }
}
