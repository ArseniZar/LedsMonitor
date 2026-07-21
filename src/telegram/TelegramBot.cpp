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
    logger.log(LOG_DEBUG, [&]() -> String256
               { String256 buf;
                                buf.add(F("[TelegramBot::handleUpdateMsg] Raw update text: '"));
                                buf.add(request.message().text().c_str());
                                buf.add('\'');
                                return buf; });

    auto apiRequestBasePtr = api::parseRequest<api::telegram::ModelBaseRequest>(request.message().text().c_str());
    if (apiRequestBasePtr->isOk())
    {
        auto *successApiRequestBasePtr = static_cast<api::SuccessRequest<api::telegram::ModelBaseRequest> *>(apiRequestBasePtr.get());
        String32 parseCommand = std::move(successApiRequestBasePtr->data.command);
        StringN<18> parseId = std::move(successApiRequestBasePtr->data.id);
        logger.log(LOG_INFO, [&]() -> String256
                   { String256 buf;
                                    buf.add(F("[TelegramBot::handleUpdateMsg] Parsed base request: command="));
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
            logger.log(LOG_INFO, [&]() -> String256
                       { String256 buf;
                                                 buf.add(F("[TelegramBot::handleUpdateMsg] Command is for this device or broadcast (MAC="));
                                                 buf.add(mac.getMac());
                                                 buf.add(F(", target ID="));
                                                 buf.add(parseId);
                                                 buf.add(F(")"));
                                                 return buf; });

            auto it = handlers.find(parseCommand.c_str());
            if (it != handlers.end())
            {
                logger.log(LOG_INFO, [&]() -> String256
                           { String256 buf;
                                            buf.add(F("[TelegramBot::handleUpdateMsg] Executing handler for command="));
                                            buf.add(parseCommand.c_str());
                                            buf.add(F(", id="));
                                            buf.add(parseId);
                                            return buf; });

                logger.log(LOG_DEBUG, [&]() -> String256
                           { String256 buf;
                                            buf.add(F("[TelegramBot::handleUpdateMsg] Handler found in map for command="));
                                            buf.add(parseCommand.c_str());
                                            return buf; });
                it->second(request);
            }
            else
            {
                logger.log(LOG_WARN, [&]() -> String256
                           { String256 buf;
                                            buf.add(F("[TelegramBot::handleUpdateMsg] Command not implemented: "));
                                            buf.add(parseCommand.c_str());
                                            buf.add(F(", id="));
                                            buf.add(parseId);
                                            return buf; });
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

        logger.log(LOG_DEBUG, [&]() -> String256
                   { String256 buf;
                                    buf.add(F("[TelegramBot::handleUpdateMsg] Parse error. Raw text='"));
                                    buf.add(request.message().text().c_str());
                                    buf.add('\'');
                                    return buf; });
    }
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
    }
    if (limitMessage != config.limitMessage)
    {
        setLimitMessage(config.limitMessage);
    }
    if (periodUpdate != config.periodUpdate)
    {
        setPeriodUpdate(config.periodUpdate);
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