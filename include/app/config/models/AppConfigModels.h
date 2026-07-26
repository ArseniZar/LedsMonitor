#pragma once
#ifndef APP_CONFIG_MODELS_H
#define APP_CONFIG_MODELS_H

#include <Arduino.h>

struct AppRuntimeConfig;

class AppConfig
{
public:
    const uint32_t webServerInactivityPeriodMs;
    const uint32_t wifiReconnectPeriodMs;
    const uint32_t applyConfigTimeoutMs;
    const uint32_t saveConfigTimeoutMs;


    static const AppConfig &defaultConfig(uint32_t webServerInactivityPeriodMs,
                                          uint32_t wifiReconnectPeriodMs, 
                                          uint32_t applyConfigTimeoutMs,
                                          uint32_t saveConfigTimeoutMs);

    static const AppConfig &fromDefault();
    static constexpr const char *TYPE_NAME = "AppConfig";

    AppConfig(const AppConfig &def);

    void operator=(const AppRuntimeConfig &config);
    void operator=(const AppConfig &config);
    bool operator==(const AppRuntimeConfig &config) const;

private:
    AppConfig(
        uint32_t webServerInactivityPeriodMs, 
        uint32_t wifiReconnectPeriodMs,
        uint32_t applyConfigTimeoutMs,
        uint32_t saveConfigTimeoutMs
    );
    static AppConfig *defaultInstance;
};

struct AppRuntimeConfig
{
    static constexpr const char *PATH = "/app";
    static constexpr uint8_t KEY = 'A';

    AppRuntimeConfig() = default;
    AppRuntimeConfig(const AppConfig &config);
    void operator=(const AppConfig &config);
};

#endif // APP_CONFIG_MODELS_H