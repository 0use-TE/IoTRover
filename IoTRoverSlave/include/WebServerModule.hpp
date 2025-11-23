#pragma once
#include <Arduino.h>
#include <ESPAsyncWebServer.h>

class WebServerModule
{
private:
    AsyncWebServer *server; // Web 服务器指针
    uint16_t port;          // 服务器监听的端口

public:
    // 构造函数：传入端口号并初始化 Web 服务器
    WebServerModule(uint16_t serverPort);

    // 析构函数：清理资源
    ~WebServerModule();

    // 启动服务器
    void init();
};

WebServerModule::WebServerModule(uint16_t serverPort)
    : port(serverPort)
{
    // 构造函数内可以添加其他初始化代码
    Serial.println("WebServerModule 已初始化");
}

WebServerModule::~WebServerModule()
{
    // 如果需要清理，释放服务器内存
    if (server != nullptr)
    {
        delete server;
    }
}

void WebServerModule::init()
{
    server = new AsyncWebServer(port);
    // 设置静态文件
    server->serveStatic("/", LittleFS, "/").setDefaultFile("index.html");
    // 处理 404 错误：返回 index.html 作为回退
    server->onNotFound([](AsyncWebServerRequest *request)
                       { request->send(LittleFS, "/index.html", "text/html"); });
    // 启动 Web 服务器
    server->begin();
    Serial.printf("WebServer 已启动，监听端口 %d\n", port);
}
