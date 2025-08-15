#pragma once
#ifndef TELEGRAM_RESPONSE_H
#define TELEGRAM_RESPONSE_H

#include <Arduino.h>
#include "MessageSerializationBase.h"
#include "TelegramTypesBase.h"

namespace telegram
{
    /*==========================TelegramSuccessResponse============================*/

    template <typename T>
    struct TelegramSuccessResponse final : public TelegramResponse
    {
        static_assert(std::is_base_of<MessageSerialization, T>::value, "T must inherit from MessageSerialization");

        T data;
        TelegramSuccessResponse() = delete;
        TelegramSuccessResponse(const T &data) : data(data) {}
        bool isOk() const override { return true; }
    };

    /*============================TelegramErrorResponse===========================*/

    struct TelegramErrorResponse final : public TelegramResponse
    {
        String text;
        String message;
        TelegramErrorResponse() = delete;
        TelegramErrorResponse(const String &text, const String &msg) : text(text), message(msg) {}
        bool isOk() const override { return false; }
    };

}

#endif // TELEGRAM_RESPONSE_H