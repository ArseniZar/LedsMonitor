#include "TelegramBot.h"
Logger *TelegramBot::logger_ptr = nullptr;
EEPROMSetup *TelegramBot::eeprom_ptr = nullptr;
FastBot2 *TelegramBot::bot_ptr = nullptr;

TelegramBot::TelegramBot(Logger *logger, EEPROMSetup *eeprom)
{
    bot_ptr = new FastBot2(BOT_TOKEN);
    eeprom_ptr = eeprom;
    logger_ptr = logger;
}

TelegramBot::~TelegramBot()
{
    delete bot_ptr;
    bot_ptr = nullptr;
}

void TelegramBot::begin()
{
    bot_ptr->setLimit(BOT_LIMIT);
    logger_ptr->log("Bot limit set to: " + String(BOT_LIMIT), LOG_INFO);

    bot_ptr->setPollMode(fb::Poll::Sync, BOT_PERIOD);
    logger_ptr->log("Bot poll mode set to Sync, period: " + String(BOT_PERIOD), LOG_INFO);

    bot_ptr->updates.set(fb::Updates::Type::Message | fb::Updates::Type::ChannelPost);
    logger_ptr->log("Bot updates types set: Message | ChannelPost", LOG_INFO);

    bot_ptr->skipUpdates();
    logger_ptr->log("Skipped previous updates", LOG_INFO);

    bot_ptr->onUpdate(updateMsg);

    logger_ptr->log("Telegram bot initialized successfully", LOG_INFO);
}

void TelegramBot::updateMsg(fb::Update &u)
{
    Info info = parseMessage(u.message().text());
    switch (SH(info.command))
    {
    case SH(CMD_SCAN):
        onScan(u.message().chat());
        break;

    case SH(CMD_UPDATE):
        onUpdate(info);
    default:
    }
}

void TelegramBot::onScan(fb::ChatRead chat)
{
    fb::Message msg("/" + String(CMD_MAC) + ":" + String(eeprom_ptr->readString(ADDR_MAC) + " /" + String(CMD_NAME) + ":" + String(DEVICE_NAME)), chat.id());
    logger_ptr->log("Sending message ( MAC and NAME ) to chat:\nID: " + String(chat.id()) + "\nTitle: " + String(chat.title()), LOG_INFO);
    bot_ptr->sendMessage(msg);
}

void TelegramBot::onUpdate(Info &info)
{
    
}

Info TelegramBot::parseMessage(su::Text message)
{
    Info info;

    const int maxLines = 10;
    su::Text parts[maxLines];
    int linesCount = message.split(parts, maxLines, ' ');

    for (int i = 0; i < linesCount; i++)
    {
        su::Text line = parts[i];
        line.trim();

        int sep = line.indexOf(':');
        if (sep == -1)
            continue;

        su::Text key = line.substring(0, sep);
        su::Text value = line.substring(sep + 1);

        key.trim();
        value.trim();
        switch (key)
        {
        case CMD_COMMAND:
            info.command = String(value);
        case CMD_MAC:
            info.mac = String(value);
        case CMD_CHAT:
            info.chat_id = String(value);
        case CMD_COLOR:
            info.color = String(value);
        case CMD_STATUS:
            info.status = String(value);
        }
    }
    return info;
}

bool TelegramBot::tick()
{

    return bot_ptr->tick();
}