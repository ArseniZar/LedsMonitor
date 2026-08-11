#include "WebServerConfigModels.h"

WebServerConfig *WebServerConfig::defaultInstance = nullptr;

WebServerConfig::WebServerConfig(const WebServerConfig &def) : port(def.port) {}

WebServerConfig::WebServerConfig(uint16_t port) : port(port) {}

const WebServerConfig &WebServerConfig::defaultConfig(uint16_t port)
{
    if (!defaultInstance)
    {
        defaultInstance = new WebServerConfig(port);
    }
    return *defaultInstance;
}

const WebServerConfig &WebServerConfig::fromDefault()
{
    return *defaultInstance;
}

void WebServerConfig::operator=(const WebServerRuntimeConfig &config)
{
    // port = config.port; // port is const, cannot be assigned
}

void WebServerConfig::operator=(const WebServerConfig &config)
{
    // port = config.port; // port is const, cannot be assigned
}

bool WebServerConfig::operator==(const WebServerRuntimeConfig &config) const
{
    // return port == config.port; // port is const, cannot be compared
    return true;
}

WebServerRuntimeConfig::WebServerRuntimeConfig(const WebServerConfig &config) {}

void WebServerRuntimeConfig::operator=(const WebServerConfig &config)
{
    // port = config.port; // port is const, cannot be assigned
}