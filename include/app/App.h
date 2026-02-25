#pragma once
#ifndef APP_H
#define APP_H

#include <Arduino.h>
#include "config.h"
#include "Logger.h"
#include "Storage.h"
#include "MacAddress.h"
#include "DeviceLed.h"
#include "NetworkManager.h"
#include "TelegramBot.h"
#include "TelegramCommands.h"
#include "LedDeviceModels.h"

#ifndef LOGGER_DEBUG_MODE
#define LOGGER_DEBUG_MODE 1
#endif

#ifndef ENABLE_WIFI_MODULE
#define ENABLE_WIFI_MODULE 1
#endif

#ifndef ENABLE_TELEGRAM_BOT_MODULE
#define ENABLE_TELEGRAM_BOT_MODULE 1
#endif

#ifndef ENABLE_DEVICE_MODULE
#define ENABLE_DEVICE_MODULE 1
#endif

#if ENABLE_STORAGE_MODULE
#define ENABLE_STORAGE_MODULE 1
#endif
class App
{
public:
    static App &init();
    void begin();
    void update();

private:
    SavedWifiData savedWifiData;
    Storage<SavedWifiData> storage;

    Logger &logger;
    NetworkManager &network;
    MacAddress &mac;
    TelegramBot &bot;
    DeviceLed<NeoBrgFeature, NeoEsp8266Dma800KbpsMethod> device;

    App();
    void commitWiFiIfChanged();
    void bindDeviceToTelegramCommands();
};
#endif // APP_H