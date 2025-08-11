#pragma once
#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>

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
  void clear();
  const String &getLog() const;
  void printToSerial() const;
  static Logger &init(bool debugMode);
  void log(const String &message, LogLevel level = LOG_INFO);

private:
  bool debugMode;
  String logBuffer;

  Logger(bool debugMode = true);
  Logger(const Logger &) = default;
  Logger(Logger &&) = default;
  Logger &operator=(const Logger &) = default;
  Logger &operator=(Logger &&) = default;

  String levelToString(LogLevel level) const;
};

#endif
