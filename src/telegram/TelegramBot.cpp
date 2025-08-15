#include "TelegramBot.h"

TelegramBot::TelegramBot(Logger &logger, const String &token, const MacAddress &mac) : logger(logger), bot(FastBot2(token)), mac(mac), limitMessage(10), periodUpdate(1000) {}

TelegramBot &TelegramBot::init(Logger &logger, const String &token, const MacAddress &mac)
{
    static TelegramBot instance(logger, token, mac);
    return instance;
}

void TelegramBot::begin()
{
    bot.setLimit(limitMessage);
    bot.setPollMode(fb::Poll::Sync, periodUpdate);
    bot.updates.set(fb::Updates::Type::Message | fb::Updates::Type::ChannelPost);
    bot.skipUpdates();
    bot.onUpdate([this](fb::Update &u)
                 { this->handleUpdateMsg(u); });
}

void TelegramBot::handleUpdateMsg(fb::Update &u)
{
    using namespace telegram;
    auto parseBasePtr = parseTelegramRequest<ModelBaseRequest>(u.message().text());
    if (parseBasePtr->isOk())
    {
        auto *successParseBasePtr = static_cast<TelegramSuccessRequest<ModelBaseRequest> *>(parseBasePtr.get());
        String &parseId = successParseBasePtr->data.id;
        if (mac.equals(parseId) || mac.isBroadcast(parseId))
        {
            auto it = handlers.find(successParseBasePtr->data.command);
            if (it != handlers.end())
            {
                it->second(u);
            }
            else
            {
            }
        }
    }
    else
    {
        auto *errorParseBasePtr = static_cast<TelegramErrorRequest *>(parseBasePtr.get());
        logger.log("[TelegramBot] Failed to parse incoming message: " + errorParseBasePtr->message, LOG_WARN);
    }
}

void TelegramBot::setLimitMessage(int limitMessage)
{
    this->limitMessage = limitMessage;
    bot.setLimit(limitMessage);
}

void TelegramBot::setPeriodUpdate(int periodUpdate)
{
    this->periodUpdate = periodUpdate;
    bot.setPollMode(fb::Poll::Sync, periodUpdate);
}

bool TelegramBot::tick()
{

    return bot.tick();
}