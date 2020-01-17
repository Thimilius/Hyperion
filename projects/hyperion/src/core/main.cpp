#include "hyppch.hpp"

#include "hyperion/core/main.hpp"

namespace Hyperion {

    u32 Main::Run() {
        Application *app = Hyperion::CreateApplication();
        u32 exit_code = app->Run();
        delete app;
        return exit_code;
    }

}