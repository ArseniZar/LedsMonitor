#pragma once
#ifndef APP_H
#define APP_H

#include <Arduino.h>
#include "Logger.h"
#include "ConfigManager.h"
#include "MacAddress.h"
#include "DeviceLed.h"
#include "NetworkManager.h"
#include "TelegramBot.h"
#include "WebServer.h"

#ifndef LOGGER_DEBUG_MODE
#define LOGGER_DEBUG_MODE 1
#endif

#ifndef ENABLE_NETWORK_MODULE
#define ENABLE_NETWORK_MODULE 1
#endif

#ifndef ENABLE_WEBSERVER_MODULE
#define ENABLE_WEBSERVER_MODULE 1
#endif

#ifndef ENABLE_TELEGRAM_BOT_MODULE
#define ENABLE_TELEGRAM_BOT_MODULE 1
#endif

#ifndef ENABLE_DEVICE_MODULE
#define ENABLE_DEVICE_MODULE 1
#endif

#ifndef ENABLE_CONFIG_MODULE
#define ENABLE_CONFIG_MODULE 1
#endif

class App
{
public:
    static App &init();
    void begin();
    void update();

private:
    using DeviceLedConfigPair = ConfigPair<DeviceLedConfig, DeviceLedRuntimeConfig>;
    using NetworkConfigPair = ConfigPair<NetworkConfig, NetworkRuntimeConfig>;
    using TelegramBotConfigPair = ConfigPair<TelegramBotConfig, TelegramBotRuntimeConfig>;
    using WebServerConfigPair = ConfigPair<WebServerConfig, WebServerRuntimeConfig>;

    Logger &logger;
    ConfigManager<DeviceLedConfigPair, NetworkConfigPair, TelegramBotConfigPair, WebServerConfigPair> &config;
    NetworkManager &network;
    WebServer &server;
    MacAddress &mac;
    TelegramBot &bot;
    DeviceLed<NeoBrgFeature, NeoEsp8266Dma800KbpsMethod> device;

    App();
    void registerCommands();
    void registerRoutes();
};
#endif // APP_H