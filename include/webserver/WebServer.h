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
#include "ApiModels.h"
#include "ApiParse.h"
#include "ApiSerialization.h"
#include "utils/helpers.h"
#include "WebServerConfigModels.h"

enum class HTTPMethod: uint8_t { ANY, GET, HEAD, POST, PUT, PATCH, DELETE, OPTIONS };

inline HTTPMethod parseMethod(const Text& str) {
    switch (str.hash()) {
        case su::SH("GET"):     return HTTPMethod::GET;
        case su::SH("POST"):    return HTTPMethod::POST;
        case su::SH("PUT"):     return HTTPMethod::PUT;
        case su::SH("DELETE"):  return HTTPMethod::DELETE;
        case su::SH("OPTIONS"): return HTTPMethod::OPTIONS;
        case su::SH("HEAD"):    return HTTPMethod::HEAD;
        case su::SH("PATCH"):   return HTTPMethod::PATCH;
        
        default:                return HTTPMethod::ANY;
    }
}

struct Route {
    const char * url;
    HTTPMethod method;
    
    Route() = delete;
    Route(const char * url, HTTPMethod method) : url(url), method(method) {}
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
    void handleNotFound();

    void handleOptions(const char* uri);


};

#include "WebServer.tpp"
#endif // WEBSERVER_H
