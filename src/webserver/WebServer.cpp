#include "WebServer.h"

namespace espweb
{
    WebServer::WebServer(Logger &logger, int port) : logger(logger), network(nullptr), server(port), serverRunning(false), lastRequestTime(millis())
    {
        MDNS.addService(F("http"), F("tcp"), port);
    }

    /*========================================begin=============================================================*/

    void WebServer::begin(IWebServerNetwork &network)
    {
        this->network = &network;

        server.on(F("/"), HTTP_GET, [this]()
                  { lastRequestTime=millis(); this->handleRoot(); });

        server.on(F("/scan/start"), HTTP_GET, [this]()
                  { lastRequestTime=millis(); this->handleScanStarted(); });

        server.on(F("/scan/status"), HTTP_GET, [this]()
                  { lastRequestTime=millis(); this->handleScanStatus(); });

        server.on(F("/scan/result"), HTTP_GET, [this]()
                  { lastRequestTime=millis(); this->handleScanResult(); });
        
        server.on(F("/connect/start"), HTTP_POST, [this]()
                  { lastRequestTime=millis(); this->handleConnect(); });

        server.on(F("/connect/status"), HTTP_GET, [this]()
                  { lastRequestTime=millis(); this->handleWifiStatus(); });

        server.on(F("/end"), HTTP_GET, [this]()
                  { lastRequestTime=millis(); this->handleEnd(); });

        server.on(F("/"), HTTP_OPTIONS, [this]()
                  {    
                       server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
                       server.sendHeader(F("Access-Control-Allow-Methods"), F("GET, OPTIONS"));
                       server.sendHeader(F("Access-Control-Allow-Headers"), F("Content-Type"));
                       server.send(204); });

       
        server.on(F("/scan/start"), HTTP_OPTIONS, [this]()
                  {
                       server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
                       server.sendHeader(F("Access-Control-Allow-Methods"), F("GET, OPTIONS"));
                       server.sendHeader(F("Access-Control-Allow-Headers"), F("Content-Type"));
                       server.send(204); });
        
        server.on(F("/scan/status"), HTTP_OPTIONS, [this]()
                  {
                       server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
                       server.sendHeader(F("Access-Control-Allow-Methods"), F("GET, OPTIONS"));
                       server.sendHeader(F("Access-Control-Allow-Headers"), F("Content-Type"));
                       server.send(204); });
        
        server.on(F("/scan/result"), HTTP_OPTIONS, [this]()
                  {
                       server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
                       server.sendHeader(F("Access-Control-Allow-Methods"), F("GET, OPTIONS"));
                       server.sendHeader(F("Access-Control-Allow-Headers"), F("Content-Type"));
                       server.send(204); });
        
        server.on(F("/connect/start"), HTTP_OPTIONS, [this]()
                  {
                       server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
                       server.sendHeader(F("Access-Control-Allow-Methods"), F("POST, OPTIONS"));
                       server.sendHeader(F("Access-Control-Allow-Headers"), F("Content-Type"));
                       server.send(204); });

        server.on(F("/connect/status"), HTTP_OPTIONS, [this]()
                  {
                       server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
                       server.sendHeader(F("Access-Control-Allow-Methods"), F("GET, OPTIONS"));
                       server.sendHeader(F("Access-Control-Allow-Headers"), F("Content-Type"));
                       server.send(204); });

        server.on(F("/end"), HTTP_OPTIONS, [this]()
                  {
                       server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
                       server.sendHeader(F("Access-Control-Allow-Methods"), F("GET, OPTIONS"));
                       server.sendHeader(F("Access-Control-Allow-Headers"), F("Content-Type"));
                       server.send(204); });
            
        

        server.onNotFound([this]()
                          { this->handleNotFound(); });

           
        server.begin();
        serverRunning = true;
        logger.log(LOG_INFO, [&]() -> String128
                   { String128 buf; buf.add(F("(WebServer::begin) Server started and running.")); return buf; });
    }

    /*========================================handleRoot=============================================================*/

