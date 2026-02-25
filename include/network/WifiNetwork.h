#pragma once
#ifndef WIFI_NETWORK_H
#define WIFI_NETWORK_H

class WifiNetwork
{
public:
    String32 ssid;
    String32 password;
    int rssi;
    int encryptionType;
    int channel;
    StringN<18> bssid;
    bool hidden;

    WifiNetwork() = delete;
    WifiNetwork(const WifiNetwork &other) = default;
    WifiNetwork(WifiNetwork &&other) = default;
    WifiNetwork(const char *ssid, const char *password, int rssi, int encryptionType, int channel, const char *bssid, bool hidden)
        : ssid(ssid),
          password(password),
          rssi(rssi),
          encryptionType(encryptionType),
          channel(channel),
          bssid(bssid),
          hidden(hidden)
    {
    }
};

#endif