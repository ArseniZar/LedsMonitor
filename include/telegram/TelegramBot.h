#pragma once
#ifndef TELEGRAM_BOT_H
#define TELEGRAM_BOT_H

#include <FastBot2.h>
#include <map>
#include "Logger.h"
#include "MessageModelsBase.h"
#include "TelegramTypesBase.h"
#include "TelegramParse.h"
#include "TelegramSerialization.h"
#include "MacAddress.h"

class TelegramBot
{
public:
    void begin();
    bool tick();

    ~TelegramBot() = default;
    void setLimitMessage(int limitMessage);
    void setPeriodUpdate(int periodUpdate);

    template <typename T, typename E = void>
    void registerCommand(const String &command, std::function<E(T &)> handler);
    static TelegramBot &init(Logger &logger, const String &token, const MacAddress &mac);

private:
    Logger &logger;
    FastBot2 bot;
    const MacAddress &mac;

    int limitMessage;
    int periodUpdate;

    std::map<String, std::function<void(fb::Update &)>> handlers;
    
    TelegramBot() = delete;
    void handleUpdateMsg(fb::Update &u);
    TelegramBot(Logger &logger, const String &token, const MacAddress &mac);
};

#include "TelegramBot.tpp"
#endif