    void WebServer::handleRoot()
    {
        logger.log(LOG_DEBUG, [&]() -> String128
                   {String128 buf; buf.add(F("(WebServer::handleRoot) Handling root page request.")); return buf; });
        server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
        server.send_P(200, "text/html", index);
    }

    /*======================================== handleScanStarted =============================================================*/
    
    void WebServer::handleScanStarted()
    {
        logger.log(LOG_DEBUG, [&]() -> String128
                   {String128 buf; buf.add(F("(WebServer::handleScanStarted) Processing start scan networks request.")); return buf; });
        const auto responce = api::SuccessResponse<api::ScanWifiNetworkStartedResponse>(200, std::move(api::ScanWifiNetworkStartedResponse(network->scanWifiNetworksAsync(), std::move(api::ModelBaseResponse()))));
        server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
        String payload = api::serializeResponse(responce);
        logger.log(LOG_DEBUG, [&]() -> String128
                   {String128 buf; buf.add(F("(WebServer::handleScanStarted) Response payload: ")); buf.add(payload.c_str()); return buf; });
        server.send(responce.getCode(), F("application/json"), payload);
    }

    /*======================================== handleScanStatus =============================================================*/
    
    void WebServer::handleScanStatus()
    {
        logger.log(LOG_DEBUG, [&]() -> String128
                   {String128 buf; buf.add(F("(WebServer::handleScanStatus) Processing scan status request.")); return buf; });
        const auto responce = api::SuccessResponse<api::GetScanStatusResponse>(200, std::move(api::GetScanStatusResponse(network->getStatusScan(), std::move(api::ModelBaseResponse()))));
        server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
        String payload = api::serializeResponse(responce);
        logger.log(LOG_DEBUG, [&]() -> String128
               {String128 buf; buf.add(F("(WebServer::handleScanStatus) Response payload: ")); buf.add(payload.c_str()); return buf; });
        server.send(responce.getCode(), F("application/json"), payload);
    }

    /*======================================== handleScanResult =============================================================*/

    void WebServer::handleScanResult()
    {
        logger.log(LOG_DEBUG, [&]() -> String128
                   {String128 buf; buf.add(F("(WebServer::handleScanResult) Processing scan result request.")); return buf; });
        const auto responce = api::SuccessResponse<api::ScanWifiNetworkResponse>(200, std::move(api::ScanWifiNetworkResponse(std::move(network->getScanWifiNetworksAsyncResults()), std::move(api::ModelBaseResponse()))));
        server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
        String payload = api::serializeResponse(responce);
        logger.log(LOG_DEBUG, [&]() -> String128
               {String128 buf; buf.add(F("(WebServer::handleScanResult) Response payload: ")); buf.add(payload.c_str()); return buf; });
        server.send(responce.getCode(), F("application/json"), payload);
    }

    /*========================================handleNetwork==========================================================*/

