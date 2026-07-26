#include "NetworkConfigModels.h"

NetworkConfig *NetworkConfig::defaultInstance = nullptr;

NetworkConfig::NetworkConfig(const NetworkConfig &def)
    : ssid(def.ssid), password(def.password),
      apSsid(def.apSsid), apPassword(def.apPassword),
      mdnsName(def.mdnsName),
      wifiConnectionTimeoutMs(def.wifiConnectionTimeoutMs)
{
}

NetworkConfig::NetworkConfig(const char *ssid, const char *password,
                             const char *apSsid, const char *apPassword,
                             const char *mdnsName,
                             uint32_t wifiConnectionTimeoutMs)
    : ssid(ssid), password(password),
      apSsid(apSsid), apPassword(apPassword),
      mdnsName(mdnsName),
      wifiConnectionTimeoutMs(wifiConnectionTimeoutMs)
{
}

const NetworkConfig &NetworkConfig::defaultConfig(const char *ssid,
                                                  const char *password,
                                                  const char *apSsid,
                                                  const char *apPassword,
                                                  const char *mdnsName,
                                                  uint32_t wifiConnectionTimeoutMs)
{
    if (!defaultInstance)
    {
        defaultInstance = new NetworkConfig(ssid, password, apSsid, apPassword, mdnsName, wifiConnectionTimeoutMs);
    }
    return *defaultInstance;
}

const NetworkConfig &NetworkConfig::fromDefault()
{
    return *defaultInstance;
}

void NetworkConfig::operator=(const NetworkRuntimeConfig &config)
{
    ssid = config.ssid;
    password = config.password;
    apSsid = config.apSsid;
    apPassword = config.apPassword;
    mdnsName = config.mdnsName;
    wifiConnectionTimeoutMs = config.wifiConnectionTimeoutMs;
}

void NetworkConfig::operator=(const NetworkConfig &config)
{
    ssid = config.ssid;
    password = config.password;
    apSsid = config.apSsid;
    apPassword = config.apPassword;
    mdnsName = config.mdnsName;
    wifiConnectionTimeoutMs = config.wifiConnectionTimeoutMs;
}

bool NetworkConfig::operator==(const NetworkRuntimeConfig &config) const
{//FIXME: Временный костыль из-за отсутствия operator== в StringN.
    return (wifiConnectionTimeoutMs == config.wifiConnectionTimeoutMs) &&
           (strcmp(ssid.c_str(), config.ssid.c_str()) == 0) &&
           (strcmp(password.c_str(), config.password.c_str()) == 0) &&
           (strcmp(apSsid.c_str(), config.apSsid.c_str()) == 0) &&
           (strcmp(apPassword.c_str(), config.apPassword.c_str()) == 0) &&
           (strcmp(mdnsName.c_str(), config.mdnsName.c_str()) == 0);
}

NetworkRuntimeConfig::NetworkRuntimeConfig(const NetworkConfig &config)
    : ssid(config.ssid), password(config.password),
      apSsid(config.apSsid), apPassword(config.apPassword),
      mdnsName(config.mdnsName),
      wifiConnectionTimeoutMs(config.wifiConnectionTimeoutMs)
{
}

void NetworkRuntimeConfig::operator=(const NetworkConfig &config)
{
    ssid = config.ssid;
    password = config.password;
    apSsid = config.apSsid;
    apPassword = config.apPassword;
    mdnsName = config.mdnsName;
    wifiConnectionTimeoutMs = config.wifiConnectionTimeoutMs;
}