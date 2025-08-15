#pragma once
#ifndef TELEGRAM_REQUEST_H
#define TELEGRAM_REQUEST_H

#include <Arduino.h>
#include "MessageParseBase.h"
#include "TelegramTypesBase.h"

namespace telegram
{
    /*========================TelegramSuccessRequest============================*/

    template <typename T>
    struct TelegramSuccessRequest final : public TelegramRequest
    {
        static_assert(std::is_base_of<MessageParse, T>::value, "T must inherit from MessageParse");

        T data;
        TelegramSuccessRequest() = delete;
        TelegramSuccessRequest(const T &data) : data(data) {}
        bool isOk() const override { return true; }
    };

    /*========================TelegramErrorRequest===========================*/

    struct TelegramErrorRequest final : public TelegramRequest
    {
        String message;
        TelegramErrorRequest() = delete;
        TelegramErrorRequest(const String &msg) : message(msg) {}
        bool isOk() const override { return false; }
    };

}

#endif // TELEGRAM_REQUEST_H