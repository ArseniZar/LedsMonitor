#include "TelegramBot.h"

TelegramBot::TelegramBot(Logger &logger, const MacAddress &mac) : logger(logger), bot(TELEGRAM_BOT_TOKEN), mac(mac), limitMessage(TELEGRAM_LIMIT_MESSAGE), periodUpdate(TELEGRAM_PERIOD_UPDATE) {}

TelegramBot &TelegramBot::init(Logger &logger, const MacAddress &mac)
{
    static TelegramBot instance(logger, mac);
    return instance;
}

void TelegramBot::begin()
{
    bot.setLimit(limitMessage);
    bot.setPollMode(fb::Poll::Sync, periodUpdate);
    bot.updates.set(fb::Updates::Type::Message | fb::Updates::Type::ChannelPost);
    bot.skipUpdates();
    bot.onUpdate([this](fb::Update &request)
                 { this->handleUpdateMsg(request); });
}

void TelegramBot::handleUpdateMsg(fb::Update &request)
{
    auto apiRequestBasePtr = api::parseRequest<api::telegram::ModelBaseRequest>(request.message().text().c_str());
    if (apiRequestBasePtr->isOk())
    {
        auto *successApiRequestBasePtr = static_cast<api::SuccessRequest<api::telegram::ModelBaseRequest> *>(apiRequestBasePtr.get());
        String32 parseCommand = std::move(successApiRequestBasePtr->data.command);
        StringN<18> parseId = std::move(successApiRequestBasePtr->data.id);

        logger.log(LOG_INFO, [&]() -> String256
                   { String256 buf;
                        buf.add(F("[TelegramBot::handleUpdateMsg] Telegram Request parse base: command="));
                        buf.add(parseCommand.c_str());
                        buf.add(F(", id="));
                        buf.add(parseId);
                        return buf; });

        logger.log(LOG_DEBUG, [&]() -> String256
                   { String256 buf;
                        buf.add(F("[TelegramBot::handleUpdateMsg] Device MAC="));
                        buf.add(mac.getMac());
                        buf.add(F(", target ID="));
                        buf.add(parseId);
                        return buf; });

        if (mac.equals(parseId) || mac.isBroadcast(parseId))
        {
            auto it = handlers.find(parseCommand.c_str());
            if (it != handlers.end())
            {
                it->second(request);
            }
            else
            {
                this->handleNotFound(parseCommand, parseId, request);
            }
        }
        else
        {

            logger.log(LOG_INFO, [&]() -> String256
                       { String256 buf;
                            buf.add(F("[TelegramBot::handleUpdateMsg] Command '"));
                            buf.add(parseCommand.c_str());
                            buf.add(F("' not intended for this device (MAC="));
                            buf.add(mac.getMac());
                            buf.add(F(", target ID="));
                            buf.add(parseId);
                            buf.add(F(")"));
                            return buf; });
        }
    }
    else
    {
        auto *errorApiRequestBasePtr = static_cast<api::ErrorRequest *>(apiRequestBasePtr.get());

        logger.log(LOG_WARN, [&]() -> String256
                   { String256 buf;
                        buf.add(F("[TelegramBot::handleUpdateMsg] Failed to parse ModelBaseRequest, error="));
                        buf.add(errorApiRequestBasePtr->message);
                        return buf; });
        const auto response = api::ErrorResponse(400, errorApiRequestBasePtr->message);
        gson::Str payload = api::serializeResponse(response);
        logger.log(LOG_DEBUG, [&]() -> String256
                   { String256 buf; buf.add(F("(TelegramBot::registerCommand) Response payload (parse error): ")); buf.add(Text(payload).c_str()); return buf; });
        bot.sendMessage(fb::Message(std::move(payload), request.message().chat().id()));
    }
}

void TelegramBot::handleNotFound(String32 parseCommand, StringN<18> parseId, fb::Update &request)
{
    logger.log(LOG_WARN, [&]() -> String128
               {String128 buf; buf.add(F("(TelegramBot::handleUpdateMsg) ")); buf.add(parseCommand.c_str()); buf.add(" "); buf.add(parseId); buf.add(F(" Resource not found.")); return buf; });
    const auto responce = api::ErrorResponse(404, String32(F("Resource not found")));
    const gson::Str payload = api::serializeResponse(responce);
    logger.log(LOG_DEBUG, [&]() -> String128
               {String128 buf; buf.add(F("(TelegramBot::handleUpdateMsg) Response payload: ")); buf.add(Text(payload).c_str()); return buf; });
    bot.sendMessage(fb::Message(std::move(payload), request.message().chat().id()));
}

void TelegramBot::setLimitMessage(uint8_t limitMessage)
{
    this->limitMessage = limitMessage;
    bot.setLimit(limitMessage);
}

void TelegramBot::setPeriodUpdate(uint16_t periodUpdate)
{
    this->periodUpdate = periodUpdate;
    bot.setPollMode(bot.getPollMode(), periodUpdate);
}

void TelegramBot::setToken(const char *token)
{
    bot.setToken(token);
}

void TelegramBot::applyConfig(const TelegramBotConfig &config)
{
    if (bot.getToken() != config.token.c_str())
    {
        setToken(config.token.c_str());
        logger.log(LOG_DEBUG, [&]() -> String128
                   { String128 buf; buf = F("(TelegramBot::applyConfig) Token changed"); return buf; });
    }
    else
    {
        logger.log(LOG_DEBUG, [&]() -> String128
                   { String128 buf; buf = F("(TelegramBot::applyConfig) Token No changed"); return buf; });
    }

    if (limitMessage != config.limitMessage)
    {
        setLimitMessage(config.limitMessage);
        logger.log(LOG_DEBUG, [&]() -> String128
                   { String128 buf; buf = F("(TelegramBot::applyConfig) LimitMessage changed"); return buf; });
    }
    else
    {
        logger.log(LOG_DEBUG, [&]() -> String128
                   { String128 buf; buf = F("(TelegramBot::applyConfig) LimitMessage no changed"); return buf; });
    }

    if (periodUpdate != config.periodUpdate)
    {
        setPeriodUpdate(config.periodUpdate);
        logger.log(LOG_DEBUG, [&]() -> String128
                   { String128 buf; buf = F("(TelegramBot::applyConfig) PeriodUpdate changed"); return buf; });
    }
    else
    {
        logger.log(LOG_DEBUG, [&]() -> String128
                   { String128 buf; buf = F("(TelegramBot::applyConfig) PeriodUpdate no changed"); return buf; });
    }
}

uint8_t TelegramBot::getLimitMessage() const
{

    return limitMessage;
}

const char *TelegramBot::getToken() const
{
    return const_cast<FastBot2 &>(bot).getToken().c_str();
}

uint16_t TelegramBot::getPeriodUpdate() const
{
    return periodUpdate;
}

bool TelegramBot::tick()
{

    return bot.tick();
}