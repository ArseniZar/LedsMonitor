#pragma once
#ifndef CONFIG_DEFAULT_H
#define CONFIG_DEFAULT_H

#include "NetworkConfigModels.h"
#include "DeviceLedConfigModels.h"
#include "TelegramBotConfigModels.h"
#include "WebServerConfigModels.h"

extern const NetworkConfig& DEFAULT_NETWORK_CONFIG;
extern const DeviceLedConfig& DEFAULT_DEVICE_CONFIG;
extern const TelegramBotConfig& DEFAULT_TELEGRAM_CONFIG;
extern const WebServerConfig& DEFAULT_WEBSERVER_CONFIG;

#endif // CONFIG_DEFAULT_H