#include "WebServer.h"

WebServer &WebServer::init(Logger &logger, int port)
{
    static WebServer instance(logger, port);
    return instance;
}

WebServer::WebServer(Logger &logger, int port) : logger(logger), server(port), serverRunning(false), lastRequestTime(millis()), handlers()
{
    MDNS.addService(F("http"), F("tcp"), port);
}

void WebServer::begin()
{
    handlers[{"/", HTTPMethod::GET}] = [this](ghttp::ServerBase::Request request)
    {
        this->handleRoot();
    };

    server.useCors(true);
    server.onRequest([this](ghttp::ServerBase::Request request)
                    {  
                        lastRequestTime = millis();

                        logger.log(LOG_INFO, [&]() -> String128
                                   { String128 buf; buf.add(F("(WebServer::onRequest) HTTP Request: ")); buf.add(request.method().c_str()); buf.add(' '); buf.add(request.path().c_str()); return buf; });

                        if(parseMethod(request.method()) == HTTPMethod::OPTIONS)
                        {
                            ghttp::ServerBase::Headers headers(200);
                            headers.add(F("Access-Control-Allow-Headers"), F("Content-Type"));
                            server.beginResponse(headers);
                            server.send(204);
                            return;
                        }

                        auto it = handlers.find({request.path().c_str(), parseMethod(request.method())});
                        if (it != handlers.end()) 
                        {
                            it->second(request);
                        } else 
                        {
                            this->handleNotFound(request);
                        }
                    });
}

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
    server.sendFile_P(index_html, "text/html", true);
}

void WebServer::handleNotFound(ghttp::ServerBase::Request request)
{
    logger.log(LOG_WARN, [&]() -> String128
               {String128 buf; buf.add(F("(WebServer::handleNotFound) ")); buf.add(request.path().c_str()); buf.add(" "); buf.add(request.method().c_str()); buf.add(F(" Resource not found.")); return buf; });
    const auto responce = api::ErrorResponse(404, String32(F("Resource not found")));
    const gson::Str payload = api::serializeResponse(responce);
    logger.log(LOG_DEBUG, [&]() -> String128
               {String128 buf; buf.add(F("(WebServer::handleNotFound) Response payload: ")); buf.add(Text(payload).c_str()); return buf; });
    server.send(payload, responce.getCode(), F("application/json"));
}

void WebServer::tick()
{
    server.tick();
}

void WebServer::stop()
{
    server.server.stop();
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
