//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/core/main.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/application.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    uint32 Main::Run() {
        Engine::Setup();
        Engine::RegisterTypes();

        Application *application = Hyperion::CreateApplication();
        uint32 exit_code = Engine::Run();
        delete application;

        return exit_code;
    }

}