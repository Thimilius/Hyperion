#pragma once

//---------------------- Library Includes ----------------------
#include <Windows.h>

//---------------------- Project Includes ----------------------
#include "hyperion/core/system/operating_system.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class WindowsOperatingSystem : public OperatingSystem {
    public:
        void Initialize() override;

        inline OperatingSystemType GetType() const override { return OperatingSystemType::Windows; }
        SystemInfo GetSystemInfo() const override;
        uint64 GetMemoryUsage() const override;
        SystemLanguage GetSystemLanguage() const override;
        String GetSystemFolder(SystemFolder system_folder) const override;

        void DisplayError(const String &title, const String &message) override;
        void PrintToConsole(LogColor color, const String &message) override;

        // TODO: Make the filter work.
        String OpenFileDialog(const String &title, const String &filter) override;
        String SaveFileDialog(const String &title, const String &filter) override;
    private:
        HANDLE m_console_handle;
    };

}