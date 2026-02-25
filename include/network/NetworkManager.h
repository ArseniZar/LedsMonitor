#pragma once
#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <Arduino.h>
#include <StringN.h>
#include <vector>
#include "Logger.h"
#include "WebServer.h"
#include "WifiNetwork.h"
#include "WifiTypes.h"

#ifndef WIFI_CONNECTION_TIMEOUT_MS
#define WIFI_CONNECTION_TIMEOUT_MS 10000
#endif

#ifndef WIFI_RETRY_DELAY_MS
#define WIFI_RETRY_DELAY_MS 60000
#endif

#ifndef WIFI_AFTER_WEBSERVER_IDLE_MS
#define WIFI_AFTER_WEBSERVER_IDLE_MS 120000
#endif

class NetworkManager
{
public:
    bool begin();
    static NetworkManager &init(Logger &logger, const char* apSsid, const char* apPassword, const char* mdnsName);
    bool attemptConnection(const char *ssid, const char *password);
    bool attemptConnectionAsync(const char *ssid, const char *password);
    void setAPConfig(const char *apSsid, const char *apPassword);
    void setWiFiConfig(const char *ssid, const char *password);
    void setMdnsName(const char *mdnsName);
    StringN<18> getMacAddress() const;
    const char *getSsid() const;
    const char *getPass() const;
    ConnState statusWifi();
    ScanState statusScan();

private:
    NetworkManager(Logger &logger, const char* apSsid, const char* apPassword, const char* mdnsName);

    Logger &logger;
    espweb::WebServer webserver;

    String32 apSsid;
    String32 apPassword;

    String32 ssid;
    String32 password;

    String32 attemptSsid;
    String32 attemptPassword;

    String32 mdnsName;
    
    static constexpr uint8_t MAX_WIFI_HANDLER = 2;;
    WiFiEventHandler onGotIpHandlers[MAX_WIFI_HANDLER];
    WiFiEventHandler onDisconnectedHandlers[MAX_WIFI_HANDLER];

    void initServer();
    void configureWifiPerformance();

    bool scanWifiNetworksAsync();
    std::vector<WifiNetwork> scanWifiNetworks();
    std::vector<WifiNetwork> getScanWifiNetworksAsyncResults();

    bool stopAP();
    bool stopMDNS();
    bool startMDNS(const String32 &mdnsName);
    bool startAP(const String32 &apSsid, const String32 &apPassword);

    bool tryConnectWifi(const String32 &ssid, const String32 &password);
    bool tryConnectWifiAsync(const String32 &ssid, const String32 &password);

};

#endif // NETWORK_MANAGER_H
