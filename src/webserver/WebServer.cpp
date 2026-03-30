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

/*========================================begin=============================================================*/

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

    // server.on(F("/network/scan"), HTTP_POST, [this]()
    //           { lastRequestTime=millis(); this->handleScanStarted(); });

    // server.on(F("/network/scan"), HTTP_GET, [this]()
    //           { lastRequestTime=millis(); this->handleScanResult(); });

    // server.on(F("/network/connection"), HTTP_POST, [this]()
    //           { lastRequestTime=millis(); this->handleConnect(); });

    // server.on(F("/network/connection"), HTTP_GET, [this]()
    //           { lastRequestTime=millis(); this->handleWifiStatus(); });

    // server.on(F("/config"), HTTP_PATCH, [this]()
    //           { lastRequestTime=millis(); this->handleUpdateConfig(); });

    // server.on(F("/config"), HTTP_GET, [this]()
    //           { lastRequestTime=millis(); this->handleGetConfig(); });

    // server.on(F("/end"), HTTP_GET, [this]()
    //           { lastRequestTime=millis(); this->handleEnd(); });

    // server.on(F("/network/scan"), HTTP_OPTIONS, [this]()
    //           {
    //                    server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
    //                    server.sendHeader(F("Access-Control-Allow-Methods"), F("POST, GET, OPTIONS"));
    //                    server.sendHeader(F("Access-Control-Allow-Headers"), F("Content-Type"));
    //                    server.send(204); });

    // server.on(F("/network/connection"), HTTP_OPTIONS, [this]()
    //           {
    //                    server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
    //                    server.sendHeader(F("Access-Control-Allow-Methods"), F("POST, GET, OPTIONS"));
    //                    server.sendHeader(F("Access-Control-Allow-Headers"), F("Content-Type"));
    //                    server.send(204); });

    // server.on(F("/config"), HTTP_OPTIONS, [this]()
    //           {
    //                    server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
    //                    server.sendHeader(F("Access-Control-Allow-Methods"), F("PATCH, GET, OPTIONS"));
    //                    server.sendHeader(F("Access-Control-Allow-Headers"), F("Content-Type"));
    //                    server.send(204); });

    // server.on(F("/end"), HTTP_OPTIONS, [this]()
    //           {
    //                    server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
    //                    server.sendHeader(F("Access-Control-Allow-Methods"), F("GET, OPTIONS"));
    //                    server.sendHeader(F("Access-Control-Allow-Headers"), F("Content-Type"));
    //                    server.send(204); });

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
    server.send_P(200, "text/html", INDEX);
}

// /*======================================== handleScanStarted =============================================================*/

// void WebServer::handleScanStarted()
// {
//     logger.log(LOG_DEBUG, [&]() -> String128
//                {String128 buf; buf.add(F("(WebServer::handleScanStarted) Processing start scan networks request.")); return buf; });
//     const auto responce = api::SuccessResponse<api::ScanWifiNetworkStartedResponse>(200, std::move(api::ScanWifiNetworkStartedResponse(network->scanWifiNetworksAsync(), std::move(api::ModelBaseResponse()))));
//     String payload = api::serializeResponse(responce);
//     logger.log(LOG_DEBUG, [&]() -> String128
//                {String128 buf; buf.add(F("(WebServer::handleScanStarted) Response payload: ")); buf.add(payload.c_str()); return buf; });
//     server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
//     server.send(responce.getCode(), F("application/json"), payload);
// }

// /*======================================== handleScanResult =============================================================*/

// void WebServer::handleScanResult()
// {
//     logger.log(LOG_DEBUG, [&]() -> String128
//                {String128 buf; buf.add(F("(WebServer::handleScanResult) Processing scan result request.")); return buf; });
//     ScanState scanStatus = network->getStatusScan();
//     if (scanStatus == ScanState::COMPLETED)
//     {
//         const auto responce = api::SuccessResponse<api::ScanWifiNetworkResponse>(200, std::move(api::ScanWifiNetworkResponse(std::move(network->getScanWifiNetworksAsyncResults()), std::move(api::ModelBaseResponse()))));
//         String payload = api::serializeResponse(responce);
//         logger.log(LOG_DEBUG, [&]() -> String128
//                    {String128 buf; buf.add(F("(WebServer::handleScanResult) Response payload: ")); buf.add(payload.c_str()); return buf; });
//         server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
//         server.send(responce.getCode(), F("application/json"), payload);
//     }
//     else
//     {
//         const auto responce = api::SuccessResponse<api::GetScanStatusResponse>(200, std::move(api::GetScanStatusResponse(scanStatus, std::move(api::ModelBaseResponse()))));
//         String payload = api::serializeResponse(responce);
//         logger.log(LOG_DEBUG, [&]() -> String128
//                    {String128 buf; buf.add(F("(WebServer::handleScanResult) Response payload (scan not completed): ")); buf.add(payload.c_str()); return buf; });
//         server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
//         server.send(responce.getCode(), F("application/json"), payload);
//     }
// }

