#pragma once
#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <Arduino.h>
#include <StringN.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <functional>
#include "Logger.h"
#include "index.h"
#include "DeviceModels.h"
#include "ApiParse.h"
#include "ApiSerialization.h"

namespace espweb
{
    class WebServer
    {
    public:
        WebServer() = delete;
        WebServer(Logger &logger, int port);

        void stop();
        void handleClient();
        bool isRunning() const;
        unsigned long getLastRequestTime() const;
        
        void begin(std::function<bool()> scanWifiNetworksStarted,
                   std::function<ScanState()> scanStatus,
                   std::function<std::vector<WifiNetwork>()> getScanWifiNetworksResults,
                   std::function<bool(const char *, const char *)> attemptConnectionStarted,
                   std::function<ConnState()> wifiStatus);

    private:
        Logger &logger;
        ESP8266WebServer server;

        bool serverRunning;
        unsigned long lastRequestTime;

        void handleEnd();
        void handleRoot();
        void handleNotFound();
        void handleScanStarted(std::function<bool()> scanWifiNetworksStarted);
        void handleScanStatus(std::function<ScanState()> scanStatus);
        void handleScanResult(std::function<std::vector<WifiNetwork>()> getScanWifiNetworksResults);
        void handleConnect(std::function<bool(const char *, const char *)> attemptConnectionStarted);
        void handleWifiStatus(std::function<ConnState()> wifiStatus);
    };

}
#endif // WEBSERVER_H
