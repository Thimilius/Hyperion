//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/platform/windows/windows_entry_point.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/core/system/main.hpp"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR cmd_line, int cmd_show) {
    return Hyperion::Main::Run();
}