#pragma once

#ifdef HYP_PLATFORM_WINDOWS
#include "hyperion/platform/windows/windows_operating_system.hpp"
namespace Hyperion { using OperatingSystem = WindowsOperatingSystem; }
#else 
#error Platform not implemented
#endif