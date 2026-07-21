#include "JsonTypes.h"

namespace api::json
{
    const __FlashStringHelper* readType(ValueType t) {
        switch (t) {
            case ValueType::Bool: return F("Bool");
            case ValueType::Int: return F("Int");
            case ValueType::Float: return F("Float");
            case ValueType::U_Long: return F("Unsigned Long");
            case ValueType::U_Int8: return F("Unsigned Int8");
            case ValueType::U_Int16: return F("Unsigned Int16");
            case ValueType::String18: return F("String18");
            case ValueType::String32: return F("String32");
            case ValueType::String64: return F("String64");
            default: return F("Unknown");
        }
    }
}