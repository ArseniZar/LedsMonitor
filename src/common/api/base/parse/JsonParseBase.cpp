#include "JsonParseBase.h"

namespace api::json
{
    std::unique_ptr<Convertible> Parse::parse(const gson::Entry &json, const std::map<const char *, Field, StrCompare> &expectedKeys)
    {
        if (expectedKeys.empty())
        {
            return std::make_unique<Error>(String128(F("[ERROR] | [JsonParse::parse] | [NoExpectedKeys] | expectedKeys map is empty")));
        }

        if (!json.is(JsonType::Object))
        {
            return std::make_unique<Error>(String128(F("[ERROR] | [JsonParse::parse] | [DataIsNotObject] | expected=object")));
        }

        if (json.length() == 0)
        {
            return std::make_unique<Error>(String128(F("[ERROR] | [JsonParse::parse] | [EmptyData] | data object is empty")));
        }

        struct Key
        {
            const char *keyPtr;
            Key() = delete;
            Key(const char *keyPtr) : keyPtr(keyPtr) {}
            bool operator<(const Key &other) const
            {
                return strcmp(keyPtr, other.keyPtr) < 0;
            }
            const char *c_str() const
            {
                return keyPtr;  
            }
        };

        std::map<Key, Value> map;
        for (const auto &[key, _] : expectedKeys)
        {
            map[Key(key)] = std::monostate{};
        }

        for (int i = 0; i < (int)json.length(); ++i)
        {
            const gson::Entry &jsonPair = json.get(i);
            if (jsonPair.isContainer())
            {
                String256 buf;
                buf.add(F("[ERROR] | [JsonParse::parse] | [InvalidKeyValuePair] | key='"));
                buf.add(jsonPair.key().c_str());
                buf.add(F("'; jsonType='"));
                buf.add(gson::readType(jsonPair.type()));
                buf.add(F("'; value is not a primitive type! Currently, only simple types (int, float, bool, string) are supported."));
                return std::make_unique<Error>(buf);
            }

            Text::Cstr jsonKeyText = jsonPair.key().c_str();
            Text::Cstr jsonValText = jsonPair.value().c_str();

            Key jsonKey(jsonKeyText);
            const char *jsonValue = jsonValText;

            auto it = map.find(jsonKey);
            if (it == map.end())
            {
                continue; // (Unknown key ignored)
            }

            if (!std::holds_alternative<std::monostate>(it->second))
            {
                String256 buf;
                buf.add(F("[ERROR] | [JsonParse::parse] | [DuplicateKey] | key="));
                buf.add(jsonKey.c_str());
                buf.add(F("'; newValue='"));
                buf.add(jsonValue);
                buf.add(F("'"));
                return std::make_unique<Error>(buf);
            }

            const Field &field = expectedKeys.at(jsonKey.c_str());

            if (!Converter::isCompatible(jsonPair.type(), field.expectedType))
            {
                String256 buf;
                buf.add(F("[ERROR] | [JsonParse::parse] | [TypeMismatch] | key="));
                buf.add(jsonKey.c_str());
                buf.add(F("; value='"));
                buf.add(jsonValue);
                buf.add(F("'; jsonType='"));
                buf.add(gson::readType(jsonPair.type()));
                buf.add(F("'; expected='"));
                buf.add(readType(field.expectedType));
                buf.add(F("'"));
                return std::make_unique<Error>(buf);
            }

            const Value value = Converter::convert(jsonValue, field.expectedType);
            if (std::holds_alternative<std::monostate>(value))
            {
                String256 buf;
                buf.add(F("[ERROR] | [JsonParse::parse] | [InvalidValue] | key="));
                buf.add(jsonKey.c_str());
                buf.add(F("; value='"));
                buf.add(jsonValue);
                buf.add(F("'; expected='"));
                buf.add(readType(field.expectedType));
                buf.add(F("'; reason=conversion failed or out-of-range"));
                return std::make_unique<Error>(buf);
            }

            it->second = std::move(value);
        }

        std::map<const char *, Value, StrCompare> outMap;
        String128 missingKeys;
        bool hasMissingRequiredKeys = false;
        bool hasProvidedKnownValue = false;

        for (const auto &[key, value] : map)
        {
            if (!std::holds_alternative<std::monostate>(value))
            {
                hasProvidedKnownValue = true;
            }
            else if (expectedKeys.at(key.c_str()).required)
            {
                missingKeys.add(key.c_str());
                missingKeys.add(F(","));
                hasMissingRequiredKeys = true;
            }

            outMap[key.c_str()] = std::move(value);
        }

        if (!hasProvidedKnownValue)
        {
            return std::make_unique<Error>(String256(F("[ERROR] | [JsonParse::parse] | [EmptyPayload] | at least one known field is required")));
        }

        if (hasMissingRequiredKeys)
        {
            String256 buf;
            buf.add(F("[ERROR] | [JsonParse::parse] | [MissingRequiredKeys] | keys="));
            buf.add(missingKeys.c_str());
            return std::make_unique<Error>(buf);
        }

        return std::make_unique<ParseSuccess<std::map<const char *, Value, StrCompare>>>(std::move(outMap));
    }
}