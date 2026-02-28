#include "ConfigModels.h"

/* ================= NetworkConfig ================= */

NetworkConfig *NetworkConfig::defaultInstance = nullptr;

NetworkConfig::NetworkConfig(const NetworkConfig &def)
    : ssid(def.ssid), password(def.password),
      apSsid(def.apSsid), apPassword(def.apPassword),
      mdnsName(def.mdnsName),
      wifiConnectionTimeout(def.wifiConnectionTimeout)
{
}

NetworkConfig::NetworkConfig(const char *ssid, const char *password,
                             const char *apSsid, const char *apPassword,
                             const char *mdnsName,
                             unsigned long wifiConnectionTimeout)
    : ssid(ssid), password(password),
      apSsid(apSsid), apPassword(apPassword),
      mdnsName(mdnsName),
      wifiConnectionTimeout(wifiConnectionTimeout)
{
}

const NetworkConfig &NetworkConfig::defaultConfig(const char *ssid,
                                                  const char *password,
                                                  const char *apSsid,
                                                  const char *apPassword,
                                                  const char *mdnsName,
                                                  unsigned long wifiConnectionTimeout)
{
    if (!defaultInstance)
    {
        defaultInstance = new NetworkConfig(ssid, password, apSsid, apPassword, mdnsName, wifiConnectionTimeout);
    }
    return *defaultInstance;
}

NetworkConfig NetworkConfig::fromDefault()
{
    // Предполагаем, что defaultInstance уже инициализирован через defaultConfig()
    return NetworkConfig(*defaultInstance);
}

void NetworkConfig::operator=(const NetworkRuntimeConfig &config)
{
    ssid = config.ssid;
    password = config.password;
    apSsid = config.apSsid;
    apPassword = config.apPassword;
    mdnsName = config.mdnsName;
    wifiConnectionTimeout = config.wifiConnectionTimeout;
}

void NetworkConfig::operator=(const NetworkConfig &config)
{
    ssid = config.ssid;
    password = config.password;
    apSsid = config.apSsid;
    apPassword = config.apPassword;
    mdnsName = config.mdnsName;
    wifiConnectionTimeout = config.wifiConnectionTimeout;
}

NetworkRuntimeConfig::NetworkRuntimeConfig(const NetworkConfig &config)
    : ssid(config.ssid), password(config.password),
      apSsid(config.apSsid), apPassword(config.apPassword),
      mdnsName(config.mdnsName),
      wifiConnectionTimeout(config.wifiConnectionTimeout)
{
}

void NetworkRuntimeConfig::operator=(const NetworkConfig &config)
{
    ssid = config.ssid;
    password = config.password;
    apSsid = config.apSsid;
    apPassword = config.apPassword;
    mdnsName = config.mdnsName;
    wifiConnectionTimeout = config.wifiConnectionTimeout;
}

/* ================= DeviceLedConfig ================= */

DeviceLedConfig *DeviceLedConfig::defaultInstance = nullptr;

DeviceLedConfig::DeviceLedConfig(const DeviceLedConfig &def)
    : pin(def.pin), countLed(def.countLed), deviceName(def.deviceName)
{
}

DeviceLedConfig::DeviceLedConfig(uint8_t p, uint16_t c, const char *name)
    : pin(p), countLed(c), deviceName(name)
{
}

const DeviceLedConfig &DeviceLedConfig::defaultConfig(uint8_t pin, uint16_t countLed, const char *deviceName)
{
    if (!defaultInstance)
    {
        defaultInstance = new DeviceLedConfig(pin, countLed, deviceName);
    }
    return *defaultInstance;
}

DeviceLedConfig DeviceLedConfig::fromDefault()
{
    return DeviceLedConfig(*defaultInstance);
}

void DeviceLedConfig::operator=(const DeviceLedRuntimeConfig &config)
{
    countLed = config.countLed;
    deviceName = config.deviceName;
}

void DeviceLedConfig::operator=(const DeviceLedConfig &config)
{
    countLed = config.countLed;
    deviceName = config.deviceName;
}

DeviceLedRuntimeConfig::DeviceLedRuntimeConfig(const DeviceLedConfig &config)
    : countLed(config.countLed), deviceName(config.deviceName)
{
}

void DeviceLedRuntimeConfig::operator=(const DeviceLedConfig &config)
{
    countLed = config.countLed;
    deviceName = config.deviceName;
}

/* ================= TelegramBotConfig ================= */

TelegramBotConfig *TelegramBotConfig::defaultInstance = nullptr;

TelegramBotConfig::TelegramBotConfig(const TelegramBotConfig &def)
    : token(def.token), limitMessage(def.limitMessage), periodUpdate(def.periodUpdate)
{
}

TelegramBotConfig::TelegramBotConfig(const char *token, uint8_t limitMessage, uint16_t periodUpdate)
    : token(token), limitMessage(limitMessage), periodUpdate(periodUpdate)
{
}

const TelegramBotConfig &TelegramBotConfig::defaultConfig(const char *token,
                                                          uint8_t limitMessage,
                                                          uint16_t periodUpdate)
{
    if (!defaultInstance)
    {
        defaultInstance = new TelegramBotConfig(token, limitMessage, periodUpdate);
    }
    return *defaultInstance;
}

TelegramBotConfig TelegramBotConfig::fromDefault()
{
    // Предполагаем, что defaultInstance уже инициализирован через defaultConfig()
    return TelegramBotConfig(*defaultInstance);
}

void TelegramBotConfig::operator=(const TelegramBotRuntimeConfig &config)
{
    token = config.token;
    limitMessage = config.limitMessage;
    periodUpdate = config.periodUpdate;
}

void TelegramBotConfig::operator=(const TelegramBotConfig &config)
{
    token = config.token;
    limitMessage = config.limitMessage;
    periodUpdate = config.periodUpdate;
}

TelegramBotRuntimeConfig::TelegramBotRuntimeConfig(const TelegramBotConfig &config)
    : token(config.token), limitMessage(config.limitMessage), periodUpdate(config.periodUpdate)
{
}

void TelegramBotRuntimeConfig::operator=(const TelegramBotConfig &config)
{
    token = config.token;
    limitMessage = config.limitMessage;
    periodUpdate = config.periodUpdate;
}
