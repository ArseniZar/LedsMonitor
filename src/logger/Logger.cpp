#include "Logger.h"

Logger &Logger::init(bool debugMode)
{
  static Logger instance(debugMode);
  return instance;
}

Logger::Logger(bool debugMode) : debugMode(debugMode) {}

void Logger::log(LogLevel level, std::function<String256()> messageGenerator) const
{
  if (!debugMode && level == LOG_DEBUG)
    return;

  Serial.print(levelToString(level));
  Serial.print(messageGenerator());
  Serial.print("\n");
}

const __FlashStringHelper *Logger::levelToString(LogLevel level) const
{
  switch (level)
  {
  case LOG_INFO:
    return F("[INFO] ");
  case LOG_WARN:
    return F("[WARN] ");
  case LOG_ERROR:
    return F("[ERROR] ");
  case LOG_DEBUG:
    return F("[DEBUG] ");
  default:
    return F("[UNKNOWN] ");
  }
}
