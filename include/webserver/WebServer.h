#pragma once
#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <Arduino.h>
#include <StringN.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "Logger.h"
#include "index.h"
#include "DeviceModels.h"
#include "ApiParse.h"
#include "ApiSerialization.h"
#include "IWebServerNetwork.h"

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
        
        void begin(IWebServerNetwork &network);

    private:
        Logger &logger;
        IWebServerNetwork *network;

        ESP8266WebServer server;

        bool serverRunning;
        unsigned long lastRequestTime;

        void handleEnd();
        void handleRoot();
        void handleNotFound();
        void handleScanStarted();
        void handleScanStatus();
        void handleScanResult();
        void handleConnect();
        void handleWifiStatus();
    };

}
#endif // WEBSERVER_H