// /*========================================handleNetwork==========================================================*/

// void WebServer::handleConnect()
// {
//     logger.log(LOG_DEBUG, [&]() -> String128
//                {String128 buf; buf.add(F("(WebServer::handleConnect) Received network connect request.")); return buf; });

//     const String &body = server.arg(F("plain"));

//     logger.log(LOG_DEBUG, [&]() -> String128
//                {String128 buf; buf.add(F("(WebServer::handleConnect) Raw request body: ")); buf.add(body.c_str()); return buf; });

//     if (body.isEmpty())
//     {
//         logger.log(LOG_WARN, [&]() -> String128
//                    {String128 buf; buf.add(F("(WebServer::handleConnect) Empty request body.")); return buf; });
//         const auto responce = api::ErrorResponse(400, String32(F("Empty request body")));
//         String payload = api::serializeResponse(responce);
//         logger.log(LOG_DEBUG, [&]() -> String128
//                    {String128 buf; buf.add(F("(WebServer::handleConnect) Response payload (error empty body): ")); buf.add(payload.c_str()); return buf; });
//         server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
//         server.send(responce.getCode(), F("application/json"), payload);
//         return;
//     }

//     auto apiRequstPtr = api::parseRequest<api::ConnectWifiNetworkRequest>(body.c_str());
//     if (apiRequstPtr->isOk())
//     {
//         auto *successApiRequstPtr = static_cast<api::SuccessRequest<api::ConnectWifiNetworkRequest> *>(apiRequstPtr.get());
//         const bool status = network->attemptConnectionAsync(successApiRequstPtr->data.ssid.c_str(), successApiRequstPtr->data.password.c_str());
//         const auto responce = api::SuccessResponse<api::ConnectWifiNetworkStartedResponse>(200, std::move(api::ConnectWifiNetworkStartedResponse(status, std::move(successApiRequstPtr->data))));
//         String payload = api::serializeResponse(responce);
//         logger.log(LOG_DEBUG, [&]() -> String128
//                    {String128 buf; buf.add(F("(WebServer::handleConnect) Response payload (success): ")); buf.add(payload.c_str()); return buf; });
//         server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
//         server.send(responce.getCode(), F("application/json"), payload);
//     }
//     else
//     {
//         auto *errorApiRequstPtr = static_cast<api::ErrorRequest *>(apiRequstPtr.get());
//         const auto responce = api::ErrorResponse(400, errorApiRequstPtr->message);
//         String payload = api::serializeResponse(responce);
//         logger.log(LOG_DEBUG, [&]() -> String128
//                    {String128 buf; buf.add(F("(WebServer::handleConnect) Response payload (parse error): ")); buf.add(payload.c_str()); return buf; });
//         server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
//         server.send(responce.getCode(), F("application/json"), payload);
//     }
// }

// /*========================================handleStatusWifi==========================================================*/

// void WebServer::handleWifiStatus()
// {
//     logger.log(LOG_DEBUG, [&]() -> String128
//                {String128 buf; buf.add(F("(WebServer::handleWifiStatus) Received WiFi status request.")); return buf; });
//     const auto responce = api::SuccessResponse<api::GetWifiStatusResponse>(200, std::move(api::GetWifiStatusResponse(std::move(network->getStatusWifi()), std::move(api::ModelBaseResponse()))));
//     String payload = api::serializeResponse(responce);
//     logger.log(LOG_DEBUG, [&]() -> String128
//                {String128 buf; buf.add(F("(WebServer::handleWifiStatus) Response payload: ")); buf.add(payload.c_str()); return buf; });
//     server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
//     server.send(responce.getCode(), F("application/json"), payload);
// }

// /*========================================handleGetConfig==========================================================*/

// void WebServer::handleGetConfig()
// {
//     logger.log(LOG_DEBUG, [&]() -> String128
//                {String128 buf; buf.add(F("(WebServer::handleGetConfig) Received get config request.")); return buf; });
//     const auto networkConfig = config->getNetworkConfig();
//     const auto deviceLedConfig = config->getDeviceLedConfig();
//     const auto telegramBotConfig = config->getTelegramBotConfig();

//     api::GetConfigResponse modelResponse(networkConfig.apSsid, networkConfig.apPassword, networkConfig.mdnsName, networkConfig.wifiConnectionTimeout,
//                                     deviceLedConfig.countLed, deviceLedConfig.deviceName,
//                                     telegramBotConfig.token, telegramBotConfig.limitMessage, telegramBotConfig.periodUpdate,
//                                     std::move(api::ModelBaseResponse()));

