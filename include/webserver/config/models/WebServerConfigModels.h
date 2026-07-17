#pragma once
#ifndef WEBSERVER_CONFIG_MODELS_H
#define WEBSERVER_LED_CONFIG_MODELS_H

#include <Arduino.h>
#include <StringN.h>

struct WebServerRuntimeConfig;

class WebServerConfig
{
public:
    const uint16_t port;

    static const WebServerConfig &defaultConfig(uint16_t port);

    static const WebServerConfig &fromDefault();
    static constexpr const char *TYPE_NAME = "WebServerConfig";

    WebServerConfig(const WebServerConfig &def);

    void operator=(const WebServerRuntimeConfig &config);
    void operator=(const WebServerConfig &config);
    bool operator==(const WebServerRuntimeConfig &config) const;

private:
    WebServerConfig(uint16_t port);
    static WebServerConfig *defaultInstance;
};

struct WebServerRuntimeConfig
{
    static constexpr const char *PATH = "/webServer";
    static constexpr uint8_t KEY = 'W';

    WebServerRuntimeConfig() = default;
    WebServerRuntimeConfig(const WebServerConfig &config);
    void operator=(const WebServerConfig &config);
};

#endif // WEBSERVER_LED_CONFIG_MODELS_H