#pragma once
#ifndef JSON_RESULT_H
#define JSON_RESULT_H

#include <Arduino.h>
#include <StringN.h>
#include "JsonContracts.h"

namespace api::json
{
    /*===================ParseSuccess===========================*/

    template <typename T>
    struct ParseSuccess final : public Convertible
    {
        T result;
        ParseSuccess() = delete;
        ParseSuccess(const T &result) : result(result) {}
        ParseSuccess(T &&result) : result(std::move(result)) {}
        bool isOk() const override { return true; }
    };

    /*====================Error=================================*/

    struct Error final : public Convertible
    {
        String128 message;
        Error() = delete;
        Error(const char *msg) : message(msg) {}
        bool isOk() const override { return false; }
    };
}

#endif // JSON_RESULT_H