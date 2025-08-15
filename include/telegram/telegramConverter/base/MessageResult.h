#pragma once
#ifndef MESSAGE_RESULT_H
#define MESSAGE_RESULT_H

#include <Arduino.h>

namespace telegram 
{
    /*=====================MessageConvertible=========================*/

    struct MessageConvertible
    {
        virtual bool isOk() const = 0;
        virtual ~MessageConvertible() = default;
    };

    /*===================MessageParseSuccess===========================*/

    template <typename T>
    struct MessageParseSuccess final : public MessageConvertible
    {
        T result;

        MessageParseSuccess(const T &result) : result(result) {}
        bool isOk() const override { return true; }
    };

    /*====================MessageError=================================*/

    struct MessageError final : public MessageConvertible
    {
        String message;

        MessageError(const String &message) : message(message) {}
        bool isOk() const override { return false; }
    };

}

#endif // MESSAGE_RESULT_H