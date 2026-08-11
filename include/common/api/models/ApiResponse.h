#pragma once
#ifndef API_RESPONSE_H
#define API_RESPONSE_H

#include <Arduino.h>
#include <StringN.h>
#include "ApiContractsBase.h"
#include "JsonSerializationBase.h"

namespace api
{
    /*========================SuccessResponse============================*/

    template <typename T>
    struct SuccessResponse final : public Response
    {
        static_assert(std::is_base_of<json::Serialization, T>::value, "T must inherit from json::Serialization");

        int code;
        T data;
        SuccessResponse() = delete;
        SuccessResponse(const int code, const T &data) : code(code), data(data) {}
        SuccessResponse(const int code, T &&data) : code(code), data(std::move(data)) {}
        bool isOk() const override { return true; }
        int getCode() const override { return code; }
    };

    /*=========================ErrorResponse===========================*/

    struct ErrorResponse final : public Response
    {
        int code;
        String256 message;
        ErrorResponse() = delete;
        ErrorResponse(const int code, const char *msg) : code(code), message(msg) {}
        bool isOk() const override { return false; }
        int getCode() const override { return code; };
    };
}
#endif // API_RESPONSE_H