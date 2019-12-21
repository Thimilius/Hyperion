#pragma once

#include "hyperion/core/app/application.hpp"

extern Hyperion::Application *Hyperion::CreateApplication();

#if !defined HYP_PLATFORM_WINDOWS || HYP_CONSOLE
int main(int argc, char **argv) {
    auto *app = Hyperion::CreateApplication();
    int exit_code = app->Run();
    delete app;
    return exit_code;
}
#else
#include <Windows.h>
int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR cmd_line, int cmd_show) {
    auto *app = Hyperion::CreateApplication();
    int exit_code = app->Run();
    delete app;
    return exit_code;
}
#endif