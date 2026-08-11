#include "WebServer.h"

WebServer &WebServer::init(Logger &logger, int port)
{
    static WebServer instance(logger, port);
    return instance;
}

WebServer::WebServer(Logger &logger, int port)
    : logger(logger),
      server(port),
      serverRunning(false),
      lastUserRequestTime(0),
      captivePortal(false),
      redirectUri(F("")),
      handlers()
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
        logger.log(LOG_INFO, [&]() -> String128 { 
            String128 buf; 
            buf.add(F("(WebServer::onRequest) HTTP Request: ")); 
            buf.add(request.method().c_str()); 
            buf.add(' '); 
            buf.add(request.path().c_str()); 
            return buf; 
        });

        if (parseMethod(request.method()) == HTTPMethod::OPTIONS) {
            ghttp::ServerBase::Headers headers(200);
            headers.add(F("Access-Control-Allow-Headers"), F("Content-Type"));
            server.beginResponse(headers);
            server.send(204);
            return;
        }

        auto it = handlers.find({request.path().c_str(), parseMethod(request.method())});

        if (it != handlers.end()) {
            const auto type = it->first.getType();
            
            if (type == EndpointType::User) {
                this->lastUserRequestTime = millis();
            } else if (type == EndpointType::System) {
                this->lastSystemRequestTime = millis();
            }

            it->second(request);
        } 
        else 
        {   
            this->lastCaptiveRequestTime = millis();
            this->handleNotFound(request);
        } });
}

void WebServer::start()
{
    logger.log(LOG_INFO, [&]() -> String128
               { String128 buf; buf.add(F("(WebServer::begin) Starting Web Server...")); return buf; });
    server.begin();
    serverRunning = true;
    lastUserRequestTime = millis();

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
    if (captivePortal)
    {
        Text path = request.path();

        if (path.endsWith(F(".ico")) || path.endsWith(F(".png")) || path.endsWith(F(".map")) || path.endsWith(F(".dat")) || path.endsWith(F(".da")))
        {
            logger.log(LOG_WARN, [&]() -> String128
                       {String128 buf; buf.add(F("(WebServer::handleNotFound) ")); buf.add(request.path().c_str()); buf.add(" "); buf.add(request.method().c_str()); buf.add(F(" Resource not found.")); return buf; });

            const auto responce = api::ErrorResponse(404, String32(F("Resource not found")));
            const gson::Str payload = api::serializeResponse(responce);
            logger.log(LOG_DEBUG, [&]() -> String128
                       {String128 buf; buf.add(F("(WebServer::handleNotFound) Response payload: ")); buf.add(Text(payload).c_str()); return buf; });
            server.sendSingle(payload, responce.getCode(), F("application/json"));
            return;
        }

        logger.log(LOG_WARN, [&]() -> String128
                   {String128 buf; buf.add(F("(WebServer::handleNotFound) Captive redirect: ")); buf.add(path.c_str()); return buf; });

        ghttp::ServerBase::Headers headers(302);
        headers.add(F("Location"), Text(redirectUri));
        headers.add(F("Connection"), F("close"));
        headers.add(F("Cache-Control"), F("public, max-age=60")); //TODO проверить, нужно ли кешировать редирект
        server.beginResponse(headers);
        server.send(302);
        return;
    }

    logger.log(LOG_WARN, [&]() -> String128
               {String128 buf; buf.add(F("(WebServer::handleNotFound) ")); buf.add(request.path().c_str()); buf.add(" "); buf.add(request.method().c_str()); buf.add(F(" Resource not found.")); return buf; });

    const auto responce = api::ErrorResponse(404, String32(F("Resource not found")));
    const gson::Str payload = api::serializeResponse(responce);
    logger.log(LOG_DEBUG, [&]() -> String128
               {String128 buf; buf.add(F("(WebServer::handleNotFound) Response payload: ")); buf.add(Text(payload).c_str()); return buf; });
    server.sendSingle(payload, responce.getCode(), F("application/json"));
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

void WebServer::startCaptivePortal(const char *apIpAddress)
{
    logger.log(LOG_DEBUG, [&]() -> String128
               { String128 buf; buf = F("(WebServer::startCaptivePortal) Starting Captive Portal..."); return buf; });

    captivePortal = true;
    redirectUri.clear();
    redirectUri.add(F("http://"));
    redirectUri.add(apIpAddress);
    redirectUri.add(F("/"));

    logger.log(LOG_INFO, [&]() -> String128
               { String128 buf; buf = F("(WebServer::startCaptivePortal) Captive Portal started successfully "); buf.add(redirectUri); return buf; });
}

void WebServer::stopCaptivePortal()
{
    logger.log(LOG_DEBUG, [&]() -> String128
               { String128 buf; buf = F("(WebServer::stopCaptivePortal) Stoping Captive Portal..."); return buf; });

    captivePortal = false;

    logger.log(LOG_INFO, [&]() -> String128
               { String128 buf; buf = F("(WebServer::stopCaptivePortal) Captive Portal stopped successfully."); return buf; });
}

bool WebServer::isCaptivePortalRunning() const
{
    return captivePortal;
}

void WebServer::applyConfig(const WebServerConfig &config)
{
    logger.log(LOG_DEBUG, [&]() -> String128
               { String128 buf; buf = F("(WebServer::applyConfig) All no changed"); return buf; });
}

uint32_t WebServer::getLastUserRequestTime() const
{
    return lastUserRequestTime;
}

uint32_t WebServer::getLastSystemRequestTime() const
{
    return lastSystemRequestTime;
}

uint32_t WebServer::getLastCaptiveRequestTime() const
{
    return lastCaptiveRequestTime;
}