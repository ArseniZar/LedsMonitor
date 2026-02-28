#pragma once
#ifndef CONFIG_MODELS_H
#define CONFIG_MODELS_H

#include <Arduino.h>
#include <StringN.h>

/* ================= NetworkConfig ======================= */

class NetworkRuntimeConfig;

class NetworkConfig
{
public:
    String32 ssid;
    String32 password;
    String32 apSsid;
    String32 apPassword;
    String32 mdnsName;
    unsigned long wifiConnectionTimeout;

    static const NetworkConfig &defaultConfig(const char *ssid,
                                              const char *password,
                                              const char *apSsid,
                                              const char *apPassword,
                                              const char *mdnsName,
                                              unsigned long wifiConnectionTimeout);

    static NetworkConfig fromDefault();

    NetworkConfig(const NetworkConfig &def);

    void operator=(const NetworkRuntimeConfig &config);
    void operator=(const NetworkConfig &config);

private:
    NetworkConfig(const char *ssid, const char *password,
                  const char *apSsid, const char *apPassword,
                  const char *mdnsName,
                  unsigned long wifiConnectionTimeout);

    static NetworkConfig* defaultInstance;
};

struct NetworkRuntimeConfig
{
    String32 ssid;
    String32 password;
    String32 apSsid;
    String32 apPassword;
    String32 mdnsName;
    unsigned long wifiConnectionTimeout;
    NetworkRuntimeConfig() = default;
    NetworkRuntimeConfig(const NetworkConfig &config);
    void operator=(const NetworkConfig &config);
};

/* ================= DeviceLedConfig ======================= */

class DeviceLedRuntimeConfig;

class DeviceLedConfig
{
public:
    const uint8_t pin;
    uint16_t countLed;
    String32 deviceName;

    static const DeviceLedConfig &defaultConfig(uint8_t pin, uint16_t countLed, const char *deviceName);

    static DeviceLedConfig fromDefault();

    DeviceLedConfig(const DeviceLedConfig &def);

    void operator=(const DeviceLedRuntimeConfig &config);
    void operator=(const DeviceLedConfig &config);

private:
    DeviceLedConfig(uint8_t p, uint16_t c, const char *name);
    static DeviceLedConfig* defaultInstance;
};

struct DeviceLedRuntimeConfig
{
    uint16_t countLed;
    String32 deviceName;
    DeviceLedRuntimeConfig() = default;
    DeviceLedRuntimeConfig(const DeviceLedConfig &config);
    void operator=(const DeviceLedConfig &config);
};

/* ================= TelegramBotConfig ======================= */

class TelegramBotRuntimeConfig;

class TelegramBotConfig
{
public:
    String64 token;
    uint8_t limitMessage;
    uint16_t periodUpdate;

    static const TelegramBotConfig &defaultConfig(const char *token,
                                                  uint8_t limitMessage,
                                                  uint16_t periodUpdate);

    static TelegramBotConfig fromDefault();

    TelegramBotConfig(const TelegramBotConfig &def);

    void operator=(const TelegramBotRuntimeConfig &config);
    void operator=(const TelegramBotConfig &config);

private:
    TelegramBotConfig(const char *token, uint8_t limitMessage, uint16_t periodUpdate);

    static TelegramBotConfig* defaultInstance;
};

struct TelegramBotRuntimeConfig
{
    String64 token;
    uint8_t limitMessage;
    uint16_t periodUpdate;
    TelegramBotRuntimeConfig() = default;
    TelegramBotRuntimeConfig(const TelegramBotConfig &config);
    void operator=(const TelegramBotConfig &config);
};

#endif // CONFIG_MODELS_H