//------------------------ Header Guard ------------------------
#pragma once

#ifdef HYP_PLATFORM_WINDOWS
#include "hyperion/platform/windows/windows_timer.hpp"

namespace Hyperion {
  using Timer = WindowsTimer;
}
#else
#error Platform not implemented
#endif
