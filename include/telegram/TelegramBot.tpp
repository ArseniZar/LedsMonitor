#pragma once
#include "TelegramBot.h"

// TODO: измеить String на const char* 
// TODO: изменить навание функции на более подходящее, 
template <typename T, typename E = void>
void TelegramBot::registerCommand(const String &command, std::function<E(T &)> handler)
{
    static_assert(std::is_base_of<telegram::ModelBaseRequest, T>::value, "T must inherit from ModelBaseRequest");
    static_assert(std::is_void<E>::value || std::is_base_of<telegram::ModelBaseResponse, E>::value, "E must be void or inherit from ModelBaseResponse");
    
    handlers[command] = [handler, this](fb::Update &u)
    {
        auto parsePtr = telegram::parseTelegramRequest<T>(u.message().text().c_str());
        if (!parsePtr->isOk())
        {
            auto *errorParsePtr = static_cast<telegram::TelegramErrorRequest *>(parsePtr.get());
            logger.log(LOG_WARN, [&]() -> String256
                       { String256 buf;
                     buf.add(F("[TelegramBot] Failed to parse incoming message: "));
                     buf.add(u.message().text().c_str());
                     buf.add(F(" Error: "));
                     buf.add(errorParsePtr->message);
                    return buf; });

            return;
        }

        auto *successParsePtr = static_cast<telegram::TelegramSuccessRequest<T> *>(parsePtr.get());
        T data = std::move(successParsePtr->data); //XXX бесполезно просто передадим по ссылке
        logger.log(LOG_DEBUG, [&]() -> String256
                   { String256 buf;
                     buf.add(F("[TelegramBot] Successfully parsed incoming message: '"));
                     buf.add(u.message().text().c_str());
                     buf.add(F("' as command '"));
                     buf.add(data.command.c_str());
                     buf.add(F("'"));
                    return buf; });

        if constexpr (std::is_void<E>::value)
        {
            logger.log(LOG_DEBUG, [&]() -> String256
                       { String256 buf;
                     buf.add(F("[TelegramBot] Executing command '"));
                     buf.add(data.command.c_str());
                     buf.add(F("' (void return)"));
                    return buf; });

            handler(data);
        }
        else
        {
            E result = handler(data);
            logger.log(LOG_DEBUG, [&]() -> String128
                       { String128 buf;
                     buf.add(F("[TelegramBot] Executing command '"));
                     buf.add(data.command.c_str());
                     buf.add(F("' (E return)"));
                    return buf; });

            telegram::TelegramSuccessResponse<E> response(std::move(result));
            fb::Message msg(telegram::serializationTelegramResponce<E>(std::move(response)), u.message().chat().id());
            bot.sendMessage(msg);
        }
    };
}