#pragma once
#include "JsonConverter.h"

namespace api::json
{
    template <typename T>
    std::optional<T> Converter::toOptional(const Value &value)
    {
        if (const T *p = std::get_if<T>(&value))
        {
            return *p;
        }

        return std::nullopt;
    }
}