#ifndef TELEGRAM_BOT_H
#define TELEGRAM_BOT_H
#include "config.h"
#include "Logger.h"
#include "FastBot2.h"
#include "EEPROMSetup.h"

struct Info
{
    String command;
    String mac;
    String chat_id;
    String color;
    String status;
};

class TelegramBot
{
private:
    static Logger *logger_ptr;
    static EEPROMSetup *eeprom_ptr;
    static FastBot2 *bot_ptr;

    static void updateMsg(fb::Update &u);
    static void onScan(fb::ChatRead chat);
    static Info parseMessage(su::Text message);
    static void onUpdate(Info &info);

public:
    TelegramBot(Logger *logger, EEPROMSetup *eeprom);
    ~TelegramBot();
    void begin();
    bool tick();
};

#endif
