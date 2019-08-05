#pragma once

namespace Hyperion {

    enum class ELogColor {
        Black,
        Red,
        Green,
        Blue,
        Yellow,
        Magenta,
        Cyan,
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
        OpenGL,
        Client
    };

    class CLog {
    protected:
        static CLog *s_instance;
        ELogLevel m_level;
    public:
        const char *GetPrefixFormat(ELogType type);
        ELogColor GetLogColor(ELogLevel level);

        inline void SetLevel(ELogLevel level) { m_level = level; }

        virtual void Init() = 0;
        virtual void Log(ELogType type, ELogLevel level, const char *message, ...) = 0;

        static CLog *Get() { return s_instance; }
    };

}

#ifdef HYP_DEBUG
    #define HYP_CORE(m, ...)       Hyperion::CLog::Get()->Log(ELogType::Core, ELogLevel::Trace, m, __VA_ARGS__)
    #define HYP_CORE_TRACE(m, ...) Hyperion::CLog::Get()->Log(ELogType::Core, ELogLevel::Trace, m, __VA_ARGS__)
    #define HYP_CORE_INFO(m, ...)  Hyperion::CLog::Get()->Log(ELogType::Core, ELogLevel::Info, m, __VA_ARGS__)
    #define HYP_CORE_WARN(m, ...)  Hyperion::CLog::Get()->Log(ELogType::Core, ELogLevel::Warning, m, __VA_ARGS__)
    #define HYP_CORE_ERROR(m, ...) Hyperion::CLog::Get()->Log(ELogType::Core, ELogLevel::Error, m, __VA_ARGS__)

    #define HYP_GL(m, ...)       Hyperion::CLog::Get()->Log(ELogType::OpenGL, ELogLevel::Trace, m, __VA_ARGS__)
    #define HYP_GL_TRACE(m, ...) Hyperion::CLog::Get()->Log(ELogType::OpenGL, ELogLevel::Trace, m, __VA_ARGS__)
    #define HYP_GL_INFO(m, ...)  Hyperion::CLog::Get()->Log(ELogType::OpenGL, ELogLevel::Info, m, __VA_ARGS__)
    #define HYP_GL_WARN(m, ...)  Hyperion::CLog::Get()->Log(ELogType::OpenGL, ELogLevel::Warning, m, __VA_ARGS__)
    #define HYP_GL_ERROR(m, ...) Hyperion::CLog::Get()->Log(ELogType::OpenGL, ELogLevel::Error, m, __VA_ARGS__)

    #define HYP(m, ...)       Hyperion::CLog::Get()->Log(ELogType::Client, ELogLevel::Trace, m, __VA_ARGS__)
    #define HYP_TRACE(m, ...) Hyperion::CLog::Get()->Log(ELogType::Client, ELogLevel::Trace, m, __VA_ARGS__)
    #define HYP_INFO(m, ...)  Hyperion::CLog::Get()->Log(ELogType::Client, ELogLevel::Info, m, __VA_ARGS__)
    #define HYP_WARN(m, ...)  Hyperion::CLog::Get()->Log(ELogType::Client, ELogLevel::Warning, m, __VA_ARGS__)
    #define HYP_ERROR(m, ...) Hyperion::CLog::Get()->Log(ELogType::Client, ELogLevel::Error, m, __VA_ARGS__)
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