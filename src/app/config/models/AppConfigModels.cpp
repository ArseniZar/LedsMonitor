#include "AppConfigModels.h"

AppConfig *AppConfig::defaultInstance = nullptr;

AppConfig::AppConfig(const AppConfig &def)
    : webServerInactivityPeriodMs(def.webServerInactivityPeriodMs),
      wifiReconnectPeriodMs(def.wifiReconnectPeriodMs),
      applyConfigTimeoutMs(def.applyConfigTimeoutMs),
      saveConfigTimeoutMs(def.saveConfigTimeoutMs),
      holdButtonTimeoutMs(def.holdButtonTimeoutMs),
      buttonPin(def.buttonPin)
{
}

AppConfig::AppConfig(
    uint32_t webServerInactivityPeriodMs,
    uint32_t wifiReconnectPeriodMs,
    uint32_t applyConfigTimeoutMs,
    uint32_t saveConfigTimeoutMs,
    uint32_t holdButtonTimeoutMs,
    uint8_t buttonPin) : webServerInactivityPeriodMs(webServerInactivityPeriodMs),
                         wifiReconnectPeriodMs(wifiReconnectPeriodMs),
                         applyConfigTimeoutMs(applyConfigTimeoutMs),
                         saveConfigTimeoutMs(saveConfigTimeoutMs),
                         holdButtonTimeoutMs(holdButtonTimeoutMs),
                         buttonPin(buttonPin) {}

const AppConfig &AppConfig::defaultConfig(
    uint32_t webServerInactivityPeriodMs,
    uint32_t wifiReconnectPeriodMs,
    uint32_t applyConfigTimeoutMs,
    uint32_t saveConfigTimeoutMs,
    uint32_t holdButtonTimeoutMs,
    uint8_t buttonPin)
{
    if (!defaultInstance)
    {
        defaultInstance = new AppConfig(webServerInactivityPeriodMs, wifiReconnectPeriodMs, applyConfigTimeoutMs, saveConfigTimeoutMs, holdButtonTimeoutMs, buttonPin);
    }
    return *defaultInstance;
}

const AppConfig &AppConfig::fromDefault()
{
    return *defaultInstance;
}

void AppConfig::operator=(const AppRuntimeConfig &config)
{
    webServerInactivityPeriodMs = config.webServerInactivityPeriodMs;
    wifiReconnectPeriodMs = config.wifiReconnectPeriodMs;
    applyConfigTimeoutMs = config.applyConfigTimeoutMs;
    saveConfigTimeoutMs = config.saveConfigTimeoutMs;
    holdButtonTimeoutMs = config.holdButtonTimeoutMs;
}

void AppConfig::operator=(const AppConfig &config)
{
    webServerInactivityPeriodMs = config.webServerInactivityPeriodMs;
    wifiReconnectPeriodMs = config.wifiReconnectPeriodMs;
    applyConfigTimeoutMs = config.applyConfigTimeoutMs;
    saveConfigTimeoutMs = config.saveConfigTimeoutMs;
    holdButtonTimeoutMs = config.holdButtonTimeoutMs;
}

bool AppConfig::operator==(const AppRuntimeConfig &config) const
{
    return webServerInactivityPeriodMs == config.webServerInactivityPeriodMs &&
           wifiReconnectPeriodMs == config.wifiReconnectPeriodMs &&
           applyConfigTimeoutMs == config.applyConfigTimeoutMs &&
           saveConfigTimeoutMs == config.saveConfigTimeoutMs &&
           holdButtonTimeoutMs == config.holdButtonTimeoutMs;
} 

AppRuntimeConfig::AppRuntimeConfig(const AppConfig &config)
    : webServerInactivityPeriodMs(config.webServerInactivityPeriodMs),
      wifiReconnectPeriodMs(config.wifiReconnectPeriodMs),
      applyConfigTimeoutMs(config.applyConfigTimeoutMs),
      saveConfigTimeoutMs(config.saveConfigTimeoutMs),
      holdButtonTimeoutMs(config.holdButtonTimeoutMs)
{
}

void AppRuntimeConfig::operator=(const AppConfig &config)
{
    webServerInactivityPeriodMs = config.webServerInactivityPeriodMs;
    wifiReconnectPeriodMs = config.wifiReconnectPeriodMs;
    applyConfigTimeoutMs = config.applyConfigTimeoutMs;
    saveConfigTimeoutMs = config.saveConfigTimeoutMs;
    holdButtonTimeoutMs = config.holdButtonTimeoutMs;
}