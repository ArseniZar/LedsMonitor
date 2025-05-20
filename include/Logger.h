#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>

enum LogLevel {
  LOG_INFO,
  LOG_WARN,
  LOG_ERROR
};

class Logger {
public:
  Logger();

  void log(const String& message, LogLevel level = LOG_INFO);
  void clear();
  const String& getLog() const;
  void printToSerial() const;

private:
  String _log;

  String levelToString(LogLevel level) const;
};

#endif
