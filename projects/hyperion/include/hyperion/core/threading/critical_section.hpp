#pragma once

#ifdef HYP_PLATFORM_WINDOWS
#include "hyperion/platform/windows/threading/windows_critical_section.hpp"
namespace Hyperion::Threading { using CriticalSection = WindowsCriticalSection; }
#else 
#error Platform not implemented
#endif

