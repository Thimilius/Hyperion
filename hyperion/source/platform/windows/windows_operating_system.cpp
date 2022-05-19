//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/platform/windows/windows_operating_system.hpp"

//---------------------- Library Includes ----------------------
#include <Windows.h>
#include <Powrprof.h>
#include <psapi.h>
#include <Shlobj.h>
#include <dotnet/hostfxr.h>
#include <dotnet/coreclr_delegates.h>

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/application.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  template<typename T>
  T GetFunctionPointer(HMODULE handle, const char *name) {
    return reinterpret_cast<T>(GetProcAddress(handle, name));
  }

  int Function(int arg, const char *string) {
    HYP_LOG_INFO("LUL", "Das isses: {}", arg);
    return 7;
  }
  
  //--------------------------------------------------------------
  void WindowsOperatingSystem::Initialize() {
    s_console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (!s_console_handle || s_console_handle == INVALID_HANDLE_VALUE) {
      AllocConsole();
      s_console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    }

    HMODULE handle = LoadLibraryA("data/tools/dotnet/host/fxr/6.0.5/hostfxr.dll");
    auto init_func = GetFunctionPointer<hostfxr_initialize_for_runtime_config_fn>(handle, "hostfxr_initialize_for_runtime_config");
    auto get_delegate_func = GetFunctionPointer<hostfxr_get_runtime_delegate_fn>(handle, "hostfxr_get_runtime_delegate");
    auto close_func = GetFunctionPointer<hostfxr_close_fn>(handle, "hostfxr_close");

    auto config_path = L"data/managed/Hyperion.runtimeconfig.json";

    hostfxr_handle context;
    int result = init_func(config_path, nullptr, &context);
    assert(result == 0);

    load_assembly_and_get_function_pointer_fn load_assembly_and_get_function_pointer;
    result = get_delegate_func(
            context,
            hdt_load_assembly_and_get_function_pointer,
            reinterpret_cast<void **>(&load_assembly_and_get_function_pointer));
    assert(result == 0);

    const char_t *dotnetlib_path = L"data/managed/Hyperion.dll";
    const char_t *dotnet_type = L"Hyperion.Bootstrapper, Hyperion";
    const char_t *dotnet_type_method = L"Bootstrap";
    // <SnippetLoadAndGet>
    // Function pointer to managed delegate
    component_entry_point_fn hello = nullptr;
    int rc = load_assembly_and_get_function_pointer(
        dotnetlib_path,
        dotnet_type,
        dotnet_type_method,
        nullptr /*delegate_type_name*/,
        nullptr,
        (void**)&hello);
    // </SnippetLoadAndGet>
    assert(rc == 0 && hello != nullptr && "Failure: load_assembly_and_get_function_pointer()");

    close_func(context);
    
    //
    // STEP 4: Run managed code
    //
    struct lib_args
    {
      const char_t *message;
      int number;
      int (*function)(int, const char *);  
    };
    for (int i = 0; i < 3; ++i)
    {
      // <SnippetCallManaged>
      lib_args args
      {
        L"from host!",
        i,
        Function,
      };

      hello(&args, sizeof(args));
      // </SnippetCallManaged>
    }
  }

  //--------------------------------------------------------------
  OperatingSystemInfo WindowsOperatingSystem::GetSystemInfo() {
    OperatingSystemInfo result;

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
  uint64 WindowsOperatingSystem::GetMemoryUsage() {
    PROCESS_MEMORY_COUNTERS process_memory;
    BOOL success = GetProcessMemoryInfo(GetCurrentProcess(), &process_memory, sizeof(process_memory));
    return success ? process_memory.WorkingSetSize : 0;
  }

  //--------------------------------------------------------------
  OperatingSystemLanguage WindowsOperatingSystem::GetSystemLanguage() {
    LANGID language_id = GetUserDefaultUILanguage();
    uint32 main_language = language_id & 0xff;

    OperatingSystemLanguage result = OperatingSystemLanguage::Unknown;
    switch (main_language) {
      case LANG_ALBANIAN: result = OperatingSystemLanguage::Albanian;
        break;
      case LANG_ARABIC: result = OperatingSystemLanguage::Arabic;
        break;
      case LANG_ARMENIAN: result = OperatingSystemLanguage::Armenian;
        break;
      case LANG_BELARUSIAN: result = OperatingSystemLanguage::Belarusian;
        break;
      case LANG_BULGARIAN: result = OperatingSystemLanguage::Bulgarian;
        break;
      case LANG_CATALAN: result = OperatingSystemLanguage::Catalan;
        break;
      case LANG_CHINESE: result = OperatingSystemLanguage::Chinese;
        break;
      case LANG_CROATIAN: result = OperatingSystemLanguage::Croatian;
        break;
      case LANG_CZECH: result = OperatingSystemLanguage::Czech;
        break;
      case LANG_DANISH: result = OperatingSystemLanguage::Danish;
        break;
      case LANG_DUTCH: result = OperatingSystemLanguage::Dutch;
        break;
      case LANG_ENGLISH: result = OperatingSystemLanguage::English;
        break;
      case LANG_ESTONIAN: result = OperatingSystemLanguage::Estonian;
        break;
      case LANG_FINNISH: result = OperatingSystemLanguage::Finnish;
        break;
      case LANG_FRENCH: result = OperatingSystemLanguage::French;
        break;
      case LANG_GERMAN: result = OperatingSystemLanguage::German;
        break;
      case LANG_GREEK: result = OperatingSystemLanguage::Greek;
        break;
      case LANG_HEBREW: result = OperatingSystemLanguage::Hebrew;
        break;
      case LANG_HINDI: result = OperatingSystemLanguage::Hindi;
        break;
      case LANG_HUNGARIAN: result = OperatingSystemLanguage::Hungarian;
        break;
      case LANG_ICELANDIC: result = OperatingSystemLanguage::Icelandic;
        break;
      case LANG_INDONESIAN: result = OperatingSystemLanguage::Indonesian;
        break;
      case LANG_IRISH: result = OperatingSystemLanguage::Irish;
        break;
      case LANG_ITALIAN: result = OperatingSystemLanguage::Italian;
        break;
      case LANG_JAPANESE: result = OperatingSystemLanguage::Japanese;
        break;
      case LANG_KOREAN: result = OperatingSystemLanguage::Korean;
        break;
      case LANG_LATVIAN: result = OperatingSystemLanguage::Lativian;
        break;
      case LANG_LITHUANIAN: result = OperatingSystemLanguage::Lithuanian;
        break;
      case LANG_MACEDONIAN: result = OperatingSystemLanguage::Macedonian;
        break;
      case LANG_MALAY: result = OperatingSystemLanguage::Malay;
        break;
      case LANG_MALTESE: result = OperatingSystemLanguage::Maltese;
        break;
      case LANG_NORWEGIAN: result = OperatingSystemLanguage::Norwegian;
        break;
      case LANG_POLISH: result = OperatingSystemLanguage::Polish;
        break;
      case LANG_PORTUGUESE: result = OperatingSystemLanguage::Portuguese;
        break;
      case LANG_ROMANIAN: result = OperatingSystemLanguage::Romanian;
        break;
      case LANG_RUSSIAN: result = OperatingSystemLanguage::Russian;
        break;
      case LANG_SLOVAK: result = OperatingSystemLanguage::Slovak;
        break;
      case LANG_SLOVENIAN: result = OperatingSystemLanguage::Slovenian;
        break;
      case LANG_SPANISH: result = OperatingSystemLanguage::Spanish;
        break;
      case LANG_SWEDISH: result = OperatingSystemLanguage::Swedish;
        break;
      case LANG_THAI: result = OperatingSystemLanguage::Thai;
        break;
      case LANG_TURKISH: result = OperatingSystemLanguage::Turkish;
        break;
      case LANG_UKRAINIAN: result = OperatingSystemLanguage::Ukrainian;
        break;
      case LANG_VIETNAMESE: result = OperatingSystemLanguage::Vietnamese;
        break;
      default: result = OperatingSystemLanguage::Unknown;
        break;
    }
    return result;
  }

  //--------------------------------------------------------------
  String WindowsOperatingSystem::GetSpecialFolder(OperatingSystemSpecialFolder special_folder) {
    KNOWNFOLDERID folder_id;

    switch (special_folder) {
      case OperatingSystemSpecialFolder::Desktop: folder_id = FOLDERID_Desktop;
        break;
      case OperatingSystemSpecialFolder::Downloads: folder_id = FOLDERID_Downloads;
        break;
      case OperatingSystemSpecialFolder::Documents: folder_id = FOLDERID_Documents;
        break;
      case OperatingSystemSpecialFolder::Pictures: folder_id = FOLDERID_Pictures;
        break;
      case OperatingSystemSpecialFolder::Music: folder_id = FOLDERID_Music;
        break;
      case OperatingSystemSpecialFolder::Videos: folder_id = FOLDERID_Videos;
        break;
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
      case LogColor::Black: console_color = 0;
        break;
      case LogColor::Red: console_color = FOREGROUND_INTENSITY | FOREGROUND_RED;
        break;
      case LogColor::Green: console_color = FOREGROUND_INTENSITY | FOREGROUND_GREEN;
        break;
      case LogColor::Blue: console_color = FOREGROUND_INTENSITY | FOREGROUND_BLUE;
        break;
      case LogColor::Yellow: console_color = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN;
        break;
      case LogColor::Magenta: console_color = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE;
        break;
      case LogColor::Cyan: console_color = FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN;
        break;
      case LogColor::DarkRed: console_color = FOREGROUND_RED;
        break;
      case LogColor::DarkGreen: console_color = FOREGROUND_GREEN;
        break;
      case LogColor::DarkBlue: console_color = FOREGROUND_BLUE;
        break;
      case LogColor::DarkYellow: console_color = FOREGROUND_RED | FOREGROUND_GREEN;
        break;
      case LogColor::DarkMagenta: console_color = FOREGROUND_RED | FOREGROUND_BLUE;
        break;
      case LogColor::DarkCyan: console_color = FOREGROUND_BLUE | FOREGROUND_GREEN;
        break;
      case LogColor::Grey: console_color = FOREGROUND_INTENSITY;
        break;
      case LogColor::White: console_color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
        break;
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
    }

    CONSOLE_SCREEN_BUFFER_INFO console_screen_buffer_info;
    GetConsoleScreenBufferInfo(s_console_handle, &console_screen_buffer_info);
    SetConsoleTextAttribute(s_console_handle, console_color);
    unsigned long written_chars = 0;
    WriteConsoleA(s_console_handle, message.c_str(), static_cast<DWORD>(message.length()), &written_chars, nullptr);
    SetConsoleTextAttribute(s_console_handle, console_screen_buffer_info.wAttributes);
  }

  //--------------------------------------------------------------
  String WindowsOperatingSystem::OpenFileDialog(const String &title, const String &filter) {
    WCHAR file_output[MAX_PATH] = { };

    WideString title_utf16 = StringUtils::Utf8ToUtf16(title);

    OPENFILENAMEW open_file_options = { };
    open_file_options.lStructSize = sizeof(open_file_options);
    open_file_options.hwndOwner = static_cast<HWND>(Application::GetInstance()->GetMainWindow()->GetNativeHandle());
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

  //--------------------------------------------------------------B
  String WindowsOperatingSystem::SaveFileDialog(const String &title, const String &filter) {
    WCHAR file_output[MAX_PATH] = { };

    WideString title_utf16 = StringUtils::Utf8ToUtf16(title);

    OPENFILENAMEW save_file_options = { };
    save_file_options.lStructSize = sizeof(save_file_options);
    save_file_options.hwndOwner = static_cast<HWND>(Application::GetInstance()->GetMainWindow()->GetNativeHandle());
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
