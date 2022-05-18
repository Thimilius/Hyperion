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

  class WindowsOperatingSystem {
  public:
    inline static OperatingSystemType GetType() { return OperatingSystemType::Windows; }

    static OperatingSystemInfo GetSystemInfo();
    static uint64 GetMemoryUsage();
    static OperatingSystemLanguage GetSystemLanguage();
    static String GetSpecialFolder(OperatingSystemSpecialFolder special_folder);

    static void DisplayError(const String &title, const String &message);
    static void PrintToConsole(LogColor color, const String &message);

    // TODO: Make the filter work.
    static String OpenFileDialog(const String &title, const String &filter);
    static String SaveFileDialog(const String &title, const String &filter);
  private:
    static void Initialize();
  private:
    inline static HANDLE s_console_handle;
  private:
    friend class Hyperion::Engine;
  };

}
