#pragma once
#ifndef TELEGRAM_BOT_CONFIG_MODELS_H
#define TELEGRAM_BOT_CONFIG_MODELS_H

#include <Arduino.h>
#include <StringN.h>

struct TelegramBotRuntimeConfig;

class TelegramBotConfig
{
public:
    String64 token;
    uint8_t limitMessage;
    uint16_t periodUpdate;

    static const TelegramBotConfig &defaultConfig(const char *token,
                                                  uint8_t limitMessage,
                                                  uint16_t periodUpdate);

    static const TelegramBotConfig &fromDefault();  
    static constexpr const char *TYPE_NAME = "TelegramBotConfig";

    TelegramBotConfig(const TelegramBotConfig &def); 

    void operator=(const TelegramBotRuntimeConfig &config);
    void operator=(const TelegramBotConfig &config);
    bool operator==(const TelegramBotRuntimeConfig &config) const;

private:
    TelegramBotConfig(const char *token, uint8_t limitMessage, uint16_t periodUpdate);

    static TelegramBotConfig *defaultInstance;
};

struct TelegramBotRuntimeConfig
{
    static constexpr const char *PATH = "/telegramBot";
    static constexpr uint8_t KEY = 'T';

    String64 token;
    uint8_t limitMessage;
    uint16_t periodUpdate;
    TelegramBotRuntimeConfig() = default;
    TelegramBotRuntimeConfig(const TelegramBotConfig &config);
    void operator=(const TelegramBotConfig &config);
};

#endif // TELEGRAM_BOT_CONFIG_MODELS_H