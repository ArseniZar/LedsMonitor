#pragma once
#include "ConfigManager.h"

template <typename Config, typename RuntimeConfig>
ConfigPair<Config, RuntimeConfig>::ConfigPair() : config(Config::fromDefault()), storage(RuntimeConfig::PATH, RuntimeConfig::KEY) {}

template <typename... Configs, typename... RuntimeConfigs>
ConfigManager<ConfigPair<Configs, RuntimeConfigs>...> &ConfigManager<ConfigPair<Configs, RuntimeConfigs>...>::init(Logger &logger)
{
    static ConfigManager instance(logger);
    return instance;
}

template <typename... Configs, typename... RuntimeConfigs>
ConfigManager<ConfigPair<Configs, RuntimeConfigs>...>::ConfigManager(Logger &logger) : logger(logger), configs() {}

template <typename... Configs, typename... RuntimeConfigs>
void ConfigManager<ConfigPair<Configs, RuntimeConfigs>...>::begin()
{
    logger.log(LOG_INFO, [&]() -> String128
               { String128 buf; buf = F("(ConfigManager::begin) Initializing configuration storage..."); return buf; });

    std::apply([](auto &...configPairs)
               { (([](auto &configPair)
                   { configPair.storage.begin(); }(configPairs)),
                  ...); }, configs);
    load();

    logger.log(LOG_INFO, [&]() -> String128
               { String128 buf; buf = F("(ConfigManager::begin) Configuration initialized."); return buf; });
}

template <typename... Configs, typename... RuntimeConfigs>
void ConfigManager<ConfigPair<Configs, RuntimeConfigs>...>::load()
{
    std::apply([this](auto &...configs)
               { (([this](auto &configPair)
                   {
                        configPair.config = configPair.storage.readData();
                        if (!configPair.storage.isLoaded())
                        {
                            configPair.config = decltype(configPair.config)::fromDefault();
                            configPair.storage.writeData(configPair.config);
                            logger.log(LOG_WARN, [&]() -> String256
                                        { String256 buf; buf = F("(ConfigManager::load) No stored "); buf.add(decltype(configPair.config)::TYPE_NAME); buf.add(" found; using defaults and saving."); return buf; });
                        }
                        logger.log(LOG_INFO, [&]() -> String256
                                    { String256 buf; buf = F("(ConfigManager::load) Loaded "); buf.add(decltype(configPair.config)::TYPE_NAME); buf.add(" NetworkConfig from storage."); return buf; }); }(configs)),
                  ...); }, configs);
}

template <typename... Configs, typename... RuntimeConfigs>
void ConfigManager<ConfigPair<Configs, RuntimeConfigs>...>::save()
{
    // logger.log(LOG_INFO, [&]() -> String128
    //            { String128 buf; buf = F("(ConfigManager::save) Saving all configuration sections to storage..."); return buf; });

    std::apply([this](auto &...configs)
               { (([this](auto &configPair)
                   { if(!(configPair.config == configPair.storage.getData())) 
                    {
                        configPair.storage.writeData(configPair.config);
                        logger.log(LOG_INFO, [&]() -> String256
                                            { String256 buf; buf = F("(ConfigManager::saveConfig) Saving"); buf.add(decltype(configPair.config)::TYPE_NAME); buf.add(" to storage..."); return buf; });
                    }
                    else
                    {
                        logger.log(LOG_DEBUG, [&]() -> String256
                                    { String256 buf; buf = F("(ConfigManager::save) No changes in "); buf.add(decltype(configPair.config)::TYPE_NAME); buf.add("; skipping save."); return buf; }); 
                    } }(configs)),
                  ...); }, configs);

    // logger.log(LOG_DEBUG, [&]() -> String128
    //            { String128 buf; buf = F("(ConfigManager::save) All config saved."); return buf; });
}

