#include "AppConfigModels.h"

AppConfig *AppConfig::defaultInstance = nullptr;

AppConfig::AppConfig(const AppConfig &def)
    : webServerInactivityPeriodMs(def.webServerInactivityPeriodMs),
      wifiReconnectPeriodMs(def.wifiReconnectPeriodMs),
      applyConfigTimeoutMs(def.applyConfigTimeoutMs),
      saveConfigTimeoutMs(def.saveConfigTimeoutMs)
{
}

AppConfig::AppConfig(
    uint32_t webServerInactivityPeriodMs,
    uint32_t wifiReconnectPeriodMs,
    uint32_t applyConfigTimeoutMs,
    uint32_t saveConfigTimeoutMs) : webServerInactivityPeriodMs(webServerInactivityPeriodMs),
                                    wifiReconnectPeriodMs(wifiReconnectPeriodMs),
                                    applyConfigTimeoutMs(applyConfigTimeoutMs),
                                    saveConfigTimeoutMs(saveConfigTimeoutMs) {}

const AppConfig &AppConfig::defaultConfig(
    uint32_t webServerInactivityPeriodMs,
    uint32_t wifiReconnectPeriodMs,
    uint32_t applyConfigTimeoutMs,
    uint32_t saveConfigTimeoutMs)
{
    if (!defaultInstance)
    {
        defaultInstance = new AppConfig(webServerInactivityPeriodMs, wifiReconnectPeriodMs, applyConfigTimeoutMs, saveConfigTimeoutMs);
    }
    return *defaultInstance;
}

const AppConfig &AppConfig::fromDefault()
{
    return *defaultInstance;
}

void AppConfig::operator=(const AppRuntimeConfig &config)
{
}

void AppConfig::operator=(const AppConfig &config)
{
}

bool AppConfig::operator==(const AppRuntimeConfig &config) const
{
    return true;
}

AppRuntimeConfig::AppRuntimeConfig(const AppConfig &config) {}

void AppRuntimeConfig::operator=(const AppConfig &config)
{
}