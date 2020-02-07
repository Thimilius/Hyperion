#pragma once

#include "hyperion/core/operating_system.hpp"

#include <Windows.h>

namespace Hyperion {

    class WindowsOperatingSystem : public OperatingSystem {
    private:
        HANDLE m_console_handle;
    public:
        void Init() override;

        inline OperatingSystemType GetType() const override { return OperatingSystemType::Windows; }
        SystemInfo GetSystemInfo() const override;
        SystemLanguage GetSystemLanguage() const override;

        void DisplayError(const String &title, const String &message) override;
        void PrintToConsole(LogColor color, const String &message) override;

        // TODO: Make the filter work
        String OpenFileDialog(const String &title, const String &filter) override;
        String SaveFileDialog(const String &title, const String &filter) override;
    };

}