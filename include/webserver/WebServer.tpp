#pragma once
#include "WebServer.h"

template <typename Request, typename Response = void>
void WebServer::registerRoute(const char *uri, HTTPMethod httpMethod, std::function<Response(Request &)> handler)
{
    static_assert(std::is_base_of<api::ModelBaseRequest, Request>::value, "Request must be void or inherit from api::ModelBaseRequest");
    static_assert(std::is_void<Response>::value || std::is_base_of<api::Response, Response>::value, "Response must be void or inherit from api::ModelBaseResponse");

    server.on(uri, httpMethod, [handler, this]()
              {
        lastRequestTime = millis();
        logger.log(LOG_DEBUG, [&]() -> String128
                   { String128 buf; buf.add(F("(WebServer::registerRoute) Received request with body.")); return buf; });

        const String &body = server.arg(F("plain"));
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
            server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
            server.send(response.getCode(), F("application/json"), payload);
            return;
        }

        auto apiRequstPtr = api::parseRequest<Request>(body.c_str());
        if (apiRequstPtr->isOk())
        {
            auto *successApiRequstPtr = static_cast<api::SuccessRequest<Request> *>(apiRequstPtr.get());
            if constexpr (!std::is_void<Response>::value)
            {
                if constexpr (is_variant<Response>::value)
                {   
                    std::visit([&](auto &&response) // TODO: может заменить & на this
                    {
                        String payload = api::serializeResponse(response);
                        logger.log(LOG_DEBUG, [&]() -> String128
                                { String128 buf; buf.add(F("(WebServer::registerRoute) Response payload (success): ")); buf.add(payload.c_str()); return buf; });
                        server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
                        server.send(response.getCode(), F("application/json"), payload);
                    },handler(successApiRequstPtr->data));
                }
                else
                {
                    const auto &response = handler(successApiRequstPtr->data);
                    String payload = api::serializeResponse(response);
                    logger.log(LOG_DEBUG, [&]() -> String128
                               { String128 buf; buf.add(F("(WebServer::registerRoute) Response payload (success): ")); buf.add(payload.c_str()); return buf; });
                    server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
                    server.send(response.getCode(), F("application/json"), payload);
                }
            }    
            else
            {
                handler(successApiRequstPtr->data);
                logger.log(LOG_INFO, [&]() -> String128
                            { String128 buf; buf.add(F("(WebServer::registerRoute) Request handled successfully.")); return buf; });
                server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
                server.send(204);
            }
        }    
        else
        {
            auto *errorApiRequstPtr = static_cast<api::ErrorRequest *>(apiRequstPtr.get());
            const auto response = api::ErrorResponse(400, errorApiRequstPtr->message);
            String payload = api::serializeResponse(response);
            logger.log(LOG_DEBUG, [&]() -> String128
                        { String128 buf; buf.add(F("(WebServer::registerRoute) Response payload (parse error): ")); buf.add(payload.c_str()); return buf; });
            server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
            server.send(response.getCode(), F("application/json"), payload);
        } });

    routeMasks[uri] |= (128 | (1 << static_cast<uint8_t>(httpMethod)));
    server.on(uri, HTTP_OPTIONS, [uri, this]()
              {  handleOptions(uri); }); 
        
}

template <typename Request = void, typename Response = void>
void WebServer::registerRoute(const char *uri, HTTPMethod httpMethod, std::function<Response()> handler)
{
    server.on(uri, httpMethod, [handler, this]()
              {
        lastRequestTime=millis();
        if constexpr (!std::is_void<Response>::value)
        {
            if constexpr (is_variant<Response>::value)
            {   
                std::visit([&](auto &&response)
                {
                    String payload = api::serializeResponse(response);
                    logger.log(LOG_DEBUG, [&]() -> String128
                                { String128 buf; buf.add(F("(WebServer::registerRoute) Response payload (success): ")); buf.add(payload.c_str()); return buf; });
                    server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
                    server.send(response.getCode(), F("application/json"), payload);
                },handler());
            }
            else
            {
                const auto &response = handler();
                String payload = api::serializeResponse(response);
                logger.log(LOG_DEBUG, [&]() -> String128
                            { String128 buf; buf.add(F("(WebServer::registerRoute) Response payload (success): ")); buf.add(payload.c_str()); return buf; });
                server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
                server.send(response.getCode(), F("application/json"), payload);
            }
        }
        else
        {
            handler();
            logger.log(LOG_INFO, [&]() -> String128
                    { String128 buf; buf.add(F("(WebServer::registerRoute) Request handled successfully.")); return buf; });
            server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
            server.send(204);
        } });

    routeMasks[uri] |= (128 | (1 << static_cast<uint8_t>(httpMethod)));
    server.on(uri, HTTP_OPTIONS, [uri, this]()
              {  handleOptions(uri); }); 
}