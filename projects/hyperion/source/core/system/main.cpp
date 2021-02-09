#include "hyppch.hpp"

#include "hyperion/core/app/application.hpp"
#include "hyperion/core/system/engine.hpp"
#include "hyperion/core/system/main.hpp"

namespace Hyperion {

    uint32 Main::Run() {
        Engine::Setup();

        Application *application = Hyperion::CreateApplication();
        uint32 exit_code = Engine::Run();
        delete application;

        return exit_code;
    }

}