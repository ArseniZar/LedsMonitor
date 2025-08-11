#pragma once
#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <functional>
#include "Logger.h"
#include "index.h"
#include "Network.h"
#include "ApiTypes.h"
#include "ApiSerialization.h"
#include "ApiParse.h"

namespace espweb
{
    class WebServer
    {
    private:
        Logger &logger;
        ESP8266WebServer server;
        bool serverRunning = false;
        void handleRoot();
        void handleEnd();
        void handleNotFound();

        void handleScan(std::function<std::vector<Network>()>);
        void handleNetwork(std::function<bool(const String &, const String &)> attemptConnection);

    public:
        explicit WebServer(Logger &logger, int port);
        bool begin(std::function<std::vector<Network>()> getNetwork,
                   std::function<bool(const String &, const String &)> attemptConnection);

        void handleClient();
        void stop();
        bool isRunning() const;
    };

}
#endif // WEBSERVER_H
