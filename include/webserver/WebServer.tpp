#pragma once
#include "WebServer.h"

template <typename Request, typename Response = void>
void WebServer::registerEndpoint(const char *uri, HTTPMethod httpMethod, EndpointType type, std::function<Response(Request &)> handler)
{
    static_assert(std::is_base_of<api::webserver::ModelBaseRequest, Request>::value, "Request must be void or inherit from api::webserver::ModelBaseRequest");
    static_assert(std::is_void<Response>::value || std::is_base_of<api::Response, Response>::value, "Response must be void or inherit from api::webserver::ModelBaseResponse");

    Endpoint endpoint(uri, httpMethod);
    endpoint.setType(type);

    handlers[endpoint] = [handler, this](ghttp::ServerBase::Request request)
    {
        logger.log(LOG_DEBUG, [&]() -> String128
                   { String128 buf; buf.add(F("(WebServer::registerEndpoint) Received request with body.")); return buf; });

        String body = request.body().readString();
        logger.log(LOG_DEBUG, [&]() -> String256
                   { String256 buf; buf.add(F("(WebServer::registerEndpoint) Raw request body: ")); buf.add(body.c_str()); return buf; });

        if (body.isEmpty())
        {
            logger.log(LOG_WARN, [&]() -> String128
                       { String128 buf; buf.add(F("(WebServer::registerEndpoint) Empty request body.")); return buf; });
            const auto response = api::ErrorResponse(400, String32(F("Empty request body")));
            gson::Str payload = api::serializeResponse(response);
            logger.log(LOG_DEBUG, [&]() -> String256
                       { String256 buf; buf.add(F("(WebServer::registerEndpoint) Response payload (error empty body): ")); buf.add(Text(payload).c_str()); return buf; });

            server.sendSingle(payload, response.getCode(), F("application/json"));
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
                                   gson::Str payload = api::serializeResponse(response);
                                   logger.log(LOG_DEBUG, [&]() -> String256
                                              { String256 buf; buf.add(F("(WebServer::registerEndpoint) Response payload (success): ")); buf.add(Text(payload).c_str()); return buf; });
                                   
                                   server.sendSingle(payload, response.getCode(), F("application/json")); },
                               handler(successApiRequestPtr->data));
                }
                else
                {
                    const auto &response = handler(successApiRequestPtr->data);
                    gson::Str payload = api::serializeResponse(response);
                    logger.log(LOG_DEBUG, [&]() -> String256
                               { String256 buf; buf.add(F("(WebServer::registerEndpoint) Response payload (success): ")); buf.add(Text(payload).c_str()); return buf; });
                    server.sendSingle(payload, response.getCode(), F("application/json"));
                }
            }
            else
            {
                handler(successApiRequestPtr->data);
                logger.log(LOG_INFO, [&]() -> String128
                           { String128 buf; buf.add(F("(WebServer::registerEndpoint) Request handled successfully.")); return buf; });
                server.send(204);
            }
        }
        else
        {
            auto *errorApiRequstPtr = static_cast<api::ErrorRequest *>(apiRequestPtr.get());
            const auto response = api::ErrorResponse(400, errorApiRequstPtr->message);
            gson::Str payload = api::serializeResponse(response);
            logger.log(LOG_DEBUG, [&]() -> String256
                       { String256 buf; buf.add(F("(WebServer::registerEndpoint) Response payload (parse error): ")); buf.add(Text(payload).c_str()); return buf; });
            server.sendSingle(payload, response.getCode(), F("application/json"));
        }
    };
    // routeMasks[uri] |= (128 | (1 << static_cast<uint8_t>(httpMethod)));
    // handlers[{uri, Method::HTTP_OPTIONS}] = [this, uri](ghttp::ServerBase::Request request)
    // {
    //     handleOptions(uri);
    // };
}

template <typename Request = void, typename Response = void>
void WebServer::registerEndpoint(const char *uri, HTTPMethod httpMethod, EndpointType type, std::function<Response()> handler)
{
    Endpoint endpoint(uri, httpMethod);
    endpoint.setType(type);

    handlers[endpoint] = [handler, this](ghttp::ServerBase::Request request)
    {
        if constexpr (!std::is_void<Response>::value)
        {
            if constexpr (is_variant<Response>::value)
            {
                std::visit([&](auto &&response)
                           {
                               gson::Str payload = api::serializeResponse(response);
                               logger.log(LOG_DEBUG, [&]() -> String256
                                          { String256 buf; buf.add(F("(WebServer::registerEndpoint) Response payload (success): ")); buf.add(Text(payload).c_str()); return buf; });
                               server.sendSingle(payload, response.getCode(), F("application/json")); },
                           handler());
            }
            else
            {
                const auto &response = handler();
                gson::Str payload = api::serializeResponse(response);
                logger.log(LOG_DEBUG, [&]() -> String256
                           { String256 buf; buf.add(F("(WebServer::registerEndpoint) Response payload (success): ")); buf.add(Text(payload).c_str()); return buf; });
                server.sendSingle(payload, response.getCode(), F("application/json"));
            }
        }
        else
        {
            handler();
            logger.log(LOG_INFO, [&]() -> String128
                       { String128 buf; buf.add(F("(WebServer::registerEndpoint) Request handled successfully.")); return buf; });
            server.send(204);
        }
    };

    // routeMasks[uri] |= (128 | (1 << static_cast<uint8_t>(httpMethod)));
    // handlers[{uri, Method::HTTP_OPTIONS}] = [this, uri](ghttp::ServerBase::Request request)
    // {
    //     handleOptions(uri);
    // };
}