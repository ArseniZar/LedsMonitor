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

enum class EndpointType : uint8_t
{
    System,
    User
};

struct Endpoint
{
    const char *url;
    HTTPMethod method;
    EndpointType type;

    Endpoint() = delete;
    Endpoint(const char *url, HTTPMethod method) : url(url), method(method), type(EndpointType::User) {}
    bool operator<(const Endpoint &other) const
    {
        if (method != other.method)
        {
            return method < other.method;
        }
        return strcmp(url, other.url) < 0;
    }

    void setType(EndpointType type)
    {
        this->type = type;
    }

    EndpointType getType() const
    {
        return type;
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
    void startCaptivePortal(const char *apIpAddress);
    void stopCaptivePortal();
    bool isCaptivePortalRunning() const;
    void applyConfig(const WebServerConfig &config);

    uint32_t getLastUserRequestTime() const;
    uint32_t getLastSystemRequestTime() const;
    uint32_t getLastCaptiveRequestTime() const;

    template <typename T, typename E = void>
    void registerEndpoint(const char *uri, HTTPMethod httpMethod, EndpointType type, std::function<E(T &)> handler);

    template <typename T = void, typename E = void>
    void registerEndpoint(const char *uri, HTTPMethod httpMethod, EndpointType type, std::function<E()> handler);

    static WebServer &init(Logger &logger, int port);

private:
    Logger &logger;
    ghttp::Server<WiFiServer, WiFiClient> server;

    bool serverRunning;

    uint32_t lastUserRequestTime;
    uint32_t lastSystemRequestTime;
    uint32_t lastCaptiveRequestTime;

    bool captivePortal;
    String32 redirectUri;

    std::map<Endpoint, std::function<void(ghttp::ServerBase::Request)>> handlers;

    WebServer() = delete;
    WebServer(Logger &logger, int port);

    void handleRoot();
    void handleNotFound(ghttp::ServerBase::Request request);
};

#include "WebServer.tpp"
#endif // WEBSERVER_H
