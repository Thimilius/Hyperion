#include "hyppch.hpp"

#include "hyperion/core/main.hpp"

namespace Hyperion {

    u32 Main::Run() {
        Engine::Setup();

        Application *application = Hyperion::CreateApplication();
        u32 exit_code = Engine::Run();
        delete application;

        return exit_code;
    }

}