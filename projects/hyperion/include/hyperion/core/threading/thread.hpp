#pragma once

#ifdef HYP_PLATFORM_WINDOWS
#include "hyperion/platform/windows/threading/windows_thread.hpp"
namespace Hyperion::Threading { using Thread = WindowsThread; }
#else 
#error Platform not implemented
#endif