#pragma once

#include "app/application.hpp"

extern Hyperion::CApplication *Hyperion::CreateApplication();

int main(int argc, char **argv) {
    auto *app = Hyperion::CreateApplication();
    int exit_code = app->Run();
    delete app;
    return exit_code;
}