#include "WebServer.h"

WebServer &WebServer::init(Logger &logger, int port)
{
    static WebServer instance(logger, port);
    return instance;
}

WebServer::WebServer(Logger &logger, int port) : logger(logger), server(port), serverRunning(false), lastRequestTime(millis()), routeMasks()
{
    MDNS.addService(F("http"), F("tcp"), port);
}

void WebServer::begin()
{
    server.on(F("/"), HTTP_GET, [this]()
              { lastRequestTime=millis(); this->handleRoot(); });

    server.on(F("/"), HTTP_OPTIONS, [this]()
              {    
                       server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
                       server.sendHeader(F("Access-Control-Allow-Methods"), F("GET, OPTIONS"));
                       server.sendHeader(F("Access-Control-Allow-Headers"), F("Content-Type"));
                       server.send(204); });

    server.onNotFound([this]()
                      { lastRequestTime=millis(); this->handleNotFound(); });
} //TODO: может быть доавить коментарий 

void WebServer::start()
{
    logger.log(LOG_INFO, [&]() -> String128
               { String128 buf; buf.add(F("(WebServer::begin) Starting Web Server...")); return buf; });
    server.begin();
    serverRunning = true;

    logger.log(LOG_INFO, [&]() -> String128
               { String128 buf; buf.add(F("(WebServer::begin) Starting Web Server finished successfully")); return buf; });
}

void WebServer::handleRoot()
{
    logger.log(LOG_DEBUG, [&]() -> String128
               {String128 buf; buf.add(F("(WebServer::handleRoot) Handling root page request.")); return buf; });
    server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
    // server.send_P(200, "text/html", INDEX);
    server.send(200, "text/html", index_html, index_html_len);
}

void WebServer::handleNotFound()
{
    logger.log(LOG_WARN, [&]() -> String128
               {String128 buf; buf.add(F("(WebServer::handleNotFound) Resource not found.")); return buf; });
    const auto responce = api::ErrorResponse(404, String32(F("Resource not found")));
    server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
    String payload = api::serializeResponse(responce);
    logger.log(LOG_DEBUG, [&]() -> String128
               {String128 buf; buf.add(F("(WebServer::handleNotFound) Response payload: ")); buf.add(payload.c_str()); return buf; });
    server.send(responce.getCode(), F("application/json"), payload);
}

void WebServer::handleOptions(const char *uri)
{
    String64 buf;
    const uint8_t masks = routeMasks[uri];
    for (uint8_t i = 0; i < 8; i++)
    {
        if (masks & (1 << i))
        {
            if (buf.length() > 0)
            {
                buf.add(F(", "));
            }

            switch (i)
            {
            case HTTP_GET:
                buf.add(F("GET"));
                break;
            case HTTP_POST:
                buf.add(F("POST"));
                break;
            case HTTP_PUT:
                buf.add(F("PUT"));
                break;
            case HTTP_PATCH:
                buf.add(F("PATCH"));
                break;
            case HTTP_DELETE:
                buf.add(F("DELETE"));
                break;
            case HTTP_HEAD:
                buf.add(F("HEAD"));
                break;
            case HTTP_OPTIONS:
                buf.add(F("OPTIONS"));
                break;
            default:
                buf.add(F("ANY"));
                break;
            }
        }
    }

    server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
    server.sendHeader(F("Access-Control-Allow-Methods"), buf.c_str());
    server.sendHeader(F("Access-Control-Allow-Headers"), F("Content-Type"));
    server.send(204);
}

void WebServer::tick()
{
    server.handleClient();
}

void WebServer::stop()
{
    server.stop();
    serverRunning = false;
    logger.log(LOG_INFO, [&]() -> String128
               {String128 buf; buf.add(F("(WebServer::stop) Stop server.")); return buf; });
}

bool WebServer::isRunning() const
{
    return serverRunning;
}

void WebServer::applyConfig(const WebServerConfig &config)
{
    
}

unsigned long WebServer::getLastRequestTime() const
{
    return lastRequestTime;
}
