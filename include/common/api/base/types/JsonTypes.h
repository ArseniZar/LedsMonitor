#pragma once
#ifndef JSON_TYPES_H
#define JSON_TYPES_H

#include <Arduino.h>
#include <StringN.h>
#include <GSON.h>
#include <variant>

namespace api::json
{
    using JsonType = gson::Type;
    
    enum class ValueType : uint8_t
    {
        Bool,
        Int,
        Float,
        U_Int8,
        U_Int16,
        U_Int32,
        String8,
        String18,
        String32,
        String64,
    };

    using Value = std::variant<std::monostate, bool, int, float, uint32_t, uint8_t, uint16_t, String8, StringN<18>, String32, String64>;
    
    const __FlashStringHelper* readType(ValueType t);
}

#endif // JSON_TYPES_H