#include <Arduino.h>
#include "FSModule.hpp"
#include "WifiModule.hpp"
#include "Settings.h"
#include "WebServerModule.hpp"
#include "APModule.hpp"
#include "WebSocketModule.hpp"
#include "ControlModule.hpp"
class Bootstrap
{
private:
    WebServerModule *webServerModule;
    WebSocketModule *webSocketModule;
    ControlModule *controlModule;

public:
    void setup();
    void loop();
};

void Bootstrap::setup()
{
    // 开启串口
    Serial.begin(115200);
    // 挂载文件系统
    FSModule fsModule;
    fsModule.init();
    // 连接网络
    WifiModule WifiModule(WifiName, WifiPassword);
    bool wifiConnected = WifiModule.init();
    // 无法连接，进入AP模式
    if (!wifiConnected)
    {
        APModule apModule(APName, APPassword);
        apModule.init();
    }
    // 控制模块
    controlModule = new ControlModule(ENA, ENB, IN1, IN2, IN3, IN4, ENAChannel, ENBChannel);
    controlModule->init();
    // 开启Web服务器
    webServerModule = new WebServerModule(WebServerPort);
    webServerModule->init();
    // 启动WebSocket服务
    webSocketModule = new WebSocketModule(WebSocketPort);
    webSocketModule->init();
}

void Bootstrap::loop()
{
    webSocketModule->loop();
}
