#pragma once
#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <type_traits>
#include "Logger.h"
#include "Storage.h"
#include "ConfigModels.h"
#include "ConfigDefault.h"



class ConfigManager
{
public:
    void begin();
    void save();
    void reset();

    NetworkConfig &getNetworkConfig();
    DeviceLedConfig &getDeviceLedConfig();
    TelegramBotConfig &getTelegramBotConfig();

    void saveNetworkConfig();
    void saveDeviceLedConfig();
    void saveTelegramBotConfig();

    void resetNetworkConfig();
    void resetDeviceLedConfig();
    void resetTelegramBotConfig();

    static ConfigManager &init(Logger &logger);

private:
	ConfigManager(Logger &logger);


	Logger &logger;
    
    NetworkConfig networkConfig;
    DeviceLedConfig deviceLedConfig;
    TelegramBotConfig telegramBotConfig;

	Storage<NetworkRuntimeConfig> wifiStorage;
	Storage<DeviceLedRuntimeConfig> deviceStorage;
	Storage<TelegramBotRuntimeConfig> telegramStorage;

    void load();
};

#endif

