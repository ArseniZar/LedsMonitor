#include "WebServer.h"

namespace espweb
{
    WebServer::WebServer(Logger &logger, int port) : logger(logger), server(port)
    {
        server.keepAlive(true);
        MDNS.addService("http", "tcp", port);
    }
    
    bool WebServer::begin(std::function<std::vector<Network>()> getNetwork,
                          std::function<bool(const String &, const String &)> attemptConnection)
    {
        server.on("/", HTTP_GET, [this]()
                  { this->handleRoot(); });

        server.on("/scan", HTTP_GET, [this, getNetwork]()
                  { this->handleScan(getNetwork); });

        server.on("/network", HTTP_POST, [this, attemptConnection]()
                  { this->handleNetwork(attemptConnection); });

        server.on("/end", HTTP_GET, [this]()
                  { this->handleEnd(); });

        server.onNotFound([this]()
                          { this->handleNotFound(); });

        server.begin();
        serverRunning = true;
        logger.log("(WebServer::begin) Server started and running.", LOG_INFO);

        return true;
    }

    void WebServer::handleRoot()
    {
        logger.log("(WebServer::handleRoot) Handling root page request.", LOG_DEBUG);
        server.send_P(200, "text/html", index);
    }
    void WebServer::handleScan(std::function<std::vector<Network>()> getNetwork)
    {
        logger.log("(WebServer::handleScan) Processing scan networks request.", LOG_DEBUG);
        api::ApiSuccessResponseList<Network> response;
        response.result = getNetwork();
        logger.log("(WebServer::handleScan) Networks scanned: " + String(response.result.size()), LOG_DEBUG);
        server.send(200, "application/json", api::serializeApiResponse(response));
    }

    void WebServer::handleNetwork(std::function<bool(const String &, const String &)> attemptConnection)
    {
        const String &body = server.arg("plain");
        logger.log("(WebServer::handleNetwork) Received network connect request.", LOG_DEBUG);
        std::unique_ptr<api::ApiResponse> response_ptr;

        if (body.isEmpty())
        {
            logger.log("(WebServer::handleNetwork) Empty request body.", LOG_WARN);
            response_ptr = std::make_unique<api::ApiErrorResponse>(400, "Empty request body");
        }
        else
        {
            auto parse_ptr = api::parseApiRequest<Network>(body);
            if (parse_ptr->isOk())
            {
                auto *success_parse_ptr = static_cast<api::ApiSuccessRequestSingle<Network> *>(parse_ptr.get());
                logger.log("(WebServer::handleNetwork) Successfully parsed", LOG_DEBUG);
                bool connected = attemptConnection(success_parse_ptr->data.ssid, success_parse_ptr->data.password);
                api::ApiSuccessResponseSingle<Network> response;
                response.ok = connected;
                response.result = success_parse_ptr->data;
                response_ptr = std::make_unique<api::ApiSuccessResponseSingle<Network>>(response);
            }
            else
            {
                auto *error_ptr = static_cast<api::ApiErrorRequest *>(parse_ptr.get());
                logger.log("(WebServer::handleNetwork) Failed to parse request: " + error_ptr->message, LOG_WARN);
                response_ptr = std::make_unique<api::ApiErrorResponse>(400, error_ptr->message);
            }
        }

        server.send(response_ptr->getCode(), "application/json", api::serializeApiResponse<Network>(*response_ptr));
    }

    void WebServer::handleEnd()
    {
        logger.log("(WebServer::handleEnd) Request accepted, stopping server.", LOG_INFO);
        server.send(200, "text/plain", "Request accepted");
        stop();
    }

    void WebServer::handleNotFound()
    {
        logger.log("(WebServer::handleNotFound) Resource not found.", LOG_WARN);
        api::ApiErrorResponse error(404, "Resource not found");
        server.send(error.getCode(), "application/json", api::serializeApiResponse(error));
    }

    void WebServer::handleClient()
    {
        server.handleClient();
    }
    void WebServer::stop()
    {
        server.stop();
        logger.log("(WebServer::stop) Stop server.", LOG_INFO);
        serverRunning = false;
    }

    bool WebServer::isRunning() const
    {
        return serverRunning;
    }
}
