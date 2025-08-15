#pragma once
#ifndef TELEGRAM_TYPES_BASE_H
#define TELEGRAM_TYPES_BASE_H

namespace telegram
{
    /*=========================TelegramRequest=============================*/

    struct TelegramRequest
    {
        virtual bool isOk() const = 0;
        virtual ~TelegramRequest() = default;
    };

    /*========================TelegramResponse============================*/

    struct TelegramResponse
    {
        virtual bool isOk() const = 0;
        virtual ~TelegramResponse() = default;
    };
}
#endif // TELEGRAM_TYPES_BASE_H