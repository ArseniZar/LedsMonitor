#pragma once
#ifndef APP_H
#define APP_H

#include <Arduino.h>
#include <GTimer.h>
#include "Logger.h"
#include "ConfigManager.h"
#include "MacAddress.h"
#include "DeviceLed.h"
#include "NetworkManager.h"
#include "TelegramBot.h"
#include "WebServer.h"
#include "AppConfigModels.h"

#ifndef WEBSERVER_INACTIVITY_PERIOD_MS
#define WEBSERVER_INACTIVITY_PERIOD_MS 180000
#endif

#ifndef WIFI_RECONNECT_PERIOD_MS
#define WIFI_RECONNECT_PERIOD_MS 120000
#endif

#ifndef APPLY_CONFIG_TIMEOUT_MS
#define APPLY_CONFIG_TIMEOUT_MS 2000
#endif

#ifndef SAVE_CONFIG_TIMEOUT_MS
#define SAVE_CONFIG_TIMEOUT_MS 2000
#endif

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
    void start();
    void update();

private:
    App();

    using DeviceLedConfigPair = ConfigPair<DeviceLedConfig, DeviceLedRuntimeConfig>;
    using NetworkConfigPair = ConfigPair<NetworkConfig, NetworkRuntimeConfig>;
    using TelegramBotConfigPair = ConfigPair<TelegramBotConfig, TelegramBotRuntimeConfig>;
    using WebServerConfigPair = ConfigPair<WebServerConfig, WebServerRuntimeConfig>;
    using AppConfigPair = ConfigPair<AppConfig, AppRuntimeConfig>;

    Logger &logger;
    ConfigManager<DeviceLedConfigPair, NetworkConfigPair, TelegramBotConfigPair, WebServerConfigPair, AppConfigPair> &config;
    NetworkManager &network;
    WebServer &server;
    MacAddress &mac;
    TelegramBot &bot;
    DeviceLed<NeoBrgFeature, NeoEsp8266Dma800KbpsMethod> device;

    uint32_t webServerInactivityPeriodMs;
    GTimer<millis> wifiReconnectTimer;
    GTimer<millis> applyConfigTimer;
    GTimer<millis> saveConfigTimer;

    void applyConfig(const AppConfig &config);
    void registerCommands();
    void registerEndpoints();
};
#endif // APP_H