#pragma once

#ifdef HYP_PLATFORM_WINDOWS
#include "hyperion/platform/windows/windows_file_watcher.hpp"
namespace Hyperion {
    using FileWatcher = WindowsFileWatcher;
}
#else 
#error Platform not implemented
#endif