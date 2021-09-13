//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/platform/windows/windows_operating_system.hpp"

//---------------------- Library Includes ----------------------
#include <Powrprof.h>
#include <psapi.h>
#include <Shlobj.h>

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/application.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    // RANT: Apparently Microsoft forgot to include this definition in their header files. What the hell!?!?
    typedef struct _PROCESSOR_POWER_INFORMATION {
        ULONG Number;
        ULONG MaxMhz;
        ULONG CurrentMhz;
        ULONG MhzLimit;
        ULONG MaxIdleState;
        ULONG CurrentIdleState;
    } PROCESSOR_POWER_INFORMATION, *PPROCESSOR_POWER_INFORMATION;

    OperatingSystem* OperatingSystem::s_instance = new WindowsOperatingSystem();

    //--------------------------------------------------------------
    void WindowsOperatingSystem::Initialize() {
        m_console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
        if (!m_console_handle || m_console_handle == INVALID_HANDLE_VALUE) {
            AllocConsole();
            m_console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
        }
    }

    //--------------------------------------------------------------
    SystemInfo WindowsOperatingSystem::GetSystemInfo() const {
        SystemInfo result;

        SYSTEM_INFO system_info;
        GetNativeSystemInfo(&system_info);
        uint32 processor_count = system_info.dwNumberOfProcessors;
        Array<PROCESSOR_POWER_INFORMATION> processor_info(processor_count);
        CallNtPowerInformation(ProcessorInformation, nullptr, 0, processor_info.GetData(), static_cast<uint32>(sizeof(processor_info[0]) * processor_count));
        result.processor_info.processor_count = processor_count;
        result.processor_info.processor_mhz_frequency = processor_info[0].MaxMhz;

        MEMORYSTATUSEX memory_status;
        memory_status.dwLength = sizeof(MEMORYSTATUSEX);
        GlobalMemoryStatusEx(&memory_status);
        result.memory_info.available_physical_memory = memory_status.ullAvailPhys;
        result.memory_info.total_physical_memory = memory_status.ullTotalPhys;
        result.memory_info.available_virtual_memory = memory_status.ullAvailVirtual;
        result.memory_info.total_virtual_memory = memory_status.ullTotalVirtual;

        return result;
    }

    //--------------------------------------------------------------
    uint64 WindowsOperatingSystem::GetMemoryUsage() const {
        PROCESS_MEMORY_COUNTERS process_memory;
        BOOL success = GetProcessMemoryInfo(GetCurrentProcess(), &process_memory, sizeof(process_memory));
        return success ? process_memory.WorkingSetSize : 0;
    }

    //--------------------------------------------------------------
    SystemLanguage WindowsOperatingSystem::GetSystemLanguage() const {
        LANGID language_id = GetUserDefaultUILanguage();
        uint32 main_language  = language_id & 0xff;

        SystemLanguage result = SystemLanguage::Unknown;
		switch (main_language) {
			case LANG_ALBANIAN: result = SystemLanguage::Albanian; break;
			case LANG_ARABIC: result = SystemLanguage::Arabic; break;
			case LANG_ARMENIAN: result = SystemLanguage::Armenian; break;
			case LANG_BELARUSIAN: result = SystemLanguage::Belarusian; break;
			case LANG_BULGARIAN: result = SystemLanguage::Bulgarian; break;
			case LANG_CATALAN: result = SystemLanguage::Catalan; break;
			case LANG_CHINESE: result = SystemLanguage::Chinese; break;
			case LANG_CROATIAN: result = SystemLanguage::Croatian; break;
			case LANG_CZECH: result = SystemLanguage::Czech; break;
			case LANG_DANISH: result = SystemLanguage::Danish; break;
			case LANG_DUTCH: result = SystemLanguage::Dutch; break;
			case LANG_ENGLISH: result = SystemLanguage::English; break;
			case LANG_ESTONIAN: result = SystemLanguage::Estonian; break;
			case LANG_FINNISH: result = SystemLanguage::Finnish; break;
			case LANG_FRENCH: result = SystemLanguage::French; break;
			case LANG_GERMAN: result = SystemLanguage::German; break;
			case LANG_GREEK: result = SystemLanguage::Greek; break;
			case LANG_HEBREW: result = SystemLanguage::Hebrew; break;
			case LANG_HINDI: result = SystemLanguage::Hindi; break;
			case LANG_HUNGARIAN: result = SystemLanguage::Hungarian; break;
			case LANG_ICELANDIC: result = SystemLanguage::Icelandic; break;
			case LANG_INDONESIAN: result = SystemLanguage::Indonesian; break;
			case LANG_IRISH: result = SystemLanguage::Irish; break;
			case LANG_ITALIAN: result = SystemLanguage::Italian; break;
			case LANG_JAPANESE: result = SystemLanguage::Japanese; break;
			case LANG_KOREAN: result = SystemLanguage::Korean; break;
			case LANG_LATVIAN: result = SystemLanguage::Lativian; break;
			case LANG_LITHUANIAN: result = SystemLanguage::Lithuanian; break;
			case LANG_MACEDONIAN: result = SystemLanguage::Macedonian; break;
			case LANG_MALAY: result = SystemLanguage::Malay; break;
			case LANG_MALTESE: result = SystemLanguage::Maltese; break;
			case LANG_NORWEGIAN: result = SystemLanguage::Norwegian; break;
			case LANG_POLISH: result = SystemLanguage::Polish; break;
			case LANG_PORTUGUESE: result = SystemLanguage::Portuguese; break;
			case LANG_ROMANIAN: result = SystemLanguage::Romanian; break;
			case LANG_RUSSIAN: result = SystemLanguage::Russian; break;
			case LANG_SLOVAK: result = SystemLanguage::Slovak; break;
			case LANG_SLOVENIAN: result = SystemLanguage::Slovenian; break;
			case LANG_SPANISH: result = SystemLanguage::Spanish; break;
			case LANG_SWEDISH: result = SystemLanguage::Swedish; break;
			case LANG_THAI: result = SystemLanguage::Thai; break;
			case LANG_TURKISH: result = SystemLanguage::Turkish; break;
			case LANG_UKRAINIAN: result = SystemLanguage::Ukrainian; break;
			case LANG_VIETNAMESE: result = SystemLanguage::Vietnamese; break;
		}
        return result;
    }

    //--------------------------------------------------------------
    String WindowsOperatingSystem::GetSystemFolder(SystemFolder system_folder) const {
        KNOWNFOLDERID folder_id;

        switch (system_folder) {
            case SystemFolder::Desktop: folder_id = FOLDERID_Desktop; break;
            case SystemFolder::Downloads: folder_id = FOLDERID_Downloads; break;
            case SystemFolder::Documents: folder_id = FOLDERID_Documents; break;
            case SystemFolder::Pictures: folder_id = FOLDERID_Pictures; break;
            case SystemFolder::Music: folder_id = FOLDERID_Music; break;
            case SystemFolder::Videos: folder_id = FOLDERID_Videos; break;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }

        PWSTR wide_path;
        HRESULT result = SHGetKnownFolderPath(folder_id, 0, nullptr, &wide_path);
        if (result == S_OK) {
            String path = StringUtils::Utf16ToUtf8(WideString(wide_path));
            CoTaskMemFree(wide_path);
            return path;
        } else {
            return String();
        }
    }

    //--------------------------------------------------------------
    void WindowsOperatingSystem::DisplayError(const String &title, const String &message) {
        MessageBoxW(nullptr, StringUtils::Utf8ToUtf16(message).c_str(), StringUtils::Utf8ToUtf16(title).c_str(), MB_OK | MB_ICONERROR);
    }

    //--------------------------------------------------------------
    void WindowsOperatingSystem::PrintToConsole(LogColor color, const String &message) {
        int16 console_color = 0;
        switch (color) {
            case LogColor::Black: console_color = 0; break;
            case LogColor::Red: console_color = FOREGROUND_INTENSITY | FOREGROUND_RED; break;
            case LogColor::Green: console_color = FOREGROUND_INTENSITY | FOREGROUND_GREEN; break;
            case LogColor::Blue: console_color = FOREGROUND_INTENSITY | FOREGROUND_BLUE; break;
            case LogColor::Yellow: console_color = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN; break;
            case LogColor::Magenta: console_color = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE; break;
            case LogColor::Cyan: console_color = FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN; break;
            case LogColor::DarkRed: console_color = FOREGROUND_RED; break;
            case LogColor::DarkGreen: console_color = FOREGROUND_GREEN; break;
            case LogColor::DarkBlue: console_color = FOREGROUND_BLUE; break;
            case LogColor::DarkYellow: console_color = FOREGROUND_RED | FOREGROUND_GREEN; break;
            case LogColor::DarkMagenta: console_color = FOREGROUND_RED | FOREGROUND_BLUE; break;
            case LogColor::DarkCyan: console_color = FOREGROUND_BLUE | FOREGROUND_GREEN; break;
            case LogColor::Grey: console_color = FOREGROUND_INTENSITY; break;
            case LogColor::White: console_color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; break;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }

        CONSOLE_SCREEN_BUFFER_INFO console_screen_buffer_info;
        GetConsoleScreenBufferInfo(m_console_handle, &console_screen_buffer_info);
        SetConsoleTextAttribute(m_console_handle, console_color);
        unsigned long written_chars = 0;
        WriteConsoleA(m_console_handle, message.c_str(), static_cast<DWORD>(message.length()), &written_chars, nullptr);
        SetConsoleTextAttribute(m_console_handle, console_screen_buffer_info.wAttributes);
    }

    //--------------------------------------------------------------
    String WindowsOperatingSystem::OpenFileDialog(const String &title, const String &filter) {
        WCHAR file_output[MAX_PATH] = { 0 };

        WideString title_utf16 = StringUtils::Utf8ToUtf16(title);

        OPENFILENAMEW open_file_options = { };
        open_file_options.lStructSize = sizeof(open_file_options);
        open_file_options.hwndOwner = static_cast<HWND>(Application::GetInstance()->GetWindow()->GetNativePointer());
        open_file_options.nMaxFile = sizeof(file_output);
        open_file_options.lpstrFile = file_output;
        open_file_options.lpstrFilter = L"All\0*.*\0";
        open_file_options.nFilterIndex = 1;
        open_file_options.lpstrTitle = title_utf16.c_str();
        open_file_options.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        if (GetOpenFileNameW(&open_file_options)) {
            return StringUtils::Utf16ToUtf8(file_output);
        } else {
            return String();
        }
    }

    //--------------------------------------------------------------
    String WindowsOperatingSystem::SaveFileDialog(const String &title, const String &filter) {
        WCHAR file_output[MAX_PATH] = { 0 };

        WideString title_utf16 = StringUtils::Utf8ToUtf16(title);

        OPENFILENAMEW save_file_options = { };
        save_file_options.lStructSize = sizeof(save_file_options);
        save_file_options.hwndOwner = static_cast<HWND>(Application::GetInstance()->GetWindow()->GetNativePointer());
        save_file_options.nMaxFile = sizeof(file_output);
        save_file_options.lpstrFile = file_output;
        save_file_options.lpstrFilter = L"All\0*.*\0";
        save_file_options.nFilterIndex = 1;
        save_file_options.lpstrTitle = title_utf16.c_str();
        save_file_options.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        if (GetSaveFileNameW(&save_file_options)) {
            return StringUtils::Utf16ToUtf8(file_output);
        } else {
            return String();
        }
    }

}