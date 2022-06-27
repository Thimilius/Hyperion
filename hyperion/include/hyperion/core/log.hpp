//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/string_utils.hpp"
#include "hyperion/core/system/operating_system_types.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  enum class LogLevel {
    Trace,
    Info,
    Warning,
    Error
  };

  enum class LogType {
    Engine,
    Client
  };

  using LogCallback = std::function<void(LogType, LogLevel, const char *, const String &)>;

  class ILogger {
  public:
    virtual ~ILogger() = default;
  public:
    virtual void Log(LogType type, LogLevel level, const String &message) = 0;
  };

  class ConsoleLogger final : public ILogger {
  public:
    void Log(LogType type, LogLevel level, const String &message) override;
  private:
    static ConsoleColor GetLogColor(LogLevel level);
  };
  
  class Log final {
  public:
    inline static void RegisterLogger(ILogger *logger) {
      s_loggers.Add(logger);
    }

    inline static void UnregisterLogger(ILogger *logger) {
      s_loggers.Remove(logger);
    }

    template<typename ...Args>
    static void LogMessage(LogType type, LogLevel level, const char *system, const String &message_format, Args ... args) {
      String formatted_message = StringUtils::Format(message_format, args...);
      LogMessageInternal(type, level, system, formatted_message);
    }
  private:
    Log() = delete;
    ~Log() = delete;
  private:
    static void LogMessageInternal(LogType type, LogLevel level, const char *system, const String &formatted_message);
  private:
    inline static Array<ILogger *> s_loggers;
  };

}

#define HYP_LOG_TRACE(s, m, ...) Hyperion::Log::LogMessage(LogType::Engine, LogLevel::Trace, s, m, ##__VA_ARGS__)
#define HYP_LOG_INFO(s, m, ...)  Hyperion::Log::LogMessage(LogType::Engine, LogLevel::Info, s, m, ##__VA_ARGS__)
#define HYP_LOG_WARN(s, m, ...)  Hyperion::Log::LogMessage(LogType::Engine, LogLevel::Warning, s, m, ##__VA_ARGS__)
#define HYP_LOG_ERROR(s, m, ...) Hyperion::Log::LogMessage(LogType::Engine, LogLevel::Error, s, m, ##__VA_ARGS__)

#define HYP_TRACE(m, ...) Hyperion::Log::LogMessage(LogType::Client, LogLevel::Trace, "Client", m, ##__VA_ARGS__)
#define HYP_INFO(m, ...)  Hyperion::Log::LogMessage(LogType::Client, LogLevel::Info, "Client", m, ##__VA_ARGS__)
#define HYP_WARN(m, ...)  Hyperion::Log::LogMessage(LogType::Client, LogLevel::Warning, "Client", m, ##__VA_ARGS__)
#define HYP_ERROR(m, ...) Hyperion::Log::LogMessage(LogType::Client, LogLevel::Error, "Client", m, ##__VA_ARGS__)
