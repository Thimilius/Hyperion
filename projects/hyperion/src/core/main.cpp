#include "hyppch.hpp"

#include "hyperion/core/main.hpp"

namespace Hyperion {

    uint32 Main::Run() {
        Engine::Setup();

        Application *application = Hyperion::CreateApplication();
        uint32 exit_code = Engine::Run();
        delete application;

        return exit_code;
    }

}