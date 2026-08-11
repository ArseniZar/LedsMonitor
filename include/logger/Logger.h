#pragma once
#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>
#include <StringN.h>

enum LogLevel
{
  LOG_INFO,
  LOG_WARN,
  LOG_ERROR,
  LOG_DEBUG,
};

class Logger
{
public:
  static Logger &init(bool debugMode);
  void log(LogLevel level, std::function<String256()> messageGenerator) const;

private:
  bool debugMode;

  Logger(bool debugMode);
  Logger() = delete;
  const __FlashStringHelper *levelToString(LogLevel level) const;
};

#endif
