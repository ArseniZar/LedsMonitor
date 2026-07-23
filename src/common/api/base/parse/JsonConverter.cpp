#include "JsonConverter.h"
#include <Arduino.h>

namespace api::json
{
    bool Converter::isCompatible(JsonType actualType, ValueType expectedType)
    {
        switch (expectedType)
        {
        case ValueType::Int:
        case ValueType::U_Long:
        case ValueType::U_Int8:
        case ValueType::U_Int16:
            return actualType == JsonType::Int;
        case ValueType::Float:
            return actualType == JsonType::Float;
        case ValueType::Bool:
            return actualType == JsonType::Bool;
        case ValueType::String8:
        case ValueType::String18:
        case ValueType::String32:
        case ValueType::String64:
            return actualType == JsonType::String;
        default:
            return false;
        }
    }

    Value Converter::convert(const char *rawValue, ValueType expectedType)
    {
        const Text data(rawValue);

        if (!data.valid())
        {
            return std::monostate{};
        }

        switch (expectedType)
        {
        case ValueType::Int:
        {
            return static_cast<int>(data.toInt());
        }

        case ValueType::U_Int8:
        {
            const int parsed = data.toInt();
            if (parsed < 0 || parsed > std::numeric_limits<uint8_t>::max())
            {
                return std::monostate{};
            }
            return static_cast<uint8_t>(parsed);
        }

        case ValueType::U_Int16:
        {
            const int parsed = data.toInt();
            if (parsed < 0 || parsed > std::numeric_limits<uint16_t>::max())
            {
                return std::monostate{};
            }
            return static_cast<uint16_t>(parsed);
        }

        case ValueType::U_Long:
        {
            const int64_t parsed = data.toInt64();
            if (parsed < 0 || static_cast<uint64_t>(parsed) > std::numeric_limits<unsigned long>::max())
            {
                return std::monostate{};
            }
            return static_cast<unsigned long>(parsed);
        }

        case ValueType::Float:
        {
            const float parsed = data.toFloat();
            if (std::isnan(parsed))
            {
                return std::monostate{};
            }
            return parsed;
        }

        case ValueType::Bool:
        {
            return data.toBool();
        }

        case ValueType::String8:
            return String8(data.c_str());
        case ValueType::String18:
            return StringN<18>(data.c_str());
        case ValueType::String32:
            return String32(data.c_str());
        case ValueType::String64:
            return String64(data.c_str());

        default:
            return std::monostate{};
        }
    }
}
