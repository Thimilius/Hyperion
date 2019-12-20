#pragma once

#include <time.h>

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
        static Log *s_instance;
        LogLevel m_level;
    public:
        inline void SetLevel(LogLevel level) { m_level = level; }
        
        template<typename ...Args>
        void LogMessage(LogType type, LogLevel level, const char *system, const char *message_format, Args... args) {
            time_t current_time;
            time(&current_time);
            struct tm time_info;
            localtime_s(&time_info, &current_time);
            char prefix_buffer[30];
            strftime(prefix_buffer, sizeof(prefix_buffer), "[%H:%M:%S]", &time_info);

            String message = StringUtils::Format("{} - [{}] - {}\n", prefix_buffer, system, StringUtils::Format(message_format, args...));

            OperatingSystem::GetInstance()->PrintToConsole(GetLogColor(level), message.c_str());
        }

        inline static Log *GetInstance() { return s_instance; }
    private:
        LogColor GetLogColor(LogLevel level);
    };

}

#define HYP_LOG(s, m, ...)       Hyperion::Log::GetInstance()->LogMessage(LogType::Core, LogLevel::Trace, s, m, __VA_ARGS__)
#define HYP_LOG_TRACE(s, m, ...) Hyperion::Log::GetInstance()->LogMessage(LogType::Core, LogLevel::Trace, s, m, __VA_ARGS__)
#define HYP_LOG_INFO(s, m, ...)  Hyperion::Log::GetInstance()->LogMessage(LogType::Core, LogLevel::Info, s, m, __VA_ARGS__)
#define HYP_LOG_WARN(s, m, ...)  Hyperion::Log::GetInstance()->LogMessage(LogType::Core, LogLevel::Warning, s, m, __VA_ARGS__)
#define HYP_LOG_ERROR(s, m, ...) Hyperion::Log::GetInstance()->LogMessage(LogType::Core, LogLevel::Error, s, m, __VA_ARGS__)

#define HYP(m, ...)       Hyperion::Log::GetInstance()->LogMessage(LogType::Client, LogLevel::Trace, "Client", m, __VA_ARGS__)
#define HYP_TRACE(m, ...) Hyperion::Log::GetInstance()->LogMessage(LogType::Client, LogLevel::Trace, "Client", m, __VA_ARGS__)
#define HYP_INFO(m, ...)  Hyperion::Log::GetInstance()->LogMessage(LogType::Client, LogLevel::Info, "Client", m, __VA_ARGS__)
#define HYP_WARN(m, ...)  Hyperion::Log::GetInstance()->LogMessage(LogType::Client, LogLevel::Warning, "Client", m, __VA_ARGS__)
#define HYP_ERROR(m, ...) Hyperion::Log::GetInstance()->LogMessage(LogType::Client, LogLevel::Error, "Client", m, __VA_ARGS__)
