#pragma once
#ifndef JSON_SERIALIZATION_BASE_H
#define JSON_SERIALIZATION_BASE_H

#include <Arduino.h>
#include <GSON.h>
#include <map>
#include "JsonTypes.h"

namespace api::json
{
    class Serialization
    {
    public:
        struct Field
        {
            const Value &value;

            Field() = delete;
            Field(const Value &value) : value(value) {}
        };

        virtual ~Serialization() = default;
        virtual gson::Str toJson() const = 0;

    protected:
        static gson::Str serialization(const std::map<const char *, Field> &pairs);
    };
}

#endif // JSON_SERIALIZATION_H