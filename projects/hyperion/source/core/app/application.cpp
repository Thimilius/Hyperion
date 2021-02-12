//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/core/app/application.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/core/system/engine.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    Application::Application(const ApplicationSettings &settings) {
        HYP_ASSERT_MESSAGE(!s_instance, "Trying to create application more than once!");
        s_instance = this;

        Engine::s_settings = settings;
    }

    //--------------------------------------------------------------
    void Application::Exit() {
        Engine::Exit();
    }

}