#pragma once
#ifndef TELEGRAM_BOT_H
#define TELEGRAM_BOT_H

#include <Arduino.h>
#include <FastBot2.h>
#include <map>
#include "Logger.h"
#include "MessageModelsBase.h"
#include "TelegramTypesBase.h"
#include "TelegramParse.h"
#include "TelegramSerialization.h"
#include "MacAddress.h"
#include "LedDeviceModels.h"
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

    ~TelegramBot() = default;
    void setLimitMessage(uint8_t limitMessage);
    void setPeriodUpdate(uint16_t periodUpdate);
    void setToken(const char *token);
    void applyConfig(const TelegramBotConfig &config);
    uint8_t getLimitMessage() const;
    uint16_t getPeriodUpdate() const; 
    const char * getToken() const; 
    
    template <typename T, typename E = void>
    void registerCommand(const String &command, std::function<E(T &)> handler);
    static TelegramBot &init(Logger &logger, const MacAddress &mac);

private:
    Logger &logger;
    FastBot2 bot;
    const MacAddress &mac;

    uint8_t limitMessage;
    uint16_t periodUpdate;
    // TODO: подумать о смене String Sting32 или const char* для экономии памяти
    std::map<String, std::function<void(fb::Update &)>> handlers;

    TelegramBot() = delete;
    TelegramBot(Logger &logger, const MacAddress &mac);
    void handleUpdateMsg(fb::Update &u);
};

#include "TelegramBot.tpp"
#endif
