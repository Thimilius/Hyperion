#include "hyppch.hpp"

#include "hyperion/core/main.hpp"

namespace Hyperion {

    u32 Main::Run() {
        Engine::Setup();

        Application *app = Hyperion::CreateApplication();
        
        Engine::PreInit(app->m_starting_settings);
        Engine::Init(app->m_starting_settings);

        u32 exit_code = app->Run();
        delete app;

        return exit_code;
    }

}