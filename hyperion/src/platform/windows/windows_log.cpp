#include "hyppch.hpp"

#include "windows_log.hpp"

#include <time.h>

namespace Hyperion {

    CLog *CLog::s_instance = new CWindowsLog();

    void CWindowsLog::Init() {
        m_console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
        if (m_console_handle == NULL) {
            AllocConsole();
            m_console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
        }
    }

    void CWindowsLog::Log(ELogType type, ELogLevel level, const char *message_format, ...) {
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
        CString string = CString("%s%s\n", prefix_buffer, CString::FromArgs(message_format, args).ToCString());
        va_end(args);

        WriteToConsole(level, string.ToCString(), string.GetLength());
    }

    void CWindowsLog::WriteToConsole(ELogLevel level, const char *message, int messsage_length) {
        CONSOLE_SCREEN_BUFFER_INFO console_screen_buffer_info;
        GetConsoleScreenBufferInfo(m_console_handle, &console_screen_buffer_info);
        SetConsoleTextAttribute(m_console_handle, GetConsoleColorForLogColor(GetLogColor(level)));
        unsigned long written_chars = 0;
        WriteConsoleA(m_console_handle, message, messsage_length, &written_chars, nullptr);
        SetConsoleTextAttribute(m_console_handle, console_screen_buffer_info.wAttributes);
    }

    int CWindowsLog::GetConsoleColorForLogColor(ELogColor color) {
        int result = 0;

        switch (color) {
            case Hyperion::ELogColor::Black: break;
            case Hyperion::ELogColor::Red: result = FOREGROUND_RED; break;
            case Hyperion::ELogColor::Green: result = FOREGROUND_GREEN; break;
            case Hyperion::ELogColor::Blue: result = FOREGROUND_BLUE; break;
            case Hyperion::ELogColor::Yellow: result = FOREGROUND_RED | FOREGROUND_GREEN; break;
            case Hyperion::ELogColor::Magenta: result = FOREGROUND_RED | FOREGROUND_BLUE; break;
            case Hyperion::ELogColor::Cyan: result = FOREGROUND_BLUE | FOREGROUND_GREEN; break;
            case Hyperion::ELogColor::White: result = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; break;
            default: HYP_ASSERT(false);
        }

        return result | FOREGROUND_INTENSITY;
    }
}