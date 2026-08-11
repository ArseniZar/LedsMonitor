#pragma once
#ifndef JSON_SERIALIZATION_BASE_H
#define JSON_SERIALIZATION_BASE_H

#include <Arduino.h>
#include <GSON.h>
#include <array>
#include <utility>
#include "JsonTypes.h"

namespace api::json
{
    class Serialization
    {
    public:
        struct Field
        {
            Value value;

            Field() = delete;
            Field(const Value &value) : value(value) {}
        };

        virtual ~Serialization() = default;
        virtual gson::Str toJson() const = 0;

    protected:
        template <std::size_t N>
        static gson::Str serialization(const std::array<std::pair<const char *, Field>, N> &pairs);
    };
}

#include "JsonSerializationBase.tpp"

#endif // JSON_SERIALIZATION_H