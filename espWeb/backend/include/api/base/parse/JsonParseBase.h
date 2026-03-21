#pragma once
#ifndef JSON_PARSE_BASE_H
#define JSON_PARSE_BASE_H

#include <Arduino.h>
#include <StringN.h>
#include <GSON.h>
#include <memory>
#include <map>
#include "JsonResult.h"
#include "JsonConverter.h"
#include "JsonTypes.h"

namespace api::json
{
    class Parse
    {
    public:
        struct Field
        {
            const ValueType expectedType;
            bool required;

            Field() = delete;
            Field(ValueType expectedType, bool required = true) : expectedType(expectedType), required(required) {}
        };

        virtual ~Parse() = default;

    protected:
        static std::unique_ptr<Convertible> parse(const gson::Entry &json, const std::map<const char *, Field> &expectedKeys);
    };
}

#endif // JSON_PARSE_BASE_H