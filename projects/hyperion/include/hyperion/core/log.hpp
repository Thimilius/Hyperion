#pragma once

#include <ctime>

#include "hyperion/core/string_utils.hpp"

namespace Hyperion {

    enum class LogColor {
        Black,
        
        Red,
        Green,
        Blue,

        Yellow,
        Magenta,
        Cyan,

        DarkRed,
        DarkGreen,
        DarkBlue,

        DarkYellow,
        DarkMagenta,
        DarkCyan,

        Grey,
        White
    };

    enum class LogLevel {
        Trace,
        Info,
        Warning,
        Error
    };

    enum class LogType {
        Core,
        Client
    };

    class Log {
    private:
        static inline LogLevel m_level;
    public:
        static inline void SetLevel(LogLevel level) { m_level = level; }
        
        template<typename ...Args>
        static void LogMessage(LogType type, LogLevel level, const char *system, const char *message_format, Args... args) {
            std::time_t current_time = std::time(&current_time);
            std::tm *time_info = std::localtime(&current_time);
            char prefix_buffer[30];
            strftime(prefix_buffer, sizeof(prefix_buffer), "[%H:%M:%S]", time_info);

            String message = StringUtils::Format("{} - [{}] - {}\n", prefix_buffer, system, StringUtils::Format(message_format, args...));

            LogMessageInternal(level, message);
        }
    private:
        static void LogMessageInternal(LogLevel level, String message);

        static LogColor GetLogColor(LogLevel level);
    };

}

#define HYP_LOG(s, m, ...)       Hyperion::Log::LogMessage(LogType::Core, LogLevel::Trace, s, m, __VA_ARGS__)
#define HYP_LOG_TRACE(s, m, ...) Hyperion::Log::LogMessage(LogType::Core, LogLevel::Trace, s, m, __VA_ARGS__)
#define HYP_LOG_INFO(s, m, ...)  Hyperion::Log::LogMessage(LogType::Core, LogLevel::Info, s, m, __VA_ARGS__)
#define HYP_LOG_WARN(s, m, ...)  Hyperion::Log::LogMessage(LogType::Core, LogLevel::Warning, s, m, __VA_ARGS__)
#define HYP_LOG_ERROR(s, m, ...) Hyperion::Log::LogMessage(LogType::Core, LogLevel::Error, s, m, __VA_ARGS__)

#define HYP(m, ...)       Hyperion::Log::LogMessage(LogType::Client, LogLevel::Trace, "Client", m, __VA_ARGS__)
#define HYP_TRACE(m, ...) Hyperion::Log::LogMessage(LogType::Client, LogLevel::Trace, "Client", m, __VA_ARGS__)
#define HYP_INFO(m, ...)  Hyperion::Log::LogMessage(LogType::Client, LogLevel::Info, "Client", m, __VA_ARGS__)
#define HYP_WARN(m, ...)  Hyperion::Log::LogMessage(LogType::Client, LogLevel::Warning, "Client", m, __VA_ARGS__)
#define HYP_ERROR(m, ...) Hyperion::Log::LogMessage(LogType::Client, LogLevel::Error, "Client", m, __VA_ARGS__)
