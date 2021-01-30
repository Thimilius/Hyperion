#pragma once

#include "hyperion/core/app/application.hpp"
#include "hyperion/core/system/main.hpp"

// This is the function required to be defined by the application program.
extern Hyperion::Application *Hyperion::CreateApplication();

#if !defined HYP_PLATFORM_WINDOWS || HYP_CONSOLE
int main(int argc, char **argv) {
    return Hyperion::Main::Run();
}
#else
#include <Windows.h>
int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR cmd_line, int cmd_show) {
    return Hyperion::Main::Run();
}
#endif