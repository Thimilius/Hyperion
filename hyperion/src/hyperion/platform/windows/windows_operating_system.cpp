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

    void CWindowsOperatingSystem::PrintToConsole(ELogColor color, const CString &message) const {
        s16 console_color = FOREGROUND_INTENSITY;
        switch (color) {
            case Hyperion::ELogColor::Black: break;
            case Hyperion::ELogColor::Red: console_color = FOREGROUND_RED; break;
            case Hyperion::ELogColor::Green: console_color = FOREGROUND_GREEN; break;
            case Hyperion::ELogColor::Blue: console_color = FOREGROUND_BLUE; break;
            case Hyperion::ELogColor::Yellow: console_color = FOREGROUND_RED | FOREGROUND_GREEN; break;
            case Hyperion::ELogColor::Magenta: console_color = FOREGROUND_RED | FOREGROUND_BLUE; break;
            case Hyperion::ELogColor::Cyan: console_color = FOREGROUND_BLUE | FOREGROUND_GREEN; break;
            case Hyperion::ELogColor::White: console_color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; break;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }

        CONSOLE_SCREEN_BUFFER_INFO console_screen_buffer_info;
        GetConsoleScreenBufferInfo(m_console_handle, &console_screen_buffer_info);
        SetConsoleTextAttribute(m_console_handle, console_color);
        unsigned long written_chars = 0;
        WriteConsoleA(m_console_handle, message.ToCString(), message.GetLength(), &written_chars, nullptr);
        SetConsoleTextAttribute(m_console_handle, console_screen_buffer_info.wAttributes);
    }

}