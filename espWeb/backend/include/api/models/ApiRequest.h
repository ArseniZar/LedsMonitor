#pragma once
#ifndef API_REQUEST_H
#define API_REQUEST_H

#include <Arduino.h>
#include <StringN.h>
#include "ApiContractsBase.h"
#include "JsonParseBase.h"

namespace api
{
    /*========================SuccessRequest============================*/
    
    template <typename T>
    struct SuccessRequest final : public Request
    {
        static_assert(std::is_base_of<json::Parse, T>::value, "T must inherit from json::Parse");

        T data;
        SuccessRequest() = delete;
        SuccessRequest(const T &data) : data(data) {}
        SuccessRequest(T &&data) : data(std::move(data)) {}
        bool isOk() const override { return true; }
    };

    /*=========================ErrorRequest===========================*/

    struct ErrorRequest final : public Request
    {
        String256 message;
        ErrorRequest() = delete;
        ErrorRequest(const char *msg) : message(msg) {}
        bool isOk() const override { return false; }
    };
}
#endif // API_REQUEST_H