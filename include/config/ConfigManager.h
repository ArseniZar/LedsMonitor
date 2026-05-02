#pragma once
#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include "Logger.h"
#include "Storage.h"
#include "utils/helpers.h"

template <typename Config, typename RuntimeConfig>
struct ConfigPair
{
    Config config;
    Storage<RuntimeConfig> storage;
    ConfigPair();
};

template <typename... ConfigPairs>
class ConfigManager
    { static_assert(true, "(ConfigManager) Template parameters must be ConfigPair<Config, RuntimeConfig> (wrap your config types with ConfigPair)."); };

template <typename... Configs, typename... RuntimeConfigs>
class ConfigManager<ConfigPair<Configs, RuntimeConfigs>...>
{
public:
    void begin();
    void save();
    void reset();

    template <typename Config>
    const Config &getConfig() const;

    template <typename Config>
    void updateConfig(const Config &config);

    template <typename Config>
    void saveConfig();

    template <typename Config>
    void resetConfig();

    static ConfigManager &init(Logger &logger);

private:
    ConfigManager(Logger &logger);

    Logger &logger;
    std::tuple<ConfigPair<Configs, RuntimeConfigs>...> configs;

    void load();
};

#include "ConfigManager.tpp"
#endif