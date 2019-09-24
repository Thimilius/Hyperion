#pragma once

#include <time.h>

#include "hyperion/core/string_utils.hpp"

namespace Hyperion {

    enum class ELogColor {
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

    enum class ELogLevel {
        Trace,
        Info,
        Warning,
        Error
    };

    enum class ELogType {
        Core,
        Client
    };

    class CLog {
    private:
        static CLog *s_instance;
        ELogLevel m_level;
    public:
        inline void SetLevel(ELogLevel level) { m_level = level; }

        template<typename ...Args>
        void Log(ELogType type, ELogLevel level, const char *system, const char *message_format, Args... args) {
            time_t current_time;
            time(&current_time);
            struct tm time_info;
            localtime_s(&time_info, &current_time);
            char prefix_buffer[30];
            strftime(prefix_buffer, sizeof(prefix_buffer), "[%H:%M:%S]", &time_info);

            TString message = CStringUtils::Format("{} - [{}] - {}\n", prefix_buffer, system, CStringUtils::Format(message_format, args...));

            COperatingSystem::GetInstance()->PrintToConsole(GetLogColor(level), message.c_str());
        }

        inline static CLog *GetInstance() { return s_instance; }
    private:
        ELogColor GetLogColor(ELogLevel level);
    };

}

#define HYP_LOG(s, m, ...)       Hyperion::CLog::GetInstance()->Log(ELogType::Core, ELogLevel::Trace, s, m, __VA_ARGS__)
#define HYP_LOG_TRACE(s, m, ...) Hyperion::CLog::GetInstance()->Log(ELogType::Core, ELogLevel::Trace, s, m, __VA_ARGS__)
#define HYP_LOG_INFO(s, m, ...)  Hyperion::CLog::GetInstance()->Log(ELogType::Core, ELogLevel::Info, s, m, __VA_ARGS__)
#define HYP_LOG_WARN(s, m, ...)  Hyperion::CLog::GetInstance()->Log(ELogType::Core, ELogLevel::Warning, s, m, __VA_ARGS__)
#define HYP_LOG_ERROR(s, m, ...) Hyperion::CLog::GetInstance()->Log(ELogType::Core, ELogLevel::Error, s, m, __VA_ARGS__)

#define HYP(m, ...)       Hyperion::CLog::GetInstance()->Log(ELogType::Client, ELogLevel::Trace, m, "Client", __VA_ARGS__)
#define HYP_TRACE(m, ...) Hyperion::CLog::GetInstance()->Log(ELogType::Client, ELogLevel::Trace, m, "Client", __VA_ARGS__)
#define HYP_INFO(m, ...)  Hyperion::CLog::GetInstance()->Log(ELogType::Client, ELogLevel::Info, m, "Client", __VA_ARGS__)
#define HYP_WARN(m, ...)  Hyperion::CLog::GetInstance()->Log(ELogType::Client, ELogLevel::Warning, m, "Client", __VA_ARGS__)
#define HYP_ERROR(m, ...) Hyperion::CLog::GetInstance()->Log(ELogType::Client, ELogLevel::Error, m, "Client", __VA_ARGS__)
