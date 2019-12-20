#pragma once

#include "hyperion/core/operating_system.hpp"

#include <Windows.h>

namespace Hyperion {

    class WindowsOperatingSystem : public OperatingSystem {
    private:
        HANDLE m_console_handle;
    public:
        void Init() override;

        OperatingSystemType GetType() const override;
        SystemInfo GetSystemInfo() const override;

        void DisplayError(const String &title, const String &message) const override;

        void PrintToConsole(LogColor color, const String &message) const override;
    };

}