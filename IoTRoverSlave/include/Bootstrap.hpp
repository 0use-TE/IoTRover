#include <Arduino.h>
#include "FSModule.hpp"
#include "WifiModule.hpp"
#include "Settings.h"
#include "WebServerModule.hpp"
class Bootstrap
{
private:
    WebServerModule *webServerModule;

public:
    void setup();
    void loop();
};

void Bootstrap::setup()
{
    Serial.begin(115200);
    FSModule fsModule;
    fsModule.init();
    WifiModule WifiModule(WifiName, WifiPassword);
    bool wifiConnected = WifiModule.Init();
    // 无法连接，进入AP模式
    if (!wifiConnected)
    {
        
    }
    // 开启Web服务器
    webServerModule = new WebServerModule(WebServerPort);
    webServerModule->init();
}
void Bootstrap::loop()
{
}
