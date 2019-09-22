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
        void Log(ELogType type, ELogLevel level, const char *format, Args... args) {
            time_t current_time;
            time(&current_time);
            struct tm time_info;
            localtime_s(&time_info, &current_time);
            char prefix_buffer[30];
            int prefix_length = (int)strftime(prefix_buffer, sizeof(prefix_buffer), GetPrefixFormat(type), &time_info);

            TString message = CStringUtils::Format("{}{}\n", prefix_buffer, CStringUtils::Format(format, args...));

            COperatingSystem::GetInstance()->PrintToConsole(GetLogColor(level), message.c_str());
        }

        inline static CLog *GetInstance() { return s_instance; }
    private:
        const char *GetPrefixFormat(ELogType type);

        ELogColor GetLogColor(ELogLevel level);
    };

}

#ifdef HYP_DEBUG
    #define HYP_CORE(m, ...)       Hyperion::CLog::GetInstance()->Log(ELogType::Core, ELogLevel::Trace, m, __VA_ARGS__)
    #define HYP_CORE_TRACE(m, ...) Hyperion::CLog::GetInstance()->Log(ELogType::Core, ELogLevel::Trace, m, __VA_ARGS__)
    #define HYP_CORE_INFO(m, ...)  Hyperion::CLog::GetInstance()->Log(ELogType::Core, ELogLevel::Info, m, __VA_ARGS__)
    #define HYP_CORE_WARN(m, ...)  Hyperion::CLog::GetInstance()->Log(ELogType::Core, ELogLevel::Warning, m, __VA_ARGS__)
    #define HYP_CORE_ERROR(m, ...) Hyperion::CLog::GetInstance()->Log(ELogType::Core, ELogLevel::Error, m, __VA_ARGS__)

    #define HYP(m, ...)       Hyperion::CLog::GetInstance()->Log(ELogType::Client, ELogLevel::Trace, m, __VA_ARGS__)
    #define HYP_TRACE(m, ...) Hyperion::CLog::GetInstance()->Log(ELogType::Client, ELogLevel::Trace, m, __VA_ARGS__)
    #define HYP_INFO(m, ...)  Hyperion::CLog::GetInstance()->Log(ELogType::Client, ELogLevel::Info, m, __VA_ARGS__)
    #define HYP_WARN(m, ...)  Hyperion::CLog::GetInstance()->Log(ELogType::Client, ELogLevel::Warning, m, __VA_ARGS__)
    #define HYP_ERROR(m, ...) Hyperion::CLog::GetInstance()->Log(ELogType::Client, ELogLevel::Error, m, __VA_ARGS__)
#else
    #define LOG(m, ...)      
    #define LOG_TRACE(m, ...)
    #define LOG_INFO(m, ...) 
    #define LOG_WARN(m, ...) 
    #define LOG_ERROR(m, ...)

    #define LOG_GL(m, ...)      
    #define LOG_GL_TRACE(m, ...)
    #define LOG_GL_INFO(m, ...) 
    #define LOG_GL_WARN(m, ...) 
    #define LOG_GL_ERROR(m, ...)
#endif