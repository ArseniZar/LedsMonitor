#pragma once
#include "TelegramParse.h"

namespace telegram
{
    template <typename T>
    std::unique_ptr<TelegramRequest> parseTelegramRequest(const su::Text &message)
    {
        if (message.length() < 3 || message[0] != '/' || message[message.length() - 1] != '/')
        {
            return std::make_unique<TelegramErrorRequest>("Invalid message format");
        }

        su::Text content = message.substring(1, message.length() - 1);
        int pairCount = content.count(';');
        su::Text pairs[pairCount];
        content.split(pairs, pairCount, ';');
        std::unique_ptr<TelegramRequest> response_ptr;
        auto parsePtr = T::fromMessage(pairs, pairCount);
        if (!parsePtr->isOk())
        {
            MessageError *errorPtr = static_cast<MessageError *>(parsePtr.get());
            return std::make_unique<TelegramErrorRequest>(errorPtr->message);
        }
        MessageParseSuccess<T> *successPtr = static_cast<MessageParseSuccess<T> *>(parsePtr.get());
        return std::make_unique<TelegramSuccessRequest<T>>(TelegramSuccessRequest<T>(successPtr->result));
    }
}
