#pragma once

#include "hyperion/core/operating_system.hpp"

#include <Windows.h>

namespace Hyperion {

    class WindowsOperatingSystem : public OperatingSystem {
    public:
        void Init() override;

        inline OperatingSystemType GetType() const override { return OperatingSystemType::Windows; }
        SystemInfo GetSystemInfo() const override;
        u64 GetMemoryUsage() const override;
        SystemLanguage GetSystemLanguage() const override;
        String GetSystemFolder(SystemFolder system_folder) const override;

        void DisplayError(const String &title, const String &message) override;
        void PrintToConsole(LogColor color, const String &message) override;

        // TODO: Make the filter work
        String OpenFileDialog(const String &title, const String &filter) override;
        String SaveFileDialog(const String &title, const String &filter) override;
    private:
        HANDLE m_console_handle;
    };

}