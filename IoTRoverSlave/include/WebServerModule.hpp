#pragma once
#include <Arduino.h>
#include <ESPAsyncWebServer.h>
class WebServerModule
{
private:
    uint16_t port;
    AsyncWebServer server; // 栈对象

public:
    WebServerModule(uint16_t serverPort);
    void init();
};

WebServerModule::WebServerModule(uint16_t serverPort)
    : port(serverPort), server(serverPort) // 这里传端口
{
    Serial.println("WebServerModule 已初始化");
}

void WebServerModule::init()
{
    server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");
    server.onNotFound([](AsyncWebServerRequest *request)
                      { request->send(LittleFS, "/index.html", "text/html"); });
    server.begin();
    Serial.printf("WebServer 已启动，监听端口 %d\n", port);
}
