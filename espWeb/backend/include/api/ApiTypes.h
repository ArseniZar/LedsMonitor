#pragma once
#ifndef API_TYPES_H
#define API_TYPES_H

#include <Arduino.h>
#include <vector>
#include <concepts>
#include "JsonSerializable.h"
#include "JsonParse.h"
#include "Meta.h"

namespace api
{

    enum class ApiResponseType
    {
        SuccessList,
        SuccessSingle,
        Error
    };

    struct ApiResponse
    {
        virtual bool isOk() const = 0;
        virtual int getCode() const { return 200; };
        virtual ApiResponseType type() const = 0;
        virtual ~ApiResponse() = default;
    };

    template <typename T>
    struct ApiSuccessResponseSingle : public ApiResponse
    {
        static_assert(std::is_base_of<JsonSerializable, T>::value, "T must inherit from JsonSerializable");

        bool ok = true;
        T result;
        bool isOk() const override
        {
            return ok;
        }
        ApiResponseType type() const override
        {
            return ApiResponseType::SuccessSingle;
        }

    };

    template <typename T>
    struct ApiSuccessResponseList : public ApiResponse
    {
        static_assert(std::is_base_of<JsonSerializable, T>::value, "T must inherit from JsonSerializable");

        bool ok = true;
        std::vector<T> result;
        bool isOk() const override
        {
            return ok;
        }
        ApiResponseType type() const override
        {
            return ApiResponseType::SuccessList;
        }
    };

    struct ApiErrorResponse : public ApiResponse
    {
        const bool ok = false;
        int error_code;
        String message;

        ApiErrorResponse(int code, const String &msg) : error_code(code), message(msg) {}

        bool isOk() const override
        {
            return ok;
        }

        ApiResponseType type() const override
        {
            return ApiResponseType::Error;
        }

        int getCode() const override { return error_code; };
    };

    enum class ApiRequestType
    {
        SuccessList,
        SuccessSingle,
        Error
    };

    struct ApiRequest
    {
        virtual bool isOk() const { return true; }
        virtual ApiRequestType type() const = 0;
        virtual ~ApiRequest() = default;
    };

    template <typename T>
    struct ApiSuccessRequestSingle : public ApiRequest
    {
        static_assert(std::is_base_of<JsonParse, T>::value, "T must inherit from JsonParse");
        T data;
        ApiRequestType type() const override
        {
            return ApiRequestType::SuccessSingle;
        }
    };

    template <typename T>
    struct ApiSuccessRequestList : public ApiRequest

    {
        static_assert(std::is_base_of<JsonParse, T>::value, "T must inherit from JsonParse");
        std::vector<T> data;
        ApiRequestType type() const override
        {
            return ApiRequestType::SuccessList;
        }
    };

    struct ApiErrorRequest : public ApiRequest
    {
        String message;
        ApiErrorRequest(const String &msg) : message(msg) {}
        bool isOk() const override
        {
            return false;
        }
        ApiRequestType type() const override
        {
            return ApiRequestType::Error;
        }
    };

    namespace ApiResponseConstants
    {
        constexpr const char *OK = "ok";
        constexpr const char *RESULT = "result";
        constexpr const char *ERROR_CODE = "error_code";
        constexpr const char *MESSAGE = "message";
    }

    namespace ApiRequestConstants
    {
        constexpr const char *DATA = "data";
    }

}
#endif // API_TYPES_H
