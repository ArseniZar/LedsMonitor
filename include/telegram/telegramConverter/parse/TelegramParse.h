#pragma once
#ifndef TELEGRAM_PARSE_H
#define TELEGRAM_PARSE_H

#include <Arduino.h>
#include <memory>
#include <StringUtils.h>
#include "MessageResult.h"
#include "TelegramTypesBase.h"
#include "TelegramRequest.h"

namespace telegram
{
    template <typename T>
    std::unique_ptr<TelegramRequest> parseTelegramRequest(const su::Text &text);
}

#include "TelegramParse.tpp"
#endif // TELEGRAM_PARSE_H
