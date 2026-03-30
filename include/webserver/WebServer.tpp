#pragma once
#include "WebServer.h"

template <typename T>
struct is_variant : std::false_type {};

template <typename... Args>
struct is_variant<std::variant<Args...>> : std::true_type {};

template <typename Request, typename Response = void>
void WebServer::registerRoutes(const char *uri, HTTPMethod httpMethod, std::function<Response(Request &)> handler)
{
    static_assert(std::is_base_of<api::ModelBaseRequest, Request>::value, "Request must be void or inherit from api::ModelBaseRequest");
    static_assert(std::is_void<Response>::value || std::is_base_of<api::ModelBaseResponse, Response>::value, "Response must be void or inherit from api::ModelBaseResponse");

    server.on(uri, httpMethod, [handler, this]()
              {
        lastRequestTime = millis();
        logger.log(LOG_DEBUG, [&]() -> String128
                   { String128 buf; buf.add(F("(WebServer::registerRoutes) Received request with body.")); return buf; });

        const String &body = server.arg(F("plain"));
        logger.log(LOG_DEBUG, [&]() -> String128
                   { String128 buf; buf.add(F("(WebServer::registerRoutes) Raw request body: ")); buf.add(body.c_str()); return buf; });

        if (body.isEmpty())
        {
            logger.log(LOG_WARN, [&]() -> String128
                       { String128 buf; buf.add(F("(WebServer::registerRoutes) Empty request body.")); return buf; });
            const auto response = api::ErrorResponse(400, String32(F("Empty request body")));
            String payload = api::serializeResponse(response);
            logger.log(LOG_DEBUG, [&]() -> String128
                       { String128 buf; buf.add(F("(WebServer::registerRoutes) Response payload (error empty body): ")); buf.add(payload.c_str()); return buf; });
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
                    std::visit([&](auto &&arg)
                    {
                        using R = std::decay_t<decltype(arg)>;
                        const auto response = api::SuccessResponse<R>(200, std::forward<decltype(arg)>(arg)); 
                        String payload = api::serializeResponse(response);
                        logger.log(LOG_DEBUG, [&]() -> String128
                                { String128 buf; buf.add(F("(WebServer::registerRoutes) Response payload (success): ")); buf.add(payload.c_str()); return buf; });
                        server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
                        server.send(response.getCode(), F("application/json"), payload);
                    },handler(successApiRequstPtr->data));
                }
                else
                {
                    const auto response = api::SuccessResponse<Response>(200, handler(successApiRequstPtr->data));
                    String payload = api::serializeResponse(response);
                    logger.log(LOG_DEBUG, [&]() -> String128
                               { String128 buf; buf.add(F("(WebServer::registerRoutes) Response payload (success): ")); buf.add(payload.c_str()); return buf; });
                    server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
                    server.send(response.getCode(), F("application/json"), payload);
                }
            }    
            else
            {
                handler(successApiRequstPtr->data);
                logger.log(LOG_INFO, [&]() -> String128
                            { String128 buf; buf.add(F("(WebServer::registerRoutes) Request handled successfully.")); return buf; });
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
                        { String128 buf; buf.add(F("(WebServer::registerRoutes) Response payload (parse error): ")); buf.add(payload.c_str()); return buf; });
            server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
            server.send(response.getCode(), F("application/json"), payload);
        } });

    routeMasks[uri] |= (128 | (1 << static_cast<uint8_t>(httpMethod)));
    server.on(uri, HTTP_OPTIONS, [uri, this]()
    {   
        String64 buf;
        const u_int8_t masks = routeMasks[uri];
        for (uint8_t i = 0; i < 8; i++) {
            if (masks & (1 << i)) {
                if (buf.length() > 0) { 
                    buf.add(F(", ")); 
                }

                switch (i) {
                    case HTTP_GET:     buf.add(F("GET"));     break;
                    case HTTP_POST:    buf.add(F("POST"));    break;
                    case HTTP_PUT:     buf.add(F("PUT"));     break;
                    case HTTP_PATCH:   buf.add(F("PATCH"));   break;
                    case HTTP_DELETE:  buf.add(F("DELETE"));  break;
                    case HTTP_HEAD:    buf.add(F("HEAD"));    break;
                    case HTTP_OPTIONS: buf.add(F("OPTIONS")); break;
                    default:           buf.add(F("ANY"));     break;
                }
            }
        }

        server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
        server.sendHeader(F("Access-Control-Allow-Methods"), buf.c_str());
        server.sendHeader(F("Access-Control-Allow-Headers"), F("Content-Type"));
        server.send(204); 
    });
}

template <typename Request = void, typename Response = void>
void WebServer::registerRoutes(const char *uri, HTTPMethod httpMethod, std::function<Response()> handler)
{
    server.on(uri, httpMethod, [handler, this]()
    {
        lastRequestTime=millis();
        if constexpr (!std::is_void<Response>::value)
        {
            if constexpr (is_variant<Response>::value)
            {
                std::visit([&](auto &&arg)
                {
                    using R = std::decay_t<decltype(arg)>;
                    const auto response = api::SuccessResponse<R>(200, std::forward<decltype(arg)>(arg)); 
                    String payload = api::serializeResponse(response);
                    logger.log(LOG_DEBUG, [&]() -> String128
                                { String128 buf; buf.add(F("(WebServer::registerRoutes) Response payload (success): ")); buf.add(payload.c_str()); return buf; });
                    server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
                    server.send(response.getCode(), F("application/json"), payload);
                },handler());
            }
            else
            {
                const auto response = api::SuccessResponse<Response>(200, handler());
                String payload = api::serializeResponse(response);
                logger.log(LOG_DEBUG, [&]() -> String128
                            { String128 buf; buf.add(F("(WebServer::registerRoutes) Response payload (success): ")); buf.add(payload.c_str()); return buf; });
                server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
                server.send(response.getCode(), F("application/json"), payload);
            }
        }
        else
        {
            handler();
            logger.log(LOG_INFO, [&]() -> String128
                    { String128 buf; buf.add(F("(WebServer::registerRoutes) Request handled successfully.")); return buf; });
            server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
            server.send(204);
        } });

    routeMasks[uri] |= (128 | (1 << static_cast<uint8_t>(httpMethod)));
    server.on(uri, HTTP_OPTIONS, [uri, this]()
              {   
        String64 buf;
        const u_int8_t masks = routeMasks[uri];
        for (uint8_t i = 0; i < 8; i++) {
            if (masks & (1 << i)) {
                if (buf.length() > 0) { 
                    buf.add(F(", ")); 
                }

                switch (i) {
                    case HTTP_GET:     buf.add(F("GET"));     break;
                    case HTTP_POST:    buf.add(F("POST"));    break;
                    case HTTP_PUT:     buf.add(F("PUT"));     break;
                    case HTTP_PATCH:   buf.add(F("PATCH"));   break;
                    case HTTP_DELETE:  buf.add(F("DELETE"));  break;
                    case HTTP_HEAD:    buf.add(F("HEAD"));    break;
                    case HTTP_OPTIONS: buf.add(F("OPTIONS")); break;
                    default:           buf.add(F("ANY"));     break;
                }
            }
        }
    
        server.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
        server.sendHeader(F("Access-Control-Allow-Methods"), buf.c_str());
        server.sendHeader(F("Access-Control-Allow-Headers"), F("Content-Type"));
        server.send(204); });
}