//     const auto responce = api::SuccessResponse<api::GetConfigResponse>(200, std::move(modelResponse));
//     String payload = api::serializeResponse(responce);
//     logger.log(LOG_DEBUG, [&]() -> String128
//                {String128 buf; buf.add(F("(WebServer::handleGetConfig) Response payload: ")); buf.add(payload.c_str()); return buf; });
//     server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
//     server.send(responce.getCode(), F("application/json"), payload);
// }

// /*========================================handleUpdateConfig==========================================================*/

// void WebServer::handleUpdateConfig()
// {
//     logger.log(LOG_DEBUG, [&]() -> String128
//                {String128 buf; buf.add(F("(WebServer::handleUpdateConfig) Received update config request.")); return buf; });

//     const String &body = server.arg(F("plain"));

//     logger.log(LOG_DEBUG, [&]() -> String128
//                {String128 buf; buf.add(F("(WebServer::handleUpdateConfig) Raw request body: ")); buf.add(body.c_str()); return buf; });

//     if (body.isEmpty())
//     {
//         logger.log(LOG_WARN, [&]() -> String128
//                    {String128 buf; buf.add(F("(WebServer::handleUpdateConfig) Empty request body.")); return buf; });
//         const auto responce = api::ErrorResponse(400, String32(F("Empty request body")));
//         String payload = api::serializeResponse(responce);
//         logger.log(LOG_DEBUG, [&]() -> String128
//                    {String128 buf; buf.add(F("(WebServer::handleUpdateConfig) Response payload (error empty body): ")); buf.add(payload.c_str()); return buf; });
//         server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
//         server.send(responce.getCode(), F("application/json"), payload);
//         return;
//     }

//     auto apiRequstPtr = api::parseRequest<api::UpdateConfigRequest>(body.c_str());
//     if (apiRequstPtr->isOk())
//     {
//         auto *successApiRequstPtr = static_cast<api::SuccessRequest<api::UpdateConfigRequest> *>(apiRequstPtr.get());
//         auto& networkConfig = config->getNetworkConfig();
//         networkConfig.apSsid = successApiRequstPtr->data.apSsid.value_or(networkConfig.apSsid);
//         networkConfig.apPassword = successApiRequstPtr->data.apPassword.value_or(networkConfig.apPassword);
//         networkConfig.mdnsName = successApiRequstPtr->data.mdnsName.value_or(networkConfig.mdnsName);
//         networkConfig.wifiConnectionTimeout = successApiRequstPtr->data.wifiConnectionTimeout.value_or(networkConfig.wifiConnectionTimeout);

//         auto& deviceLedConfig = config->getDeviceLedConfig();
//         deviceLedConfig.countLed = successApiRequstPtr->data.countLed.value_or(deviceLedConfig.countLed);
//         deviceLedConfig.deviceName = successApiRequstPtr->data.deviceName.value_or(deviceLedConfig.deviceName);

//         auto& telegramBotConfig = config->getTelegramBotConfig();
//         telegramBotConfig.token = successApiRequstPtr->data.token.value_or(telegramBotConfig.token);
//         telegramBotConfig.limitMessage = successApiRequstPtr->data.limitMessage.value_or(telegramBotConfig.limitMessage);
//         telegramBotConfig.periodUpdate = successApiRequstPtr->data.periodUpdate.value_or(telegramBotConfig.periodUpdate);

//         logger.log(LOG_INFO, [&]() -> String128
//                {String128 buf; buf.add(F("(WebServer::handleUpdateConfig) Configuration updated successfully.")); return buf; });
//         server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
//         server.send(204);
//     }
//     else
//     {
//         auto *errorApiRequstPtr = static_cast<api::ErrorRequest *>(apiRequstPtr.get());
//         const auto responce = api::ErrorResponse(400, errorApiRequstPtr->message);
//         String payload = api::serializeResponse(responce);
//         logger.log(LOG_DEBUG, [&]() -> String128
//                    {String128 buf; buf.add(F("(WebServer::handleUpdateConfig) Response payload (parse error): ")); buf.add(payload.c_str()); return buf; });
//         server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
//         server.send(responce.getCode(), F("application/json"), payload);
//     }
// }

// /*========================================handleEnd============================================================*/

// void WebServer::handleEnd()
// {
//     logger.log(LOG_INFO, [&]() -> String128
//                {String128 buf; buf.add(F("(WebServer::handleEnd) Request accepted, stopping server.")); return buf; });
//     server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
//     server.send(204);
//     stop();
// }

// /*========================================handleNotFound========================================================*/

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
