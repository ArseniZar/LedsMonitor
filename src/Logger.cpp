#include "Logger.h"

Logger::Logger() {}

void Logger::log(const String& message, LogLevel level) {
  String prefix = levelToString(level);
  _log += prefix + message + "\n";
  Serial.println(prefix + message + "\n");
}

void Logger::clear() {
  _log = "";
}

const String& Logger::getLog() const {
  return _log;
}

void Logger::printToSerial() const {
  Serial.print(_log);
}

String Logger::levelToString(LogLevel level) const {
  switch (level) {
    case LOG_INFO: return "[INFO] ";
    case LOG_WARN: return "[WARN] ";
    case LOG_ERROR: return "[ERROR] ";
    default: return "[UNKNOWN] ";
  }
}
