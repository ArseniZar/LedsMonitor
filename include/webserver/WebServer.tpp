#pragma once
#include "WebServer.h"

template <typename Request, typename Response = void>
void WebServer::registerRoute(const char *uri, HTTPMethod httpMethod, std::function<Response(Request &)> handler)
{
    static_assert(std::is_base_of<api::webserver::ModelBaseRequest, Request>::value, "Request must be void or inherit from api::webserver::ModelBaseRequest");
    static_assert(std::is_void<Response>::value || std::is_base_of<api::Response, Response>::value, "Response must be void or inherit from api::webserver::ModelBaseResponse");
    
    handlers[{uri, httpMethod}] = [handler, this](ghttp::ServerBase::Request request)
    {
        logger.log(LOG_DEBUG, [&]() -> String128
                   { String128 buf; buf.add(F("(WebServer::registerRoute) Received request with body.")); return buf; });

        String body = request.body().readString();
        logger.log(LOG_DEBUG, [&]() -> String128
                   { String128 buf; buf.add(F("(WebServer::registerRoute) Raw request body: ")); buf.add(body.c_str()); return buf; });

        if (body.isEmpty())
        {
            logger.log(LOG_WARN, [&]() -> String128
                       { String128 buf; buf.add(F("(WebServer::registerRoute) Empty request body.")); return buf; });
            const auto response = api::ErrorResponse(400, String32(F("Empty request body")));
            String payload = api::serializeResponse(response);
            logger.log(LOG_DEBUG, [&]() -> String128
                       { String128 buf; buf.add(F("(WebServer::registerRoute) Response payload (error empty body): ")); buf.add(payload.c_str()); return buf; });

            server.send(payload, response.getCode(), F("application/json"));
            return;
        }

        auto apiRequestPtr = api::parseRequest<Request>(body.c_str());
        if (apiRequestPtr->isOk())
        {
            auto *successApiRequestPtr = static_cast<api::SuccessRequest<Request> *>(apiRequestPtr.get());
            if constexpr (!std::is_void<Response>::value)
            {
                if constexpr (is_variant<Response>::value)
                {
                    std::visit([&](auto &&response)
                               {
                                   String payload = api::serializeResponse(response);
                                   logger.log(LOG_DEBUG, [&]() -> String128
                                              { String128 buf; buf.add(F("(WebServer::registerRoute) Response payload (success): ")); buf.add(payload.c_str()); return buf; });
                                   
                                   server.send(payload, response.getCode(), F("application/json")); },
                               handler(successApiRequestPtr->data));
                }
                else
                {
                    const auto &response = handler(successApiRequestPtr->data);
                    String payload = api::serializeResponse(response);
                    logger.log(LOG_DEBUG, [&]() -> String128
                               { String128 buf; buf.add(F("(WebServer::registerRoute) Response payload (success): ")); buf.add(payload.c_str()); return buf; });
                    server.send(payload, response.getCode(), F("application/json"));
                    // server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
                    // server.send(response.getCode(), F("application/json"), payload);
                }
            }
            else
            {
                handler(successApiRequestPtr->data);
                logger.log(LOG_INFO, [&]() -> String128
                           { String128 buf; buf.add(F("(WebServer::registerRoute) Request handled successfully.")); return buf; });
                server.handle();
            }
        }
        else
        {
            auto *errorApiRequstPtr = static_cast<api::ErrorRequest *>(apiRequestPtr.get());
            const auto response = api::ErrorResponse(400, errorApiRequstPtr->message);
            String payload = api::serializeResponse(response);
            logger.log(LOG_DEBUG, [&]() -> String128
                       { String128 buf; buf.add(F("(WebServer::registerRoute) Response payload (parse error): ")); buf.add(payload.c_str()); return buf; });
            server.send(payload, response.getCode(), F("application/json"));
        }
    };
    // routeMasks[uri] |= (128 | (1 << static_cast<uint8_t>(httpMethod)));
    // handlers[{uri, Method::HTTP_OPTIONS}] = [this, uri](ghttp::ServerBase::Request request)
    // {
    //     handleOptions(uri);
    // };
}

template <typename Request = void, typename Response = void>
void WebServer::registerRoute(const char *uri, HTTPMethod httpMethod, std::function<Response()> handler)
{
    handlers[{uri, httpMethod}] = [handler, this](ghttp::ServerBase::Request request)
    {
        if constexpr (!std::is_void<Response>::value)
        {
            if constexpr (is_variant<Response>::value)
            {
                std::visit([&](auto &&response)
                           {
                               String payload = api::serializeResponse(response);
                               logger.log(LOG_DEBUG, [&]() -> String128
                                          { String128 buf; buf.add(F("(WebServer::registerRoute) Response payload (success): ")); buf.add(payload.c_str()); return buf; });
                               server.send(payload, response.getCode(), F("application/json")); },
                           handler());
            }
            else
            {
                const auto &response = handler();
                String payload = api::serializeResponse(response);
                logger.log(LOG_DEBUG, [&]() -> String128
                           { String128 buf; buf.add(F("(WebServer::registerRoute) Response payload (success): ")); buf.add(payload.c_str()); return buf; });
                server.send(payload, response.getCode(), F("application/json"));
            }
        }
        else
        {
            handler();
            logger.log(LOG_INFO, [&]() -> String128
                       { String128 buf; buf.add(F("(WebServer::registerRoute) Request handled successfully.")); return buf; });
            server.handle();
        }
    };

    // routeMasks[uri] |= (128 | (1 << static_cast<uint8_t>(httpMethod)));
    // handlers[{uri, Method::HTTP_OPTIONS}] = [this, uri](ghttp::ServerBase::Request request)
    // {
    //     handleOptions(uri);
    // };
}