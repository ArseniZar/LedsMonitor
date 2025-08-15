#include "MessageParseBase.h"
namespace telegram
{
    std::unique_ptr<MessageConvertible> MessageParse::parsePairs(const su::Text pairs[], const int pairsLength, const std::map<String, std::regex> &expectedKeys, std::map<String, String> &outMap)
    {
        if (pairsLength < static_cast<int>(expectedKeys.size()))
        {
            return std::make_unique<MessageError>("Insufficient number of pairs: expected at least " + String(expectedKeys.size()) + ", got " + String(pairsLength));
        }

        for (const auto &[key, _] : expectedKeys)
        {
            outMap[key] = "";
        }

        for (int i = 0; i < pairsLength; ++i)
        {
            const su::Text &pair = pairs[i];
            int equalIndex = pair.indexOf('=');
            if (equalIndex < 0)
            {
                return std::make_unique<MessageError>("Missing '=' in pair #" + String(i) + ": \"" + String(pair) + "\"");
            }

            String key = pair.substring(0, equalIndex).trim();
            String value = pair.substring(equalIndex + 1).trim();

            auto it = outMap.find(key);
            if (it != outMap.end())
            {
                if (it->second.isEmpty())
                {
                    if (!std::regex_match(value.c_str(), expectedKeys.find(key)->second))
                    {
                        return std::make_unique<MessageError>("Value for key '" + key + "' doesn't match pattern");
                    }
                    it->second = value;
                }
                else
                {
                    return std::make_unique<MessageError>("Duplicate key '" + key + "' in pair #" + String(i) +": previous value='" + String(it->second) +"', new value='" + String(value) + "'");
                }
            }
            else
            {
                // return std::make_unique<MessageError>(
                //     "Unknown key '" + key + "' in pair #" + String(i) + ": \"" + String(pair) + "\"");
            }
        }

        for (const auto &[key, value] : outMap)
        {
            if (value.isEmpty())
            {
                return std::make_unique<MessageError>("Missing value for expected key '" + key + "'");
            }
        }

        return nullptr;
    }

}