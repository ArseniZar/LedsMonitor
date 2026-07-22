#pragma once
#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <Arduino.h>
#include <StringN.h>
#include <GyverHTTP.h>
#include <ESP8266mDNS.h>
#include <ESP8266WiFi.h>
#include "Logger.h"
#include "index.h"
#include "WebServerApiModels.h"
#include "ApiParse.h"
#include "ApiSerialization.h"
#include "utils/helpers.h"
#include "WebServerConfigModels.h"
#include "webserver/HTTPMethod.h"

struct Route {
    const char *url;
    HTTPMethod method;
    
    Route() = delete;
    Route(const char *url, HTTPMethod method) : url(url), method(method) {}
    bool operator<(const Route &other) const {
        if (method != other.method) {
            return method < other.method;
        }
       return strcmp(url, other.url) < 0;
    }
};

class WebServer
{

public:
    void begin();  
    void start();
    void tick();
    void stop();

    ~WebServer() = default;

    bool isRunning() const;
    void applyConfig(const WebServerConfig &config);
    unsigned long getLastRequestTime() const;

    template <typename T, typename E = void>
    void registerRoute(const char * uri, HTTPMethod httpMethod, std::function<E(T &)> handler);
    
    template <typename T = void, typename E = void>
    void registerRoute(const char * uri, HTTPMethod httpMethod, std::function<E()> handler);

    static WebServer &init(Logger &logger, int port);

private:
    Logger &logger;
    ghttp::Server<WiFiServer, WiFiClient> server;

    bool serverRunning;
    unsigned long lastRequestTime;

    std::map<Route, std::function<void(ghttp::ServerBase::Request)>> handlers;
    // std::map<const char*, uint8_t, StrCompare> routeMasks;

    WebServer() = delete;
    WebServer(Logger &logger, int port);

    void handleRoot();
    void handleNotFound(ghttp::ServerBase::Request request);

    // void handleOptions(const char* uri);
    //XXX по удаления коментарии

};

#include "WebServer.tpp"
#endif // WEBSERVER_H