    void WebServer::handleConnect()
    {
        logger.log(LOG_DEBUG, [&]() -> String128
                   {String128 buf; buf.add(F("(WebServer::handleConnect) Received network connect request.")); return buf; });

        const String &body = server.arg(F("plain"));

        logger.log(LOG_DEBUG, [&]() -> String128
               {String128 buf; buf.add(F("(WebServer::handleConnect) Raw request body: ")); buf.add(body.c_str()); return buf; });

        if (body.isEmpty())
        {
            logger.log(LOG_WARN, [&]() -> String128
                       {String128 buf; buf.add(F("(WebServer::handleConnect) Empty request body.")); return buf; });
            const auto responce = api::ErrorResponse(400, String32(F("Empty request body")));
            server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
            String payload = api::serializeResponse(responce);
            server.send(responce.getCode(), F("application/json"), payload);
            logger.log(LOG_DEBUG, [&]() -> String128
                       {String128 buf; buf.add(F("(WebServer::handleConnect) Response payload (error empty body): ")); buf.add(payload.c_str()); return buf; });
            return;
        }

        auto apiRequstPtr = api::parseRequest<api::ConnectWifiNetworkRequest>(body.c_str());
        if (apiRequstPtr->isOk())
        {
            auto *successApiRequstPtr = static_cast<api::SuccessRequest<api::ConnectWifiNetworkRequest> *>(apiRequstPtr.get());
            const bool status = network->attemptConnectionAsync(successApiRequstPtr->data.ssid.c_str(), successApiRequstPtr->data.password.c_str());
            const auto responce = api::SuccessResponse<api::ConnectWifiNetworkStartedResponse>(200, std::move(api::ConnectWifiNetworkStartedResponse(status, std::move(successApiRequstPtr->data))));
            server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
            String payload = api::serializeResponse(responce);
            logger.log(LOG_DEBUG, [&]() -> String128
                       {String128 buf; buf.add(F("(WebServer::handleConnect) Response payload (success): ")); buf.add(payload.c_str()); return buf; });
            server.send(responce.getCode(), F("application/json"), payload);
        }
        else
        {
            auto *errorApiRequstPtr = static_cast<api::ErrorRequest *>(apiRequstPtr.get());
            const auto responce = api::ErrorResponse(400, errorApiRequstPtr->message);
            server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
            String payload = api::serializeResponse(responce);
            logger.log(LOG_DEBUG, [&]() -> String128
                       {String128 buf; buf.add(F("(WebServer::handleConnect) Response payload (parse error): ")); buf.add(payload.c_str()); return buf; });
            server.send(responce.getCode(), F("application/json"), payload);
        }
    }

    /*========================================handleStatusWifi==========================================================*/

    void WebServer::handleWifiStatus()
    {
        logger.log(LOG_DEBUG, [&]() -> String128
                   {String128 buf; buf.add(F("(WebServer::handleWifiStatus) Received WiFi status request.")); return buf; });
        const auto responce = api::SuccessResponse<api::GetWifiStatusResponse>(200, std::move(api::GetWifiStatusResponse(std::move(network->getStatusWifi()), std::move(api::ModelBaseResponse()))));
        server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
        String payload = api::serializeResponse(responce);
        logger.log(LOG_DEBUG, [&]() -> String128
                   {String128 buf; buf.add(F("(WebServer::handleWifiStatus) Response payload: ")); buf.add(payload.c_str()); return buf; });
        server.send(responce.getCode(), F("application/json"), payload);
    }

    /*========================================handleEnd============================================================*/

    void WebServer::handleEnd()
    {
        logger.log(LOG_INFO, [&]() -> String128
                   {String128 buf; buf.add(F("(WebServer::handleEnd) Request accepted, stopping server.")); return buf; });
        server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
        server.send(200);
        stop();
    }

    /*========================================handleNotFound========================================================*/

    void WebServer::handleNotFound()
    {
        logger.log(LOG_WARN, [&]() -> String128
                   {String128 buf; buf.add(F("(WebServer::handleNotFound) Resource not found.")); return buf; });
        const auto responce = api::ErrorResponse(400, String32(F("Resource not found")));
        server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
        String payload = api::serializeResponse(responce);
        logger.log(LOG_DEBUG, [&]() -> String128
               {String128 buf; buf.add(F("(WebServer::handleNotFound) Response payload: ")); buf.add(payload.c_str()); return buf; });
        server.send(responce.getCode(), F("application/json"), payload);
    }

    /*========================================handleClient==========================================================*/

    void WebServer::handleClient()
    {
        server.handleClient();
    }

    /*==============================================stop=============================================================*/

    void WebServer::stop()
    {
        server.stop();
        serverRunning = false;
        logger.log(LOG_INFO, [&]() -> String128
                   {String128 buf; buf.add(F("(WebServer::stop) Stop server.")); return buf; });
    }

    /*========================================isRunning=============================================================*/

    bool WebServer::isRunning() const
    {
        return serverRunning;
    }

    unsigned long WebServer::getLastRequestTime() const
    {
        return lastRequestTime;
    }
}