template <typename... Configs, typename... RuntimeConfigs>
void ConfigManager<ConfigPair<Configs, RuntimeConfigs>...>::reset()
{
    std::apply([](auto &...configs)
               { (([](auto &configPair)
                   { configPair.config = decltype(configPair.config)::fromDefault(); }(configs)),
                  ...); }, configs);

    logger.log(LOG_WARN, [&]() -> String128
               { String128 buf; buf = F("(ConfigManager::reset) All configuration sections reset to defaults."); return buf; });
}

template <typename... Configs, typename... RuntimeConfigs>
template <typename Config>
const Config &ConfigManager<ConfigPair<Configs, RuntimeConfigs>...>::getConfig() const
{
    static_assert(has_type_in_tuple_with_pair<Config, decltype(configs)>::value, "(ConfigManager::getConfig): Config must be one of the Config types in ConfigManager");

    const Config *result = nullptr;
    std::apply([&result](auto &...configs)
               { (([&result](auto &configPair)
                   {
                        if constexpr (std::is_same_v<decltype(configPair.config), Config>)
                        {
                            if (!result)
                            {
                                result = &configPair.config;
                            }
                        } }(configs)),
                  ...); }, configs);

    return *result;
}

template <typename... Configs, typename... RuntimeConfigs>
template <typename Config>
void ConfigManager<ConfigPair<Configs, RuntimeConfigs>...>::updateConfig(const Config &config)
{
    static_assert(has_type_in_tuple_with_pair<Config, decltype(configs)>::value, "(ConfigManager::updateConfig): Config must be one of the Config types in ConfigManager");

    std::apply([this, &config](auto &...configs)
               { (([this, &config](auto &configPair)
                   {
                        if constexpr (std::is_same_v<decltype(configPair.config), Config>)
                        {
                            configPair.config = config;
                            logger.log(LOG_DEBUG, [&]() -> String256
                                        { String256 buf; buf = F("(ConfigManager::updateConfig) Updated "); buf.add(decltype(configPair.config)::TYPE_NAME); buf.add(" from argument."); return buf; });
                        } }(configs)),
                  ...); }, configs);
}

template <typename... Configs, typename... RuntimeConfigs>
template <typename Config>
void ConfigManager<ConfigPair<Configs, RuntimeConfigs>...>::saveConfig()
{
    static_assert(has_type_in_tuple_with_pair<Config, decltype(configs)>::value, "(ConfigManager::saveConfig): Config must be one of the Config types in ConfigManager");
    std::apply([this](auto &...configs)
               { (([this](auto &configPair)
                   {
                        if constexpr (std::is_same_v<decltype(configPair.config), Config>)
                        {   
                            if(!(configPair.config == configPair.storage.getData())) 
                            {
                                configPair.storage.writeData(configPair.config);
                                logger.log(LOG_INFO, [&]() -> String256
                                            { String256 buf; buf = F("(ConfigManager::saveConfig) Saving "); buf.add(decltype(configPair.config)::TYPE_NAME); buf.add(" to storage..."); return buf; });
                            }
                            else
                            {
                                logger.log(LOG_DEBUG, [&]() -> String256
                                            { String256 buf; buf = F("(ConfigManager::saveConfig) No changes in "); buf.add(decltype(configPair.config)::TYPE_NAME); buf.add("; skipping save."); return buf; }); 
                            }
                        } }(configs)),
                  ...); }, configs);
}

template <typename... Configs, typename... RuntimeConfigs>
template <typename Config>
void ConfigManager<ConfigPair<Configs, RuntimeConfigs>...>::resetConfig()
{
    static_assert(has_type_in_tuple_with_pair<Config, decltype(configs)>::value, "(ConfigManager::resetConfig): Config must be one of the Config types in ConfigManager");
    std::apply([this](auto &...configs)
               { (([this](auto &configPair)
                   {
                           if constexpr (std::is_same_v<decltype(configPair.config), Config>)
                           {
                               configPair.config = decltype(configPair.config)::fromDefault();
                               logger.log(LOG_WARN, [&]() -> String256
                                          { String256 buf; buf = F("(ConfigManager::resetConfig) Resetting "); buf.add(decltype(configPair.config)::TYPE_NAME); buf.add(" to defaults"); return buf; });
                           } }(configs)),
                  ...); }, configs);
}