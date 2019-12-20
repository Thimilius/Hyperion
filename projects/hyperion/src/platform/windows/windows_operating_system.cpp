#include "hyppch.hpp"

#include "hyperion/platform/windows/windows_operating_system.hpp"

#include <Powrprof.h>

namespace Hyperion {

    // Apparently microsoft forgot to include this definition in their header files. What the hell!?!?
    typedef struct _PROCESSOR_POWER_INFORMATION {
        ULONG Number;
        ULONG MaxMhz;
        ULONG CurrentMhz;
        ULONG MhzLimit;
        ULONG MaxIdleState;
        ULONG CurrentIdleState;
    } PROCESSOR_POWER_INFORMATION, *PPROCESSOR_POWER_INFORMATION;

    OperatingSystem* OperatingSystem::s_instance = new WindowsOperatingSystem();

    void WindowsOperatingSystem::Init() {
        // Initialize console
        m_console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
        if (m_console_handle == NULL) {
            AllocConsole();
            m_console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
        }
    }

    OperatingSystemType WindowsOperatingSystem::GetType() const {
        return OperatingSystemType::Windows;
    }

    SystemInfo WindowsOperatingSystem::GetSystemInfo() const {
        SystemInfo result;

        // Gather processor info
        SYSTEM_INFO system_info;
        GetNativeSystemInfo(&system_info);
        u32 processor_count = system_info.dwNumberOfProcessors;
        PROCESSOR_POWER_INFORMATION *processor_info = new PROCESSOR_POWER_INFORMATION[processor_count];
        CallNtPowerInformation(ProcessorInformation, NULL, 0, processor_info, (u32)(sizeof(*processor_info) * processor_count));
        result.processor_info.processor_count = processor_count;
        result.processor_info.processor_mhz_frequency = processor_info[0].MaxMhz;
        delete[] processor_info;

        // Gather memory info
        MEMORYSTATUSEX memory_status;
        memory_status.dwLength = sizeof(MEMORYSTATUSEX);
        GlobalMemoryStatusEx(&memory_status);
        result.memory_info.available_physical_memory = memory_status.ullAvailPhys;
        result.memory_info.total_physical_memory = memory_status.ullTotalPhys;
        result.memory_info.available_virtual_memory = memory_status.ullAvailVirtual;
        result.memory_info.total_virtual_memory = memory_status.ullTotalVirtual;

        return result;
    }

    void WindowsOperatingSystem::DisplayError(const String &title, const String &message) const {
        MessageBoxW(nullptr, StringUtils::Utf8ToUtf16(message).c_str(), StringUtils::Utf8ToUtf16(title).c_str(), MB_OK | MB_ICONERROR);
    }

    void WindowsOperatingSystem::PrintToConsole(LogColor color, const String &message) const {
        s16 console_color = 0;
        switch (color) {
            case Hyperion::LogColor::Black: console_color = 0; break;
            case Hyperion::LogColor::Red: console_color = FOREGROUND_INTENSITY | FOREGROUND_RED; break;
            case Hyperion::LogColor::Green: console_color = FOREGROUND_INTENSITY | FOREGROUND_GREEN; break;
            case Hyperion::LogColor::Blue: console_color = FOREGROUND_INTENSITY | FOREGROUND_BLUE; break;
            case Hyperion::LogColor::Yellow: console_color = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN; break;
            case Hyperion::LogColor::Magenta: console_color = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE; break;
            case Hyperion::LogColor::Cyan: console_color = FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN; break;
            case Hyperion::LogColor::DarkRed: console_color = FOREGROUND_RED; break;
            case Hyperion::LogColor::DarkGreen: console_color = FOREGROUND_GREEN; break;
            case Hyperion::LogColor::DarkBlue: console_color = FOREGROUND_BLUE; break;
            case Hyperion::LogColor::DarkYellow: console_color = FOREGROUND_RED | FOREGROUND_GREEN; break;
            case Hyperion::LogColor::DarkMagenta: console_color = FOREGROUND_RED | FOREGROUND_BLUE; break;
            case Hyperion::LogColor::DarkCyan: console_color = FOREGROUND_BLUE | FOREGROUND_GREEN; break;
            case Hyperion::LogColor::Grey: console_color = FOREGROUND_INTENSITY; break;
            case Hyperion::LogColor::White: console_color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; break;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }

        CONSOLE_SCREEN_BUFFER_INFO console_screen_buffer_info;
        GetConsoleScreenBufferInfo(m_console_handle, &console_screen_buffer_info);
        SetConsoleTextAttribute(m_console_handle, console_color);
        unsigned long written_chars = 0;
        WriteConsoleA(m_console_handle, message.c_str(), (DWORD)message.length(), &written_chars, nullptr);
        SetConsoleTextAttribute(m_console_handle, console_screen_buffer_info.wAttributes);
    }

}