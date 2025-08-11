#include "Logger.h"

Logger &Logger::init(bool debugMode)
{
  static Logger instance(debugMode);
  return instance;
}

Logger::Logger(bool debugMode) : debugMode(debugMode) {}

void Logger::log(const String &message, LogLevel level)
{
  if (!debugMode && level == LOG_DEBUG)
  {
    return;
  }
  String prefix = levelToString(level);
  logBuffer += prefix + message + "\n";
  Serial.println(prefix + message + "\n");
}

void Logger::clear()
{
  logBuffer = "";
}

const String &Logger::getLog() const
{
  return logBuffer;
}

void Logger::printToSerial() const
{
  Serial.print(logBuffer);
}

String Logger::levelToString(LogLevel level) const
{
  switch (level)
  {
  case LOG_INFO:
    return "[INFO] ";
  case LOG_WARN:
    return "[WARN] ";
  case LOG_ERROR:
    return "[ERROR] ";
  case LOG_DEBUG:
    return "[DEBUG] ";
  default:
    return "[UNKNOWN] ";
  }
}
