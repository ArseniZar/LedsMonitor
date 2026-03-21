#include "JsonConverter.h"
#include <Arduino.h>
#include <limits>

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
        case ValueType::String32:
        case ValueType::String64:
            return actualType == JsonType::String;
        default:
            return false;
        }
    }
    
    Value Converter::convert(const char *rawValue, ValueType expectedType)
    {
        const String data(rawValue);

        switch (expectedType)
        {
        case ValueType::Int:
            return static_cast<int>(data.toInt());
        case ValueType::U_Long:
        {
            const long parsed = data.toInt();
            if (parsed < 0)
            {
                return std::monostate{};
            }
            return static_cast<unsigned long>(parsed);
        }
        case ValueType::U_Int8:
        {
            const long parsed = data.toInt();
            if (parsed < 0 || parsed > std::numeric_limits<uint8_t>::max())
            {
                return std::monostate{};
            }
            return static_cast<uint8_t>(parsed);
        }
        case ValueType::U_Int16:
        {
            const long parsed = data.toInt();
            if (parsed < 0 || parsed > std::numeric_limits<uint16_t>::max())
            {
                return std::monostate{};
            }
            return static_cast<uint16_t>(parsed);
        }
        case ValueType::Float:
            return static_cast<float>(data.toFloat());
        case ValueType::Bool:
            if (data.equalsIgnoreCase("true"))
            {
                return true;
            }
            if (data.equalsIgnoreCase("false"))
            {
                return false;
            }
            return std::monostate{};
        case ValueType::String32:
            return String32(data.c_str());
        case ValueType::String64:
            return String64(data.c_str());
        default:
            return std::monostate{};
        }
    }
}
