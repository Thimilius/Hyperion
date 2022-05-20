//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/system/operating_system_types.hpp"
#include "hyperion/platform/windows/windows_types.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
  class Engine;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  using LibraryHandle = HMODULE;

  class WindowsOperatingSystem {
  public:
    inline static OperatingSystemType GetType() { return OperatingSystemType::Windows; }

    static OperatingSystemInfo GetSystemInfo();
    static uint64 GetMemoryUsage();
    static OperatingSystemLanguage GetSystemLanguage();
    static String GetSpecialFolder(OperatingSystemSpecialFolder special_folder);

    static void DisplayError(const String &title, const String &message);
    static void PrintToConsole(LogColor color, const String &message);

    static LibraryHandle LoadLibrary(const String &path);
    template<typename T>
    static T GetFunctionPointer(LibraryHandle handle, const char *name) {
      return reinterpret_cast<T>(GetFunctionPointerRaw(handle, name));
    }
    
    // TODO: Make the filter work.
    static String OpenFileDialog(const String &title, const String &filter);
    static String SaveFileDialog(const String &title, const String &filter);
  private:
    static void Initialize();

    static void *GetFunctionPointerRaw(LibraryHandle handle, const char *name);
  private:
    inline static HANDLE s_console_handle;
  private:
    friend class Hyperion::Engine;
  };

}
