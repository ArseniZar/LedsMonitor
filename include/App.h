#pragma once
#ifndef APP_H
#define APP_H

#include <Arduino.h>
#include "config.h"
#include "Logger.h"
#include "Eeprom.h"
#include "MacAddress.h"
#include "DeviceLed.h"
#include "WiFiSetup.h"
#include "TelegramBot.h"
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

class App
{
public:
    static App &init();
    void begin();
    void update();
    ~App() = default;

private:
    String cachedSsid;
    String cachedPass;

    Logger &logger;
    Eeprom &eeprom;
    WiFiSetup &wifi;
    MacAddress &mac;
    TelegramBot &bot;
    DeviceLed<NeoBrgFeature, NeoEsp8266Dma800KbpsMethod> device;

    App();
    void commitWiFiIfChanged();
    void bindDeviceToTelegramCommands();
};
#endif // APP_H