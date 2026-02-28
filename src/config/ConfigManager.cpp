#include "ConfigManager.h"

ConfigManager &ConfigManager::init(Logger &logger)
{
    static ConfigManager instance(logger);
    return instance;
}

ConfigManager::ConfigManager(Logger &logger)
    : logger(logger),
      networkConfig(DEFAULT_NETWORK_CONFIG),
      deviceLedConfig(DEFAULT_DEVICE_CONFIG),
      telegramBotConfig(DEFAULT_TELEGRAM_CONFIG),
      wifiStorage("/wifi.dat", 'W'),
      deviceStorage("/device.dat", 'D'),
      telegramStorage("/telegram.dat", 'T')
{
}

void ConfigManager::begin()
{
    logger.log(LOG_INFO, [&]() -> String128
               { String128 buf; buf = F("(ConfigManager::begin) Initializing configuration storage..."); return buf; });

    wifiStorage.begin();
    deviceStorage.begin();
    telegramStorage.begin();
    load();

    logger.log(LOG_INFO, [&]() -> String128
               { String128 buf; buf = F("(ConfigManager::begin) Configuration initialized."); return buf; });
}

void ConfigManager::load()
{
    networkConfig = wifiStorage.readData();
    if (!wifiStorage.isLoaded())
    {
        networkConfig = DEFAULT_NETWORK_CONFIG;
        wifiStorage.writeData(networkConfig); 
        logger.log(LOG_WARN, [&]() -> String128
                   { String128 buf; buf = F("(ConfigManager::load) No stored NetworkConfig found; using defaults and saving."); return buf; });
    }
    else
    {
        logger.log(LOG_INFO, [&]() -> String128
                   { String128 buf; buf = F("(ConfigManager::load) Loaded NetworkConfig from storage."); return buf; });
    }

    deviceLedConfig = deviceStorage.readData();
    if (!deviceStorage.isLoaded())
    {
        deviceLedConfig = DEFAULT_DEVICE_CONFIG;
        deviceStorage.writeData(deviceLedConfig); 
        logger.log(LOG_WARN, [&]() -> String128
                   { String128 buf; buf = F("(ConfigManager::load) No stored DeviceLedConfig found; using defaults and saving."); return buf; });
    }
    else
    {
        logger.log(LOG_INFO, [&]() -> String128
                   { String128 buf; buf = F("(ConfigManager::load) Loaded DeviceLedConfig from storage."); return buf; });
    }

    telegramBotConfig = telegramStorage.readData();
    if (!telegramStorage.isLoaded())
    {
        telegramBotConfig = DEFAULT_TELEGRAM_CONFIG;
        telegramStorage.writeData(telegramBotConfig); 
        logger.log(LOG_WARN, [&]() -> String128
                   { String128 buf; buf = F("(ConfigManager::load) No stored TelegramBotConfig found; using defaults and saving."); return buf; });
    }
    else
    {
        logger.log(LOG_INFO, [&]() -> String128
                   { String128 buf; buf = F("(ConfigManager::load) Loaded TelegramBotConfig from storage."); return buf; });
    }
}

void ConfigManager::save()
{
    logger.log(LOG_INFO, [&]() -> String128
               { String128 buf; buf = F("(ConfigManager::save) Saving all configuration sections to storage..."); return buf; });

    wifiStorage.writeData(networkConfig);
    deviceStorage.writeData(deviceLedConfig);
    telegramStorage.writeData(telegramBotConfig);

    logger.log(LOG_DEBUG, [&]() -> String128
               { String128 buf; buf = F("(ConfigManager::save) All configuration sections saved."); return buf; });
}

void ConfigManager::reset()
{
    logger.log(LOG_WARN, [&]() -> String128
               { String128 buf; buf = F("(ConfigManager::reset) Resetting all configurations to defaults..."); return buf; });

    networkConfig = DEFAULT_NETWORK_CONFIG;
    deviceLedConfig = DEFAULT_DEVICE_CONFIG;
    telegramBotConfig = DEFAULT_TELEGRAM_CONFIG;
    save();

    logger.log(LOG_INFO, [&]() -> String128
               { String128 buf; buf = F("(ConfigManager::reset) All configurations reset to defaults and saved."); return buf; });
}

NetworkConfig &ConfigManager::getNetworkConfig()
{
    logger.log(LOG_DEBUG, [&]() -> String128
               { String128 buf; buf = F("(ConfigManager::getNetworkConfig) Returning NetworkConfig reference."); return buf; });
    return networkConfig;
}

DeviceLedConfig &ConfigManager::getDeviceLedConfig()
{
    logger.log(LOG_DEBUG, [&]() -> String128
               { String128 buf; buf = F("(ConfigManager::getDeviceLedConfig) Returning DeviceLedConfig reference."); return buf; });
    return deviceLedConfig;
}

TelegramBotConfig &ConfigManager::getTelegramBotConfig()
{
    logger.log(LOG_DEBUG, [&]() -> String128
               { String128 buf; buf = F("(ConfigManager::getTelegramBotConfig) Returning TelegramBotConfig reference."); return buf; });
    return telegramBotConfig;
}

void ConfigManager::saveNetworkConfig()
{
    logger.log(LOG_INFO, [&]() -> String128
               { String128 buf; buf = F("(ConfigManager::saveNetworkConfig) Saving NetworkConfig to storage..."); return buf; });
    wifiStorage.writeData(networkConfig);
}

void ConfigManager::saveDeviceLedConfig()
{
    logger.log(LOG_INFO, [&]() -> String128
               { String128 buf; buf = F("(ConfigManager::saveDeviceLedConfig) Saving DeviceLedConfig to storage..."); return buf; });
    deviceStorage.writeData(deviceLedConfig);
}

void ConfigManager::saveTelegramBotConfig()
{
    logger.log(LOG_INFO, [&]() -> String128
               { String128 buf; buf = F("(ConfigManager::saveTelegramBotConfig) Saving TelegramBotConfig to storage..."); return buf; });
    telegramStorage.writeData(telegramBotConfig);
}

void ConfigManager::resetNetworkConfig()
{
    logger.log(LOG_WARN, [&]() -> String128
               { String128 buf; buf = F("(ConfigManager::resetNetworkConfig) Resetting NetworkConfig to defaults and saving..."); return buf; });
    networkConfig = DEFAULT_NETWORK_CONFIG;
    wifiStorage.writeData(networkConfig);
}

void ConfigManager::resetDeviceLedConfig()
{
    logger.log(LOG_WARN, [&]() -> String128
               { String128 buf; buf = F("(ConfigManager::resetDeviceLedConfig) Resetting DeviceLedConfig to defaults and saving..."); return buf; });
    deviceLedConfig = DEFAULT_DEVICE_CONFIG;
    deviceStorage.writeData(deviceLedConfig);
}

void ConfigManager::resetTelegramBotConfig()
{
    logger.log(LOG_WARN, [&]() -> String128
               { String128 buf; buf = F("(ConfigManager::resetTelegramBotConfig) Resetting TelegramBotConfig to defaults and saving..."); return buf; });
    telegramBotConfig = DEFAULT_TELEGRAM_CONFIG;
    telegramStorage.writeData(telegramBotConfig);
}