#pragma once

#include "hyperion/util/log.hpp"

#include <Windows.h>

namespace Hyperion {

    class CWindowsLog : public CLog {
    private:
        HANDLE m_console_handle;
    public:
        void Init() override;
        void Log(ELogType type, ELogLevel level, const char *message, ...) override;

        void WriteToConsole(ELogLevel level, const char *message, int message_length);
        int GetConsoleColorForLogColor(ELogColor color);
    };
}