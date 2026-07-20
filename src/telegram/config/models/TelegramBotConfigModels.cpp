#include "TelegramBotConfigModels.h"

TelegramBotConfig *TelegramBotConfig::defaultInstance = nullptr;

TelegramBotConfig::TelegramBotConfig(const TelegramBotConfig &def)
    : token(def.token), limitMessage(def.limitMessage), periodUpdate(def.periodUpdate)
{
}

TelegramBotConfig::TelegramBotConfig(const char *token, uint8_t limitMessage, uint16_t periodUpdate)
    : token(token), limitMessage(limitMessage), periodUpdate(periodUpdate)
{
}

const TelegramBotConfig &TelegramBotConfig::defaultConfig(const char *token,
                                                          uint8_t limitMessage,
                                                          uint16_t periodUpdate)
{
    if (!defaultInstance)
    {
        defaultInstance = new TelegramBotConfig(token, limitMessage, periodUpdate);
    }
    return *defaultInstance;
}

const TelegramBotConfig &TelegramBotConfig::fromDefault()
{
    return *defaultInstance;
}

void TelegramBotConfig::operator=(const TelegramBotRuntimeConfig &config)
{
    token = config.token;
    limitMessage = config.limitMessage;
    periodUpdate = config.periodUpdate;
}

void TelegramBotConfig::operator=(const TelegramBotConfig &config)
{
    token = config.token;
    limitMessage = config.limitMessage;
    periodUpdate = config.periodUpdate;
}

bool TelegramBotConfig::operator==(const TelegramBotRuntimeConfig &config) const
{
    return (token == config.token) &&
           (limitMessage == config.limitMessage) &&
           (periodUpdate == config.periodUpdate);
}

TelegramBotRuntimeConfig::TelegramBotRuntimeConfig(const TelegramBotConfig &config)
    : token(config.token), limitMessage(config.limitMessage), periodUpdate(config.periodUpdate)
{
}

void TelegramBotRuntimeConfig::operator=(const TelegramBotConfig &config)
{
    token = config.token;
    limitMessage = config.limitMessage;
    periodUpdate = config.periodUpdate;
}
