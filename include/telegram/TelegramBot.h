#pragma once
#ifndef TELEGRAM_BOT_H
#define TELEGRAM_BOT_H

#include <Arduino.h>
#include <FastBot2.h>
#include "Logger.h"
#include "TelegramApiModels.h"
#include "ApiParse.h"
#include "ApiSerialization.h"
#include "utils/helpers.h"
#include "MacAddress.h"
#include "TelegramBotConfigModels.h"

#ifndef TELEGRAM_BOT_TOKEN
#define TELEGRAM_BOT_TOKEN ""
#endif

#ifndef TELEGRAM_LIMIT_MESSAGE
#define TELEGRAM_LIMIT_MESSAGE 10
#endif

#ifndef TELEGRAM_PERIOD_UPDATE
#define TELEGRAM_PERIOD_UPDATE 1000
#endif

class TelegramBot
{
public:
    void begin();
    bool tick();
    static TelegramBot &init(Logger &logger, const MacAddress &mac);

    ~TelegramBot() = default;

    void setLimitMessage(uint8_t limitMessage);
    void setPeriodUpdate(uint16_t periodUpdate);
    void setToken(const char *token);
    void applyConfig(const TelegramBotConfig &config);

    uint8_t getLimitMessage() const;
    uint16_t getPeriodUpdate() const;
    const char *getToken() const;

    template <typename Request, typename Response = void>
    void registerCommand(const char *command, std::function<Response(Request &)> handler);

    template <typename Request = void, typename Response = void>
    void registerCommand(const char *command, std::function<Response()> handler);

private:
    Logger &logger;
    FastBot2 bot;
    const MacAddress &mac;

    uint8_t limitMessage;
    uint16_t periodUpdate;

    std::map<const char *, std::function<void(fb::Update &)>, StrCompare> handlers;

    TelegramBot() = delete;
    TelegramBot(Logger &logger, const MacAddress &mac);

    void handleUpdateMsg(fb::Update &request);
    void handleNotFound(String32 parseCommand, StringN<18> parseId, fb::Update &request);
};

#include "TelegramBot.tpp"
#endif
