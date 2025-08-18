#pragma once
#ifndef WIFI_SETUP_H
#define WIFI_SETUP_H

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <Arduino.h>
#include <GSON.h>
#include <vector>
#include "Logger.h"
#include "Network.h"
#include "WebServer.h"

#define WIFI_CONNECTION_TIMEOUT_MS 10000

enum class ConnState : int
{
    WL_NO_SHIELD        = 255,
    WL_IDLE_STATUS      = 0,
    WL_NO_SSID_AVAIL    = 1,
    WL_SCAN_COMPLETED   = 2,
    WL_CONNECTED        = 3,
    WL_CONNECT_FAILED   = 4,
    WL_CONNECTION_LOST  = 5,
    WL_WRONG_PASSWORD   = 6,
    WL_DISCONNECTED     = 7
};


class WiFiSetup
{
public:
    bool begin();
    static WiFiSetup &init(Logger &logger);
    bool attemptConnection(const String &ssid, const String &password);
    void setAPConfig(const String &apSsid, const String &apPassword);
    void setWiFiConfig(const String &ssid, const String &password);
    void setMdnsName(const String &mdnsName);
    const String getMAC() const;
    const String &getSsid() const;
    const String &getPass() const;
    ConnState status();

private:
    WiFiSetup(Logger &logger);

    Logger &logger;
    espweb::WebServer webserver;

    String apSsid;
    String apPassword;

    String ssid;
    String password;

    String mdnsName;

    bool stopAP();
    bool stopMDNS();
    bool initServer();
    std::vector<Network> scanNetworks();
    bool startMDNS(const String &mdnsName);
    bool tryConnectWifi(const String &ssid, const String &password);
    bool startAP(const String &apSsid, const String &apPassword);
};

#endif // WIFI_SETUP_H
