#include "hyppch.hpp"

#include "windows_operating_system.hpp"

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

    COperatingSystem* COperatingSystem::s_instance = new CWindowsOperatingSystem();

    void CWindowsOperatingSystem::Init() {
        // Initialize console
        m_console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
        if (m_console_handle == NULL) {
            AllocConsole();
            m_console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
        }
    }

    EOperatingSystemType CWindowsOperatingSystem::GetType() const {
        return EOperatingSystemType::Windows;
    }

    SSystemInfo CWindowsOperatingSystem::GetSystemInfo() const {
        SSystemInfo result;

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

    void CWindowsOperatingSystem::PrintToConsole(ELogColor color, const TString &message) const {
        s16 console_color = 0;
        switch (color) {
            case Hyperion::ELogColor::Black: console_color = 0; break;
            case Hyperion::ELogColor::Red: console_color = FOREGROUND_INTENSITY | FOREGROUND_RED; break;
            case Hyperion::ELogColor::Green: console_color = FOREGROUND_INTENSITY | FOREGROUND_GREEN; break;
            case Hyperion::ELogColor::Blue: console_color = FOREGROUND_INTENSITY | FOREGROUND_BLUE; break;
            case Hyperion::ELogColor::Yellow: console_color = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN; break;
            case Hyperion::ELogColor::Magenta: console_color = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE; break;
            case Hyperion::ELogColor::Cyan: console_color = FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN; break;
            case Hyperion::ELogColor::DarkRed: console_color = FOREGROUND_RED; break;
            case Hyperion::ELogColor::DarkGreen: console_color = FOREGROUND_GREEN; break;
            case Hyperion::ELogColor::DarkBlue: console_color = FOREGROUND_BLUE; break;
            case Hyperion::ELogColor::DarkYellow: console_color = FOREGROUND_RED | FOREGROUND_GREEN; break;
            case Hyperion::ELogColor::DarkMagenta: console_color = FOREGROUND_RED | FOREGROUND_BLUE; break;
            case Hyperion::ELogColor::DarkCyan: console_color = FOREGROUND_BLUE | FOREGROUND_GREEN; break;
            case Hyperion::ELogColor::Grey: console_color = FOREGROUND_INTENSITY; break;
            case Hyperion::ELogColor::White: console_color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; break;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }

        CONSOLE_SCREEN_BUFFER_INFO console_screen_buffer_info;
        GetConsoleScreenBufferInfo(m_console_handle, &console_screen_buffer_info);
        SetConsoleTextAttribute(m_console_handle, console_color);
        unsigned long written_chars = 0;
        WriteConsoleA(m_console_handle, message.c_str(), (DWORD)message.length(), &written_chars, nullptr);
        SetConsoleTextAttribute(m_console_handle, console_screen_buffer_info.wAttributes);
    }

    TString CWindowsOperatingSystem::ConvertUTF16ToUTF8(const TWString &string) const {
        auto utf8_length = WideCharToMultiByte(CP_UTF8, 0, string.c_str(), (int)string.length(), nullptr, 0, nullptr, nullptr);

        char *buffer = new char[utf8_length + 1];
        buffer[utf8_length] = 0;

        WideCharToMultiByte(CP_UTF8, 0, string.c_str(), (int)string.length(), buffer, utf8_length, nullptr, nullptr);

        TString result = buffer;
        delete[] buffer;

        return result;
    }

    std::wstring CWindowsOperatingSystem::ConvertUTF8ToUTF16(const TString &string) const {
        auto utf16_length = MultiByteToWideChar(CP_UTF8, 0, string.c_str(), (int)string.length(), nullptr, 0);
        
        WCHAR *buffer = new WCHAR[utf16_length + 1];
        buffer[utf16_length] = 0;

        MultiByteToWideChar(CP_UTF8, 0, string.c_str(), (int)string.length(), buffer, utf16_length);

        std::wstring result = buffer;
        delete[] buffer;

        return result;
    }

}