#pragma once
#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>
#include <Arduino.h>
#include <StringN.h>
#include <vector>
#include "Logger.h"
#include "WifiStatus.h"
#include "WifiNetwork.h"
#include "NetworkConfigModels.h"

#ifndef WIFI_ATTEMPT_SSID
#define WIFI_ATTEMPT_SSID ""
#endif

#ifndef WIFI_ATTEMPT_PASS
#define WIFI_ATTEMPT_PASS ""
#endif

#ifndef AP_SSID
#define AP_SSID "SmartHome"
#endif

#ifndef AP_PASS
#define AP_PASS "12345678"
#endif

#ifndef MDNS_NAME
#define MDNS_NAME "smarthome"
#endif

#ifndef WIFI_CONNECTION_TIMEOUT_MS
#define WIFI_CONNECTION_TIMEOUT_MS 10000
#endif

class NetworkManager
{
public:
    bool begin();
    void tick();
    static NetworkManager &init(Logger &logger);

    bool attemptConnection();
    bool attemptConnection(const char *ssid, const char *password);
    bool attemptConnectionAsync();
    bool attemptConnectionAsync(const char *ssid, const char *password);

    bool scanWifiNetworksAsync();
    std::vector<WifiNetwork> scanWifiNetworks();
    std::vector<WifiNetwork> getScanWifiNetworksAsyncResults();
    
    bool startCaptivePortal();
    bool stopCaptivePortal();

    void setAttemptWifiConfig(const char *ssid, const char *password);
    void setAPConfig(const char *apSsid, const char *apPassword);
    void setMdnsName(const char *mdnsName);
    void setWifiConnectionTimeout(unsigned long timeout);
    void applyConfig(const NetworkConfig &config);

    unsigned long getWifiConnectionTimeout() const;
    StringN<18> getMacAddress() const;
    IPAddress getAPIpAddress() const;
    const char *getSsid() const;
    const char *getPass() const;
    const char *getMdnsName() const;
    ConnState getStatusWifi() const;
    ScanState getStatusScan() const;

private:
    NetworkManager(Logger &logger);

    Logger &logger;
    DNSServer dnsServer;

    String32 apSsid;
    String32 apPassword;

    String32 ssid;
    String32 password;

    String32 attemptSsid;
    String32 attemptPassword;

    String32 mdnsName;

    unsigned long wifiConnectionTimeout;

    static constexpr uint8_t MAX_WIFI_HANDLER = 2;
    WiFiEventHandler onGotIpHandlers[MAX_WIFI_HANDLER];
    WiFiEventHandler onDisconnectedHandlers[MAX_WIFI_HANDLER];

    void configureWifiPerformance();

    bool stopMDNS();
    bool startMDNS();
    bool startMDNS(const String32 &mdnsName);
    bool stopAP();
    bool startAP();
    bool startAP(const String32 &apSsid, const String32 &apPassword);
    bool stopDNS();
    bool startDNS();

    bool tryConnectWifi(const String32 &ssid, const String32 &password);
    bool tryConnectWifiAsync(const String32 &ssid, const String32 &password);
};

#endif // NETWORK_MANAGER_H
