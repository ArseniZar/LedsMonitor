#include "MessageParseBase.h"
namespace telegram
{
    std::unique_ptr<MessageConvertible> MessageParse::parsePairs(const su::Text *pairs, const int pairsSize, const std::map<const char *, std::regex> &expectedKeys)
    {

        struct Key
        {
            const char *keyPtr;
            Key() = delete;
            Key(const char *keyPtr) { this->keyPtr = keyPtr; }
            bool operator<(const Key &other) const
            {
                return strcmp(keyPtr, other.keyPtr) < 0;
            }
            const char *c_str() const
            {
                return keyPtr;
            }
        };

        if (pairsSize < static_cast<int>(expectedKeys.size()))
        {
            String128 buf;
            buf.add(F("Message Parse error: "));
            buf.add(F("Insufficient number of pairs: expected at least "));
            buf.add(expectedKeys.size());
            buf.add(F(", got "));
            buf.add(pairsSize);
            return std::make_unique<MessageError>(buf);
        }

        std::map<Key, String> map;
        for (const auto &[key, _] : expectedKeys)
        {
            map[key] = "#";
        }

        for (int i = 0; i < pairsSize; ++i)
        {
            const su::Text &pair = pairs[i];
            int equalIndex = pair.indexOf('=');
            if (equalIndex < 0)
            {
                String128 buf;
                buf.add(F("Message Parse error: "));
                buf.add(F("Missing '=' in pair # "));
                buf.add(i);
                buf.add(F(" missing '=' -> \""));
                buf.add(pair.c_str());
                buf.add(F("\""));
                return std::make_unique<MessageError>(buf);
            }

            String key(pair.substring(0, equalIndex).trim());
            String value(pair.substring(equalIndex + 1).trim());

            auto it = map.find(Key(key.c_str()));
            if (it != map.end())
            {
                if (it->second.equalsIgnoreCase(F("#")))
                {
                    if (!std::regex_match(value.c_str(), expectedKeys.at(it->first.c_str())))
                    {
                        String128 buf;
                        buf.add(F("Message Parse error: "));
                        buf.add(F("Value for key '"));
                        buf.add(key.c_str());
                        buf.add(F("' doesn't match pattern"));
                        return std::make_unique<MessageError>(buf);
                    }

                    it->second = std::move(value);
                }
                else
                {
                    String128 buf;
                    buf.add(F("Message Parse error: "));
                    buf.add(F("Duplicate key '"));
                    buf.add(key.c_str());
                    buf.add(F("' in pair "));
                    buf.add(i);
                    buf.add(F(": previous value='"));
                    buf.add(it->second.c_str());
                    buf.add(F("', new value='"));
                    buf.add(value.c_str());
                    buf.add(F("'"));
                    return std::make_unique<MessageError>(buf);
                }
            }
            else
            {
                // Неизвестный ключ (если нужно, можно раскомментировать)
                // Serial.print(F("Unknown key: "));
                // Serial.println(key);
            }
        }

        std::map<const char *, String> outMap;
        String128 buf;

        for (const auto &[key, value] : map)
        {
            if (value.equalsIgnoreCase(F("#")))
            {
                if (buf.length() == 0)
                {
                    buf.add(F("Message Parse error: "));
                    buf.add(F("Missing value for expected key: "));
                }
                buf.add(F("'"));
                buf.add(key.c_str());
                buf.add(F("' "));
            }

            outMap[key.c_str()] = std::move(value);
        }

        if (buf.length() > 0)
        {
            return std::make_unique<MessageError>(buf);
        }

        return std::make_unique<MessageParseSuccess<std::map<const char *, String>>>(std::move(outMap));
    }

}
// TODO: добавить жесткие условия проверки есть ли данные для парсинга и тд 