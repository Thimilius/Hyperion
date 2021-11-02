//------------------------ Header Guard ------------------------
#pragma once

#ifdef HYP_PLATFORM_WINDOWS
#include "hyperion/platform/windows/app/windows_window.hpp"
namespace Hyperion { using Window = WindowsWindow; }
#else 
#error Platform not implemented
#endif
