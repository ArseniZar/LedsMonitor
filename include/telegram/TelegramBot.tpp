#pragma once
#include "TelegramBot.h"

template <typename Request, typename Response = void>
void TelegramBot::registerCommand(const char *command, std::function<Response(Request &)> handler)
{

    static_assert(std::is_base_of<api::telegram::ModelBaseRequest, Request>::value, "Request must be void or inherit from api::telegram::ModelBaseRequest");
    static_assert(std::is_void<Response>::value || std::is_base_of<api::Response, Response>::value, "Response must be void or inherit from api::telegram::ModelBaseResponse");

    handlers[command] = [handler, this](fb::Update &request)
    {
        logger.log(LOG_DEBUG, [&]() -> String128
                   { String128 buf; buf.add(F("(TelegramBot::registerCommand) Received request with body.")); return buf; });

        String body = String(request.message().text().c_str());
        logger.log(LOG_DEBUG, [&]() -> String256
                   { String256 buf; buf.add(F("(TelegramBot::registerCommand) Raw request body: ")); buf.add(body.c_str()); return buf; });

        if (body.isEmpty())
        {
            logger.log(LOG_WARN, [&]() -> String128
                       { String128 buf; buf.add(F("(TelegramBot::registerCommand) Empty request body.")); return buf; });
            const auto response = api::ErrorResponse(400, String32(F("Empty request body")));
            gson::Str payload = api::serializeResponse(response);
            logger.log(LOG_DEBUG, [&]() -> String256
                       { String256 buf; buf.add(F("(TelegramBot::registerCommand) Response payload (error empty body): ")); buf.add(Text(payload).c_str()); return buf; });
            bot.sendMessage(fb::Message(std::move(payload), request.message().chat().id()));
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
                                   logger.log(LOG_DEBUG, [&]() -> String256
                                              { String256 buf; buf.add(F("(TelegramBot::registerCommand) Response payload (success): ")); buf.add(Text(payload).c_str()); return buf; });
                                   bot.sendMessage(fb::Message(std::move(payload), request.message().chat().id())); }, handler(successApiRequestPtr->data));
                }
                else
                {
                    const auto &response = handler(successApiRequestPtr->data);
                    gson::Str payload = api::serializeResponse(response);
                    logger.log(LOG_DEBUG, [&]() -> String256
                               { String256 buf; buf.add(F("(TelegramBot::registerCommand) Response payload (success): ")); buf.add(Text(payload).c_str()); return buf; });
                    bot.sendMessage(fb::Message(std::move(payload), request.message().chat().id()));
                }
            }
            else
            {
                handler(successApiRequestPtr->data);
                logger.log(LOG_INFO, [&]() -> String128
                           { String128 buf; buf.add(F("(TelegramBot::registerCommand) Request handled successfully.")); return buf; });
            }
        }
        else
        {
            auto *errorApiRequstPtr = static_cast<api::ErrorRequest *>(apiRequestPtr.get());
            const auto response = api::ErrorResponse(400, errorApiRequstPtr->message);
            gson::Str payload = api::serializeResponse(response);
            logger.log(LOG_DEBUG, [&]() -> String256
                       { String256 buf; buf.add(F("(TelegramBot::registerCommand) Response payload (parse error): ")); buf.add(Text(payload).c_str()); return buf; });
            bot.sendMessage(fb::Message(std::move(payload), request.message().chat().id()));
        }
    };
}

template <typename Request = void, typename Response = void>
void TelegramBot::registerCommand(const char *command, std::function<Response()> handler)
{
    handlers[command] = [handler, this](fb::Update &request)
    {
        if constexpr (!std::is_void<Response>::value)
        {
            if constexpr (is_variant<Response>::value)
            {
                std::visit([&](auto &&response)
                           {
                               String payload = api::serializeResponse(response);
                               logger.log(LOG_DEBUG, [&]() -> String256
                                          { String256 buf; buf.add(F("(TelegramBot::registerCommand) Response payload (success): ")); buf.add(Text(payload).c_str()); return buf; });
                               bot.sendMessage(fb::Message(std::move(payload), request.message().chat().id())); },
                           handler());
            }
            else
            {
                const auto &response = handler();
                gson::Str payload = api::serializeResponse(response);
                logger.log(LOG_DEBUG, [&]() -> String256
                           { String256 buf; buf.add(F("(TelegramBot::registerCommand) Response payload (success): ")); buf.add(Text(payload).c_str()); return buf; });
                bot.sendMessage(fb::Message(std::move(payload), request.message().chat().id()));
            }
        }
        else
        {
            handler();
            logger.log(LOG_INFO, [&]() -> String128
                       { String128 buf; buf.add(F("(TelegramBot::registerCommand) Request handled successfully.")); return buf; });
        }
    };
}