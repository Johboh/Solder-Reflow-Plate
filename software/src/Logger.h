#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <Arduino.h>
#include <functional>

/**
 * Take care of logging and errors.
 */
class Logger {
public:
  Logger() {}

  enum class Severity {
    Info,
    Warning,
    Error,
  };

  /**
   * @brief Log a message given a Severity.
   */
  void log(Severity severity, const char message[]) {
    if (_on_log != nullptr) {
      _on_log(severity, message);
    }
    switch (severity) {
    case Severity::Info:
      Serial.print("INFO: ");
      break;
    case Severity::Warning:
      Serial.print("WARNING: ");
      break;
    case Severity::Error:
      Serial.print("ERROR: ");
      break;
    }
    Serial.println(String(message));
  }

  /**
   * @brief Register as a callback for log messages.
   */
  void setCallback(std::function<void(Severity severity, const char message[])> on_log) { _on_log = on_log; }

private:
  std::function<void(Severity severity, const char message[])> _on_log = nullptr;
};
#endif //__LOGGER_H__