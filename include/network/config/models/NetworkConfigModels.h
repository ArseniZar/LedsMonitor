#pragma once
#ifndef NETWORK_CONFIG_MODELS_H
#define NETWORK_CONFIG_MODELS_H

#include <Arduino.h>
#include <StringN.h>

struct NetworkRuntimeConfig;

class NetworkConfig
{
public:
    String32 ssid;
    String32 password;
    String32 apSsid;
    String32 apPassword;
    String32 mdnsName;
    uint32_t wifiConnectionTimeoutMs;

    static const NetworkConfig &defaultConfig(const char *ssid,
                                              const char *password,
                                              const char *apSsid,
                                              const char *apPassword,
                                              const char *mdnsName,
                                              uint32_t wifiConnectionTimeoutMs);

    static const NetworkConfig &fromDefault();
    static constexpr const char *TYPE_NAME = "NetworkConfig";

    NetworkConfig(const NetworkConfig &def);

    void operator=(const NetworkRuntimeConfig &config);
    void operator=(const NetworkConfig &config);
    bool operator==(const NetworkRuntimeConfig &config) const;

private:
    NetworkConfig(const char *ssid, const char *password,
                  const char *apSsid, const char *apPassword,
                  const char *mdnsName,
                  uint32_t wifiConnectionTimeoutMs);

    static NetworkConfig *defaultInstance;
};

struct NetworkRuntimeConfig
{
    static constexpr const char *PATH = "/network";
    static constexpr uint8_t KEY = 'N';

    String32 ssid;
    String32 password;
    String32 apSsid;
    String32 apPassword;
    String32 mdnsName;
    uint32_t wifiConnectionTimeoutMs;
    NetworkRuntimeConfig() = default;
    NetworkRuntimeConfig(const NetworkConfig &config);
    void operator=(const NetworkConfig &config);
};

#endif // NETWORK_CONFIG_MODELS_H