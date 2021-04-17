#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/application.hpp"

// This is the function required to be defined by the application program.
extern Hyperion::Application *Hyperion::CreateApplication();

#ifdef HYP_PLATFORM_WINDOWS
#include "hyperion/platform/windows/windows_entry_point.hpp"
#else 
#error Platform not implemented
#endif