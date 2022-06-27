//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/menu.hpp"
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

    static void PrintToConsole(ConsoleColor color, const String &message);
    
    static void MessageBox(MessageBoxType type, const String &title, const String &message);

    static LibraryHandle LoadLibrary(const String &path);
    static void UnloadLibrary(LibraryHandle handle);
    template<typename T>
    static T GetFunctionPointer(LibraryHandle handle, const char *name) {
      return reinterpret_cast<T>(GetFunctionPointerRaw(handle, name));
    }
    
    // TODO: Make the filter work.
    static String OpenFileDialog(const String &title, const String &filter);
    static String SaveFileDialog(const String &title, const String &filter);

    static void OpenContextMenu(const Menu &menu);
  private:
    static void Initialize();

    static void *GetFunctionPointerRaw(LibraryHandle handle, const char *name);
  private:
    inline static HANDLE s_console_handle;
  private:
    friend class Hyperion::Engine;
  };

}
