#pragma once
#ifndef MESSAGE_PARSE_BASE_H
#define MESSAGE_PARSE_BASE_H

#include <Arduino.h>
#include <memory>
#include <map>
#include <regex>
#include <StringUtils.h>
#include "MessageResult.h"

namespace telegram
{
    class MessageParse
    {
    public:
        virtual int countKey() const = 0;
        virtual ~MessageParse() = default;

    protected:
        static std::unique_ptr<MessageConvertible> parsePairs(const su::Text pairs[], const int pairsLength, const std::map<String, std::regex> &expectedKeys, std::map<String, String> &outMap);
    };
}
#endif // MESSAGE_PARSE_BASE_H
