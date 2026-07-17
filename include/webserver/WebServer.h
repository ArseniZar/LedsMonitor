#pragma once
#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <Arduino.h>
#include <StringN.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "Logger.h"
#include "index.h"
#include "ApiModels.h"
#include "ApiParse.h"
#include "ApiSerialization.h"
#include "utils/helpers.h"
#include "WebServerConfigModels.h"

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
    ESP8266WebServer server;

    bool serverRunning;
    unsigned long lastRequestTime;

    WebServer() = delete;
    WebServer(Logger &logger, int port);

    void handleRoot();
    void handleNotFound();

    

    struct StrCompare {bool operator()(const char* a, const char* b) const { return strcmp(a, b) < 0; }};
    std::map<const char*, uint8_t, StrCompare> routeMasks;
    void handleOptions(const char* uri);


};

#include "WebServer.tpp"
#endif // WEBSERVER_H
