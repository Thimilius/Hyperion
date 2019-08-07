#include "hyppch.hpp"

#include "hyperion/system/log.hpp"

#include <time.h>

namespace Hyperion {

    CLog *CLog::s_instance = new CLog();

    void CLog::Log(ELogType type, ELogLevel level, const char *message_format, ...) {
        if (level < m_level) {
            return;
        }

        // Create prefix
        time_t current_time;
        time(&current_time);
        struct tm time_info;
        localtime_s(&time_info, &current_time);
        char prefix_buffer[30];
        int prefix_length = (int)strftime(prefix_buffer, sizeof(prefix_buffer), GetPrefixFormat(type), &time_info);

        // Compose full message
        va_list args;
        va_start(args, message_format);
        CString message = CString::Format("%s%s\n", prefix_buffer, CString::FormatArgs(message_format, args).ToCString());
        va_end(args);

        COperatingSystem::GetInstance()->PrintToConsole(GetLogColor(level), message);
    }

    const char *CLog::GetPrefixFormat(ELogType type) {
        switch (type) {
            case Hyperion::ELogType::Core: return "[%H:%M:%S] - [Core] - ";
            case Hyperion::ELogType::Client: return "[%H:%M:%S] - [Client] - ";
            default: HYP_ASSERT_ENUM_OUT_OF_RAGE; return "";
        }
    }

    ELogColor CLog::GetLogColor(ELogLevel level) {
        switch (level) {
            case Hyperion::ELogLevel::Trace: return ELogColor::White;
            case Hyperion::ELogLevel::Info: return ELogColor::Green;
            case Hyperion::ELogLevel::Warning: return ELogColor::Yellow;
            case Hyperion::ELogLevel::Error: return ELogColor::Red;
            default: HYP_ASSERT_ENUM_OUT_OF_RAGE; return ELogColor::White;
        }
    }

}