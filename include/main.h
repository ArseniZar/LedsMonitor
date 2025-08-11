#pragma once
#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include "config.h"
#include "Logger.h"
#include "Eeprom.h"
#include "WiFiSetup.h"
#include "TelegramBot.h"
#include "LedController.h"

#ifndef LOGGER_DEBUG_MODE
#define LOGGER_DEBUG_MODE 1
#endif

// unsigned long previousMillis = 0; // хранит время последней проверки
// const unsigned long interval = 5000;
// void updateCredentialsIfChanged();
// void wifiTick();

#endif // MAIN_H
