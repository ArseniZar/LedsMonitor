#pragma once
#include "TelegramBot.h"

template <typename T, typename E = void>
void TelegramBot::registerCommand(const String &command, std::function<E(T &)> handler)
{
    using namespace telegram;
    static_assert(std::is_base_of<ModelBaseRequest, T>::value, "T must inherit from ModelBaseRequest");
    static_assert(std::is_void<E>::value || std::is_base_of<ModelBaseResponse, E>::value, "E must be void or inherit from ModelBaseResponse");
    handlers[command] = [handler, this](fb::Update &u)
    {
        auto parsePtr = parseTelegramRequest<T>(u.message().text());
        if (!parsePtr->isOk())
        {
            auto *errorParsePtr = static_cast<TelegramErrorRequest *>(parsePtr.get());
            logger.log("[TelegramBot] Failed to parse incoming message: " + String(u.message().text()) + " Error: " + errorParsePtr->message, LOG_WARN);
            return;
        }

        auto *successParsePtr = static_cast<TelegramSuccessRequest<T> *>(parsePtr.get());
        T &data = successParsePtr->data;
        logger.log("[TelegramBot] Successfully parsed incoming message: '" + String(u.message().text()) + "' as command '" + data.command + "'", LOG_DEBUG);

        if constexpr (std::is_void<E>::value)
        {
            logger.log("[TelegramBot] Executing command '" + data.command + "' (void return)", LOG_DEBUG);
            handler(data);
        }
        else
        {
            E result = handler(data);
            logger.log("[TelegramBot] Executed command '" + data.command + "' (E return)", LOG_DEBUG);
            TelegramSuccessResponse<E> response(result);
            fb::Message msg(serializationTelegramResponce<E>(response), u.message().chat().id());
            bot.sendMessage(msg);
        }
    };
}