#include "NetworkConfigModels.h"

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

bool NetworkConfig::operator==(const NetworkRuntimeConfig &config) const
{
    return (ssid == config.ssid) &&
           (password == config.password) &&
           (apSsid == config.apSsid) &&
           (apPassword == config.apPassword) &&
           (mdnsName == config.mdnsName) &&
           (wifiConnectionTimeout == config.wifiConnectionTimeout);
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