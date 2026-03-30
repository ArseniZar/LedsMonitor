#pragma once
#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <Arduino.h>
#include <StringN.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "Logger.h"
#include "index.h"
#include "ApiConfigModels.h"
#include "ApiParse.h"
#include "ApiSerialization.h"

class WebServer
{
public:
    void begin();  

    ~WebServer() = default;

    void stop();
    void handleClient();
    bool isRunning() const;
    unsigned long getLastRequestTime() const;

    template <typename T, typename E = void>
    void registerRoutes(const char * uri, HTTPMethod httpMethod, std::function<E(T &)> handler);
    
    template <typename T = void, typename E = void>
    void registerRoutes(const char * uri, HTTPMethod httpMethod, std::function<E()> handler);

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

    void handleCorsOptions(const char* uri);

};

#include "WebServer.tpp"
#endif // WEBSERVER_H
